#include "../gameboy/gameboy.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_CYCLES   100000000
#define MOONEYE_PATH "tests/mooneye-tests/"
#define TEST_COUNT   14
const char *tests[TEST_COUNT] = {"acceptance/div_timing", "acceptance/timer/tim01", "acceptance/timer/tim10", "acceptance/timer/tim11", "acceptance/timer/tim00", "acceptance/timer/div_write",
				 "acceptance/timer/tim01_div_trigger", "acceptance/timer/tim10_div_trigger", "acceptance/timer/tim11_div_trigger", "acceptance/timer/tim00_div_trigger",
				 "acceptance/timer/rapid_toggle", "acceptance/timer/tima_reload", "acceptance/timer/tima_write_reloading", "acceptance/timer/tma_write_reloading"};

bool mooneye_passed(Gameboy *gb) {
	return gb->cpu.regs[BC].high == 3 &&
	       gb->cpu.regs[BC].low == 5 &&
	       gb->cpu.regs[DE].high == 8 &&
	       gb->cpu.regs[DE].low == 13 &&
	       gb->cpu.regs[HL].high == 21 &&
	       gb->cpu.regs[HL].low == 34;
}

static bool mooneye_failed(const Gameboy *gb) {
	return gb->cpu.regs[BC].high == 0x42 &&
	       gb->cpu.regs[BC].low == 0x42 &&
	       gb->cpu.regs[DE].high == 0x42 &&
	       gb->cpu.regs[DE].low == 0x42 &&
	       gb->cpu.regs[HL].high == 0x42 &&
	       gb->cpu.regs[HL].low == 0x42;
}

void test_mooneye() {
	for (int i = 0; i < TEST_COUNT; ++i) {
		char path[256];
		snprintf(path, sizeof(path), "%s%s.gb", MOONEYE_PATH, tests[i]);
		printf("Test: %s\n", path);
		fflush(stdout);

		Gameboy gb = {};
		gameboy_initialize(path, &gb);

		// Disable boot rom - the current boot rom doesn't work well with this trick:
		// https://github.com/Gekkio/mooneye-test-suite?tab=readme-ov-file#passfail-reporting
		// "If you don't have a full Game boy system, pass/fail reporting can be sped up by making
		// sure LY (0xff44) and SC (0xff02) both return 0xff when read. This will bypass some
		// unnecessary drawing code and waiting for serial transfer to finish."
		gb.rom.boot_rom_enabled = false;
		gb.cpu.regs[PC].full = 0x0100;
		gb.cpu.regs[SP].full = 0xFFFE;

		u8 opcode = 0;
		while (opcode != 0x40) {
			if (gb.cpu.cycle > MAX_CYCLES) assert(!"Fail: test reached max amount of cycles");

			opcode = read8(&gb, gb.cpu.regs[PC].full);
			gameboy_step(&gb, false);
		}

		if (mooneye_passed(&gb)) continue;

		if (mooneye_failed(&gb)) assert(!"Fail: test failed cleanly (0x42 everywhere)\n");
		else assert(!"Fail: test failed non-cleanly\n");
	}
}
