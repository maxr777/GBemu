#include "../gameboy/gameboy.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROM_FILL  0x11
#define RAM_FILL  0x22
#define VRAM_FILL 0x33
#define OAM_FILL  0x44
#define IO_FILL	  0x55
#define HRAM_FILL 0x66

static void
test_memory_read(void) {
	puts("Test: Memory map reads");
	fflush(stdout);

	Gameboy gb = {};

	const int game_size = 524288;
	gb.rom.game_rom = malloc(game_size);
	memset(gb.rom.game_rom, ROM_FILL, game_size);

	memset(gb.memory.ram, RAM_FILL, sizeof(gb.memory.ram));
	memset(gb.memory.vram, VRAM_FILL, sizeof(gb.memory.vram));
	memset(gb.memory.oam, OAM_FILL, sizeof(gb.memory.oam));
	memset(gb.memory.io_registers, IO_FILL, sizeof(gb.memory.io_registers));
	memset(gb.memory.hram, HRAM_FILL, sizeof(gb.memory.hram));

	// Distinct edge markers so an off-by-one into the next region fails.
	gb.rom.game_rom[ROM_BANK_0_ADDR] = 0xB0;
	gb.rom.game_rom[ROM_BANK_N_ADDR - 1] = 0xB1;
	gb.rom.game_rom[ROM_BANK_N_ADDR] = 0xB2;
	gb.rom.game_rom[VRAM_ADDR - 1] = 0xB3;

	gb.memory.vram[0] = 0xC0;
	gb.memory.vram[VRAM_SIZE - 1] = 0xC1;

	gb.memory.ram[0] = 0xD0;			    // C000
	gb.memory.ram[WRAM_SIZE - 1] = 0xD1;		    // CFFF
	gb.memory.ram[WRAM_SIZE] = 0xD2;		    // D000
	gb.memory.ram[OAM_ADDR - ECHO_RAM_ADDR - 1] = 0xD3; // DDFF, last mirrored byte
	gb.memory.ram[sizeof(gb.memory.ram) - 1] = 0xD4;    // DFFF, not mirrored

	gb.memory.oam[0] = 0xE0;
	gb.memory.oam[sizeof(gb.memory.oam) - 1] = 0xE1;

	gb.memory.io_registers[0] = 0xF0;
	gb.memory.io_registers[sizeof(gb.memory.io_registers) - 1] = 0xF1;

	gb.memory.hram[0] = 0xA0;
	gb.memory.hram[sizeof(gb.memory.hram) - 1] = 0xA1;

	gb.memory.ram[0x0100] = 0x34;
	gb.memory.ram[0x0101] = 0x12;

	// ROM bank 00
	assert(read8(&gb, ROM_BANK_0_ADDR) == 0xB0);
	assert(read8(&gb, ROM_BANK_N_ADDR - 1) == 0xB1);
	assert(read8(&gb, ROM_BANK_0_ADDR + 0x0100) == ROM_FILL);

	// ROM bank 01-NN
	assert(read8(&gb, ROM_BANK_N_ADDR) == 0xB2);
	assert(read8(&gb, VRAM_ADDR - 1) == 0xB3);
	assert(read8(&gb, ROM_BANK_N_ADDR + 0x0100) == ROM_FILL);

	// VRAM
	assert(read8(&gb, VRAM_ADDR) == 0xC0);
	assert(read8(&gb, EXTERN_RAM_ADDR - 1) == 0xC1);
	assert(read8(&gb, VRAM_ADDR + 0x1000) == VRAM_FILL);
	assert(read8(&gb, VRAM_ADDR) != read8(&gb, VRAM_ADDR - 1));
	assert(read8(&gb, EXTERN_RAM_ADDR - 1) != read8(&gb, VRAM_ADDR - 1));

	// WRAM 0 / WRAM N
	assert(read8(&gb, WRAM_0_ADDR) == 0xD0);
	assert(read8(&gb, WRAM_N_ADDR - 1) == 0xD1);
	assert(read8(&gb, WRAM_N_ADDR) == 0xD2);
	assert(read8(&gb, ECHO_RAM_ADDR - 1) == 0xD4);
	assert(read8(&gb, WRAM_0_ADDR + 0x0100) == 0x34);
	assert(read8(&gb, WRAM_N_ADDR + 0x00C0) == RAM_FILL);

	// Echo RAM
	assert(read8(&gb, ECHO_RAM_ADDR) == read8(&gb, WRAM_0_ADDR));
	assert(read8(&gb, ECHO_RAM_ADDR) == 0xD0);
	assert(read8(&gb, ECHO_RAM_ADDR + 0x0FFF) == read8(&gb, WRAM_N_ADDR - 1));
	assert(read8(&gb, ECHO_RAM_ADDR + 0x1000) == read8(&gb, WRAM_N_ADDR));
	assert(read8(&gb, OAM_ADDR - 1) == read8(&gb, WRAM_0_ADDR + (OAM_ADDR - ECHO_RAM_ADDR - 1)));
	assert(read8(&gb, OAM_ADDR - 1) == 0xD3);
	assert(read8(&gb, ECHO_RAM_ADDR + 0x0100) == read8(&gb, WRAM_0_ADDR + 0x0100));
	assert(read8(&gb, ECHO_RAM_ADDR + 0x10C0) == read8(&gb, WRAM_N_ADDR + 0x00C0));

	for (u16 off = 0; off < (OAM_ADDR - ECHO_RAM_ADDR); ++off) {
		assert(read8(&gb, ECHO_RAM_ADDR + off) == gb.memory.ram[off]);
		assert(read8(&gb, WRAM_0_ADDR + off) == gb.memory.ram[off]);
	}

	// OAM
	assert(read8(&gb, OAM_ADDR) == 0xE0);
	assert(read8(&gb, INVAL_MEM_ADDR - 1) == 0xE1);
	assert(read8(&gb, OAM_ADDR + 0x0001) == OAM_FILL);
	assert(read8(&gb, OAM_ADDR) != read8(&gb, OAM_ADDR - 1));

	// 0xFEA0-0xFEFF is unused and reads as 0
	assert(read8(&gb, INVAL_MEM_ADDR) == 0);
	assert(read8(&gb, IO_REGS_ADDR - 1) == 0);
	assert(read8(&gb, INVAL_MEM_ADDR) != read8(&gb, INVAL_MEM_ADDR - 1));

	// I/O registers; LY and SC are hardwired to 0xFF for test reporting
	assert(read8(&gb, IO_REGS_ADDR) == 0xF0);
	assert(read8(&gb, HRAM_ADDR - 1) == 0xF1);
	assert(read8(&gb, IO_REGS_ADDR + 0x000A) == IO_FILL);
	assert(read8(&gb, SERIAL_CONTROL) == 0xFF);
	assert(read8(&gb, LCD_Y) == 0xFF);
	assert(read8(&gb, IO_REGS_ADDR) != read8(&gb, IO_REGS_ADDR - 1));
	assert(read8(&gb, HRAM_ADDR - 1) != read8(&gb, HRAM_ADDR));

	// HRAM
	assert(read8(&gb, HRAM_ADDR) == 0xA0);
	assert(read8(&gb, INT_ENABLE_ADDR - 1) == 0xA1);
	assert(read8(&gb, HRAM_ADDR + 0x0008) == HRAM_FILL);

	// IE is not implemented yet
	assert(read8(&gb, INT_ENABLE_ADDR) == 0);
	assert(read8(&gb, INT_ENABLE_ADDR) != read8(&gb, INT_ENABLE_ADDR - 1));

	assert(read16(&gb, WRAM_0_ADDR + 0x0100) == 0x1234);
	assert(read16(&gb, ECHO_RAM_ADDR + 0x0100) == 0x1234);
	assert(read16(&gb, HRAM_ADDR) == (u16)(0xA0 | (HRAM_FILL << 8)));

	free(gb.rom.game_rom);
}
