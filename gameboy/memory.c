#include "constants.h"
#include "gameboy.h"

#ifdef CPU_TEST
uint8_t test_memory[0xFFFF + 1] = {0};
#endif

void write16(Memory *mem, const u16 addr, const u16 val) {
	write8(mem, addr, val);
	write8(mem, addr + 1, val >> 8);
}

u16 read16(const Memory *mem, const u16 addr) {
	return read8(mem, addr) | (read8(mem, addr + 1) << 8);
}

void write8(Memory *mem, const u16 addr, const u8 val) {
#ifdef CPU_TEST
	test_memory[addr] = val;
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

u8 read8(const Memory *mem, const u16 addr) {
#ifdef CPU_TEST
	return test_memory[addr];
#else
	// if (addr < ROM_BANK_N_ADDR)
	// 	return rom_read(addr);
	// else if (addr < VRAM_ADDR)
	// 	return rom_read(addr);
	// else if (addr < EXTERN_RAM_ADDR)
	// 	return vram[addr - VRAM_ADDR];
	// else if (addr < WRAM_0_ADDR)
	// 	return rom_read(addr);
	// else if (addr < WRAM_N_ADDR)
	// 	return ram[addr - WRAM_0_ADDR];
	// else if (addr < ECHO_RAM_ADDR)
	// 	return ram[addr - WRAM_0_ADDR];
	// else if (addr < OAM_ADDR) {
	// 	fprintf(stderr, "read8: use of echo ram is prohibited\n");
	// 	return 0;
	// } else if (addr < INVAL_MEM_ADDR)
	// 	return oam[addr - OAM_ADDR];
	// else if (addr < IO_REGS_ADDR) {
	// 	fprintf(stderr, "read8: use of 0xFEA0-0xFEFF is prohibited\n");
	// 	return 0;
	// } else if (addr < HRAM_ADDR) {
	// 	// https://github.com/Gekkio/mooneye-test-suite?tab=readme-ov-file#passfail-reporting
	// 	// If you don't have a full Game boy system, pass/fail reporting can be sped up by making
	// 	// sure LY (0xff44) and SC (0xff02) both return 0xff when read. This will bypass some
	// 	// unnecessary drawing code and waiting for serial transfer to finish.
	// 	if ((addr == SERIAL_CONTROL) || (addr == LCD_Y)) return 0xFF;
	// 	return io_registers[addr - IO_REGS_ADDR];
	// } else if (addr < INT_ENABLE_ADDR)
	// 	return hram[addr - HRAM_ADDR];
	// else
	// 	fprintf(stderr, "IE reads aren't implemented yet\n");

	return 0;
#endif
}
