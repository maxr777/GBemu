#include "gameboy.h"

static void timer_tick(Gameboy *gb) {
	// This handles DIV increment, DIV is just bits 13..6 of timer (timer counts M-cycles, see read8)
	++gb->timer;

	u8 *tima = &gb->memory.io_registers[TIMA_ADDR - IO_REGS_ADDR];
	u8 *tma = &gb->memory.io_registers[TMA_ADDR - IO_REGS_ADDR];
	u8 *tac = &gb->memory.io_registers[TAC_ADDR - IO_REGS_ADDR];

	// clang-format off
	/*
	 * TAC bits:
	 * 7:
	 * 6:
	 * 5:
	 * 4:
	 * 3:
	 * 2: enable
	 * 1: clock select
	 * 0: clock select
	 *
	 * Bit 2 controls whether TIMA gets incremented.
	 * Bits 1 and 0 set the rate at which TIMA is incremented, as follows in the switch below.
	 *
	 * It's basically a multiplexer on the timer. This is the hardware's counter, which runs on
	 * T-cycles. Mine runs on M-cycles, so its bit numbers are 2 lower (e.g. hardware bit 9 = my bit 7):
	 *
	 * 16-bit counter (+1 every T-cycle)        DIV = bits 15..8
	 *      │
	 *      ├─ bit 9 ─┐
	 *      ├─ bit 3 ─┤   TAC bits 1-0 pick one
	 *      ├─ bit 5 ─┼──► (multiplexer) ──┐
	 *      └─ bit 7 ─┘                    │
	 *                                    AND ──► falling-edge detector ──► TIMA++
	 *           TAC bit 2 (enable) ───────┘        (output goes 1 → 0)
	 *
	 * TAC 00 means that the falling-edge detector watches bit 9. 01 = bit 3, 10 = bit 5, 11 = bit 7.
	 */
	// clang-format on
	int tac_increment = 0;
	if (*tac & 0x04) {
		switch (*tac & 0x03) {
		case 0x00: {
			tac_increment = TAC_00_CYCLES;
			break;
		}
		case 0x01: {
			tac_increment = TAC_01_CYCLES;
			break;
		}
		case 0x02: {
			tac_increment = TAC_10_CYCLES;
			break;
		}
		case 0x03: {
			tac_increment = TAC_11_CYCLES;
			break;
		}
		}

		if (gb->timer % tac_increment == 0) {
			// If TIMA overflows, set it to the value in TMA and request an interrupt
			if (*tima == 0xFF) {
				*tima = *tma;
				// TODO: request an interrupt
			} else {
				++(*tima);
			}
		}
	}
}

static void cpu_tick(Gameboy *gb) {
	++gb->cpu.cycle;
	timer_tick(gb);
}
