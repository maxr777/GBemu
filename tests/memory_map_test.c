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

static void test_memory_read() {
	puts("Test: Load a correct ROM");
	fflush(stdout);

	Gameboy gb = {};

	const int game_size = 524288;
	gb.rom.game_rom = malloc(game_size);
	memset(gb.rom.game_rom, ROM_FILL, game_size);

	memset(gb.memory.ram, RAM_FILL, sizeof(gb.memory.ram));
	memset(gb.memory.ram, RAM_FILL, sizeof(gb.memory.ram));
	memset(gb.memory.vram, VRAM_FILL, sizeof(gb.memory.vram));
	memset(gb.memory.oam, OAM_FILL, sizeof(gb.memory.oam));
	memset(gb.memory.io_registers, IO_FILL, sizeof(gb.memory.io_registers));
	memset(gb.memory.hram, HRAM_FILL, sizeof(gb.memory.hram));

	assert(read8(&gb, WRAM_0_ADDR + 0x0100) == RAM_FILL);
	assert(read8(&gb, WRAM_N_ADDR + 0x00C0) == RAM_FILL);
	assert(read8(&gb, VRAM_ADDR + 0x1000) == VRAM_FILL);
	assert(read8(&gb, OAM_ADDR + 0x0001) == OAM_FILL);
	assert(read8(&gb, IO_REGS_ADDR + 0x000A) == IO_FILL);
	assert(read8(&gb, HRAM_ADDR + 0x0008) == HRAM_FILL);
}
