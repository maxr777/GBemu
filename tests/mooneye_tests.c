#include "../gameboy/gameboy.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_CYCLES   100000000
#define MOONEYE_PATH "tests/mooneye-tests/"
#define TEST_COUNT   14
const char *tests[TEST_COUNT] = {"acceptance/div_timing", "timer/tim01", "timer/tim10", "timer/tim11", "timer/tim00", "timer/div_write",
				 "timer/tim01_div_trigger", "timer/tim10_div_trigger", "timer/tim11_div_trigger", "timer/tim00_div_trigger",
				 "timer/rapid_toggle", "timer/tima_reload", "timer/tima_write_reloading", "timer/tima_write_reloading"};

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
	puts("Test: Memory map reads");
	fflush(stdout);

	for (int i = 0; i < TEST_COUNT; ++i) {
		char path[256];
		snprintf(path, sizeof(path), "%s%s.gb", MOONEYE_PATH, tests[i]);

		Gameboy gb = {};
		gameboy_initialize(path, &gb);

		u8 opcode = 0;
		while (opcode != 0x40) {
			if (gb.cpu.cycle > MAX_CYCLES) {
				fprintf(stderr, "%s%s - over %d cycles - test failed\n", MOONEYE_PATH, tests[i], MAX_CYCLES);
				assert(0);
			}

			opcode = read8(&gb, gb.cpu.regs[PC].full);
			opcode_execute(opcode, &gb, false);
		}

		if (mooneye_passed(&gb)) {
			fprintf(stderr, "%s%s - test passed\n", MOONEYE_PATH, tests[i]);
			break;
		}

		if (mooneye_failed(&gb)) {
			fprintf(stderr, "%s%s - test failed cleanly (0x42 everywhere)\n", MOONEYE_PATH, tests[i]);
			assert(0);
		} else {
			fprintf(stderr, "%s%s - test failed non-cleanly\n", MOONEYE_PATH, tests[i]);
			assert(0);
		}
	}
}
