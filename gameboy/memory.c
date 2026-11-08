#include "../platform/platform.h"
#include "constants.h"
#include "gameboy.h"
#include <assert.h>

/*
 * The CPU versions of write and read are there because, when the CPU issues those memory accesses,
 * the timer should tick - for example, when the CPU fetches an operand, the timer should tick once.
 * When it fetches another byte, it should tick again. This gets rid of plain ++cycle
 * increments everywhere and allows for more precise implementation of the timer.
 */

static void cpu_write16(Gameboy *gb, const u16 addr, const u16 val) {
	cpu_write8(gb, addr, val);
	cpu_write8(gb, addr + 1, val >> 8);
}

static void write16(Gameboy *gb, const u16 addr, const u16 val) {
	write8(gb, addr, val);
	write8(gb, addr + 1, val >> 8);
}

static u16 cpu_read16(Gameboy *gb, const u16 addr) {
	u8 low = cpu_read8(gb, addr);
	u8 high = cpu_read8(gb, addr + 1);
	return low | (high << 8);
}

static u16 read16(const Gameboy *gb, const u16 addr) {
	u8 low = read8(gb, addr);
	u8 high = read8(gb, addr + 1);
	return low | (high << 8);
}

// addresses: https://gbdev.io/pandocs/MBC1.html
static void mbc1_write(Gameboy *gb, const u16 addr, const u8 val) {
	if (addr < 0x2000) { // RAM enable
		if ((val & 0x0F) == 0x0A) {
			gb->mbc1.ram_enable = true;
		} else {
			gb->mbc1.ram_enable = false;
		}
	} else if (addr < 0x4000) { // ROM bank number
		gb->mbc1.first_rom_bank_reg = val & 0x1F;
		gb->rom.current_rom_bank = (gb->mbc1.second_rom_bank_reg << 5) + gb->mbc1.first_rom_bank_reg;
		gb->rom.current_rom_bank &= gb->rom.max_rom_banks - 1;
		if (gb->rom.current_rom_bank == 0) {
			gb->rom.current_rom_bank = 1;
		}
	} else if (addr < 0x6000) { // RAM bank number/upper bits of ROM bank number
		gb->mbc1.second_rom_bank_reg = val & 0x03;
		gb->rom.current_rom_bank = (gb->mbc1.second_rom_bank_reg << 5) + gb->mbc1.first_rom_bank_reg;
		gb->rom.current_rom_bank &= gb->rom.max_rom_banks - 1;
		if (gb->rom.current_rom_bank == 0) {
			gb->rom.current_rom_bank = 1;
		}
		if (gb->mbc1.banking_mode_is_advanced) {
			gb->rom.current_ram_bank = val & 0x03;
		}
	} else if (addr < 0x8000) { // banking mode select
		gb->mbc1.banking_mode_is_advanced = val & 0x01;
	} else { // ofc vram is in between extern ram and rom banks, but that gets distinguished in write8
		if (gb->mbc1.ram_enable) {
			gb->rom.external_ram[(addr - EXTERN_RAM_ADDR) + (gb->rom.current_ram_bank * EXTERN_RAM_SIZE)] = val;
		}
	}
}

static void rom_write(Gameboy *gb, const u16 addr, const u8 val) {
	switch (gb->rom.cartridge_header.cartridge_type) {
	case 0x00:
		// This does nothing
		break;
	case 0x01:
	case 0x02:
	case 0x03:
		mbc1_write(gb, addr, val);
		break;
	default:
		assert(!"rom_write(): Unimplemented MCB\n");
		break;
	}
}

static void cpu_write8(Gameboy *gb, const u16 addr, const u8 val) {
	write8(gb, addr, val);
	cpu_tick(gb);
}

static void write8(Gameboy *gb, const u16 addr, const u8 val) {
#ifdef CPU_TEST
	gb->memory.test_memory[addr] = val;
#else
	if (addr < ROM_BANK_N_ADDR) {
		rom_write(gb, addr, val);
	} else if (addr < VRAM_ADDR) {
		rom_write(gb, addr, val);
	} else if (addr < EXTERN_RAM_ADDR) {
		gb->memory.vram[addr - VRAM_ADDR] = val;
	} else if (addr < WRAM_0_ADDR) {
		rom_write(gb, addr, val);
	} else if (addr < WRAM_N_ADDR) {
		gb->memory.ram[addr - WRAM_0_ADDR] = val;
	} else if (addr < ECHO_RAM_ADDR) {
		gb->memory.ram[addr - WRAM_0_ADDR] = val;
	} else if (addr < OAM_ADDR) {
		assert(!"write8: use of echo ram is prohibited\n");
	} else if (addr < INVAL_MEM_ADDR) {
		gb->memory.oam[addr - OAM_ADDR] = val;
	} else if (addr < IO_REGS_ADDR) {
		assert(!"write8: use of 0xFEA0-0xFEFF is prohibited\n");
	} else if (addr < HRAM_ADDR) {
		u8 *reg = &gb->memory.io_registers[addr - IO_REGS_ADDR];
		bool timer_signal_old = timer_signal(gb);

		/*
		 * Each register does its own store, so it can decide whether (and how) the write lands.
		 * Registers without special behavior just store the value in the default case.
		 */
		switch (addr) {
		case BOOT_ROM_DISABLE: {
			*reg = val;
			if (val != 0) {
				gb->rom.boot_rom_enabled = false;
			}
			break;
		}
		case SERIAL_TRANSFER: {
			*reg = val;
			platform_serial_print(val);
			break;
		}
		case DIV_ADDR: {
			/*
			 * Writing to DIV sets the timer to 0.
			 * DIV is read from gb->timer, so we don't need to use io_registers
			 */
			gb->timer = 0;
			tima_update(gb, timer_signal_old);
			break;
		}
		case IF_ADDR: {
			*reg = val | 0xE0;
			break;
		}
		case TIMA_ADDR: {
			/*
			 * https://gbdev.io/pandocs/Timer_Obscure_Behaviour.html#timer-overflow-behavior
			 *
			 * Writing to TIMA during cycle B (TIMA_RELOADING) will be ignored; TIMA will
			 * be equal to TMA at the end of the cycle anyway.
			 */
			if (gb->tima_state == TIMA_RELOADING) {
				break;
			}
			/*
			 * https://gbdev.io/pandocs/Timer_Obscure_Behaviour.html#timer-overflow-behavior
			 *
			 * Writing to TIMA during cycle A (TIMA_OVERFLOWED here) acts as if the overflow
			 * didn’t happen! TMA will not be copied to TIMA (the value written will therefore
			 * stay), and bit 2 of IF will not be set. Writing to DIV, TAC, or other registers
			 * won’t prevent the IF flag from being set or TIMA from being reloaded.
			 */
			if (gb->tima_state == TIMA_OVERFLOWED) {
				gb->tima_state = TIMA_NORMAL;
			}
			*reg = val;
			break;
		}
		case TMA_ADDR: {
			*reg = val;
			/*
			 * https://gbdev.io/pandocs/Timer_Obscure_Behaviour.html#timer-overflow-behavior
			 *
			 * Writing to TMA during cycle B (TIMA_RELOADING) will have the same
			 * value copied to TIMA as well, on the same cycle.
			 */
			if (gb->tima_state == TIMA_RELOADING) {
				gb->memory.io_registers[TIMA_ADDR - IO_REGS_ADDR] = val;
			}
			break;
		}
		case TAC_ADDR: {
			// Bits 7-3 read as ones and have no writeable function
			*reg = val | 0xF8;
			tima_update(gb, timer_signal_old);
			break;
		}
		default: {
			*reg = val;
			break;
		}
		}
	} else if (addr < IE_ADDR) {
		gb->memory.hram[addr - HRAM_ADDR] = val;
	} else {
		gb->memory.ie = val;
		gb->memory.ie |= 0xE0;
	}
#endif
}

static u8 mbc1_read(const Gameboy *gb, const u16 addr) {
	assert(addr < WRAM_0_ADDR && !(addr >= VRAM_ADDR && addr < WRAM_0_ADDR));

	if (addr < ROM_BANK_N_ADDR) { // ROM bank X0
		if (gb->mbc1.banking_mode_is_advanced) {
			uint8_t bank = (gb->mbc1.second_rom_bank_reg << 5) & (gb->rom.max_rom_banks - 1);
			return gb->rom.game_rom[addr + (ROM_BANK_SIZE * bank)];
		}
		return gb->rom.game_rom[addr];
	} else if (addr < EXTERN_RAM_ADDR) { // ROM bank 01-7F
		u16 address_actual = (addr - ROM_BANK_N_ADDR) + (ROM_BANK_SIZE * gb->rom.current_rom_bank);
		return gb->rom.game_rom[address_actual];
	} else { // RAM bank 00-03 (if any)
		if (!gb->mbc1.ram_enable) {
			return 0xFF;
		}
		return gb->rom.external_ram[(addr - EXTERN_RAM_ADDR) + (gb->rom.current_ram_bank * EXTERN_RAM_SIZE)];
	}
}

static u8 rom_read(const Gameboy *gb, const u16 addr) {
	if (gb->rom.boot_rom_enabled && addr <= 0x00FF) {
		return gb->rom.boot_rom[addr];
	}

	switch (gb->rom.cartridge_header.cartridge_type) {
	case 0x00:
		return gb->rom.game_rom[addr];
	case 0x01:
	case 0x02:
	case 0x03:
		return mbc1_read(gb, addr);
	default:
		platform_error_log("rom_read(): Unimplemented MCB\n");
		return 0;
	}
}

static u8 cpu_read8(Gameboy *gb, const u16 addr) {
	u8 val = read8(gb, addr);
	cpu_tick(gb);
	return val;
}

static u8 read8(const Gameboy *gb, const u16 addr) {
#ifdef CPU_TEST
	return gb->memory.test_memory[addr];
#else
	if (addr < ROM_BANK_N_ADDR) {
		return rom_read(gb, addr);
	} else if (addr < VRAM_ADDR) {
		return rom_read(gb, addr);
	} else if (addr < EXTERN_RAM_ADDR) {
		return gb->memory.vram[addr - VRAM_ADDR];
	} else if (addr < WRAM_0_ADDR) {
		return rom_read(gb, addr);
	} else if (addr < WRAM_N_ADDR) {
		return gb->memory.ram[addr - WRAM_0_ADDR];
	} else if (addr < ECHO_RAM_ADDR) {
		return gb->memory.ram[addr - WRAM_0_ADDR];
	} else if (addr < OAM_ADDR) {
		return gb->memory.ram[addr - ECHO_RAM_ADDR];
	} else if (addr < INVAL_MEM_ADDR) {
		return gb->memory.oam[addr - OAM_ADDR];
	} else if (addr < IO_REGS_ADDR) {
		return 0;
	} else if (addr < HRAM_ADDR) {
		if (addr == DIV_ADDR) {
			/*
			 * T-cycle = one tick of the master oscillator, ~4.194304 MHz
			 * M-cycle = 4 T-cycles, basically everything in the gameboy works on those:
			 *
			 * NOP takes 1 M-cycle (4 T-cycles),
			 * LD A, (HL) takes 2 M-cycles (8 T-cycles; opcode fetch then memory read),
			 * CALL nn takes 6 M-cycles (24 T-cycles).
			 *
			 * Basically, you can use M-cycles for everything, and that's what I do in this emulator.
			 * But the hardware timer ticks at T-cycles and mine ticks at M-cycles, so instead of just
			 * returning gb->timer >> 8 I have to multiply it by 4 first
			 * ((timer * 4) >> 8 == (timer << 2) >> 8 == timer >> 6). Timer in M-cycles is basically
			 * timer in T-cycles with the lowest 2 bits removed.
			 */
			return gb->timer >> 6;
		}
		/*
		 * https://github.com/Gekkio/mooneye-test-suite?tab=readme-ov-file#passfail-reporting
		 * If you don't have a full Game boy system, pass/fail reporting can be sped up by making
		 * sure LY (0xff44) and SC (0xff02) both return 0xff when read. This will bypass some
		 * unnecessary drawing code and waiting for serial transfer to finish.
		 */
		if ((addr == SERIAL_CONTROL) || (addr == LCD_Y)) {
			return 0xFF;
		}
		return gb->memory.io_registers[addr - IO_REGS_ADDR];
	} else if (addr < IE_ADDR) {
		return gb->memory.hram[addr - HRAM_ADDR];
	} else {
		return gb->memory.ie;
	}
	return 0;
#endif
}
