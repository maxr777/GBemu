#include "../platform/desktop.h"
#include "constants.h"
#include "gameboy.h"
#include <assert.h>

/*
 * TODO: Remember to write the assert for read-only addresses when
 * implementing mbc1_write()
 */

void
write16(Gameboy *gb, const u16 addr, const u16 val) {
	write8(gb, addr, val);
	write8(gb, addr + 1, val >> 8);
}

u16
read16(const Gameboy *gb, const u16 addr) {
	return read8(gb, addr) | (read8(gb, addr + 1) << 8);
}

void
write8(Gameboy *gb, const u16 addr, const u8 val) {
#ifdef CPU_TEST
	gb->memory.test_memory[addr] = val;
#else
	// if (addr < ROM_BANK_N_ADDR)
	// 	rom_write(addr, val);
	// else if (addr < VRAM_ADDR)
	// 	rom_write(addr, val);
	// else if (addr < EXTERN_RAM_ADDR)
	// 	vram[addr - VRAM_ADDR] = val;
	// else if (addr < WRAM_0_ADDR)
	// 	rom_write(addr, val);
	// else if (addr < WRAM_N_ADDR)
	// 	;
	// else if (addr < ECHO_RAM_ADDR)
	// 	ram[addr - WRAM_0_ADDR] = val;
	// else if (addr < OAM_ADDR)
	// 	fprintf(stderr, "write8: use of echo ram is prohibited\n");
	// else if (addr < INVAL_MEM_ADDR)
	// 	oam[addr - OAM_ADDR] = val;
	// else if (addr < IO_REGS_ADDR)
	// 	fprintf(stderr, "write8: use of 0xFEA0-0xFEFF is prohibited\n");
	// else if (addr < HRAM_ADDR) {
	// 	io_registers[addr - IO_REGS_ADDR] = val;
	// 	if (addr == SERIAL_TRANSFER)
	// 		printf("%c", val);
	// 	else if (addr == DIV_ADDR) {
	// 		io_registers[addr - IO_REGS_ADDR] = 0;
	// 		timer_controls.div_cycle_counter = 0;
	// 	} else if (addr == TAC_ADDR) {
	// 		timer_controls.tac_enable = val & 0x04;
	// 		switch (val & 0x03) {
	// 		case 0x00:
	// 			timer_controls.tac_increment_cycles = TAC_00_CYCLES;
	// 			break;
	// 		case 0x01:
	// 			timer_controls.tac_increment_cycles = TAC_01_CYCLES;
	// 			break;
	// 		case 0x02:
	// 			timer_controls.tac_increment_cycles = TAC_10_CYCLES;
	// 			break;
	// 		case 0x03:
	// 			timer_controls.tac_increment_cycles = TAC_11_CYCLES;
	// 			break;
	// 		}
	// 	}
	// } else if (addr < INT_ENABLE_ADDR)
	// 	hram[addr - HRAM_ADDR] = val;
	// else {
	// 	fprintf(stderr, "IE writes aren't implemented yet\n");
	// }
#endif
}

u8
mbc1_read(const Gameboy *gb, const u16 addr) {
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
		if (!gb->mbc1.ram_enable) return 0xFF;
		return gb->rom.external_ram[(addr - EXTERN_RAM_ADDR) + (gb->rom.current_ram_bank * EXTERN_RAM_SIZE)];
	}
}

u8
rom_read(const Gameboy *gb, const u16 addr) {
	if (gb->rom.boot_rom_enabled) {
		assert(addr <= 0x00FF);
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

u8
read8(const Gameboy *gb, const u16 addr) {
#ifdef CPU_TEST
	return gb->memory.test_memory[addr];
#else
	if (addr < ROM_BANK_N_ADDR)
		return rom_read(gb, addr);
	else if (addr < VRAM_ADDR)
		return rom_read(gb, addr);
	else if (addr < EXTERN_RAM_ADDR)
		return gb->memory.vram[addr - VRAM_ADDR];
	else if (addr < WRAM_0_ADDR)
		return rom_read(gb, addr);
	else if (addr < WRAM_N_ADDR)
		return gb->memory.ram[addr - WRAM_0_ADDR];
	else if (addr < ECHO_RAM_ADDR)
		return gb->memory.ram[addr - WRAM_0_ADDR];
	else if (addr < OAM_ADDR) {
		return gb->memory.ram[addr - ECHO_RAM_ADDR];
	} else if (addr < INVAL_MEM_ADDR)
		return gb->memory.oam[addr - OAM_ADDR];
	else if (addr < IO_REGS_ADDR) {
		platform_error_log("read8: use of 0xFEA0-0xFEFF is prohibited\n");
		return 0;
	} else if (addr < HRAM_ADDR) {
		// https://github.com/Gekkio/mooneye-test-suite?tab=readme-ov-file#passfail-reporting
		// If you don't have a full Game boy system, pass/fail reporting can be sped up by making
		// sure LY (0xff44) and SC (0xff02) both return 0xff when read. This will bypass some
		// unnecessary drawing code and waiting for serial transfer to finish.
		if ((addr == SERIAL_CONTROL) || (addr == LCD_Y)) return 0xFF;
		return gb->memory.io_registers[addr - IO_REGS_ADDR];
	} else if (addr < INT_ENABLE_ADDR)
		return gb->memory.hram[addr - HRAM_ADDR];
	else {
		platform_error_log("IE reads aren't implemented yet\n");
	}
	return 0;
#endif
}
