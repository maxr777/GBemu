#include "gameboy.h"

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
	 *
	 * The function below returns that signal (the bit we're watching and tac enable).
	 *
	 * It's used for the TIMA calculations (just looking at cycles wouldn't work, because if we write
	 * 0 to DIV then the cycles don't detect the change, but the bit we're looking for might've changed
	 * if it was set to 1 before that write). Use wherever the timer changes.
	 */
// clang-format on
static bool timer_signal(const Gameboy *gb) {
	const u8 bit[] = {7, 1, 3, 5};
	u8 tac = gb->memory.io_registers[TAC_ADDR - IO_REGS_ADDR];
	// Basically the diagram above: return tac enable AND that one bit from the timer we're watching
	return (tac & 0x04) && (gb->timer & (1u << bit[tac & 0x03]));
}

static void tima_update(Gameboy *gb, bool signal_old) {
	u8 *tima = &gb->memory.io_registers[TIMA_ADDR - IO_REGS_ADDR];

	// Only actual hardware timer reacts only on falling edges, so 1 -> 0
	if (signal_old && !timer_signal(gb)) {
		if (*tima == 0xFF) {
			// Actual overflow code happens in the tima_state switch
			gb->tima_state = TIMA_OVERFLOWED;
		}
		++(*tima);
	}
}

static void timer_tick(Gameboy *gb) {
	u8 *tima = &gb->memory.io_registers[TIMA_ADDR - IO_REGS_ADDR];
	u8 *tma = &gb->memory.io_registers[TMA_ADDR - IO_REGS_ADDR];
	TimaState *state = &gb->tima_state;

	/*
	 * On overflow, TIMA reads 0x00 and IF bit 2 is not set for one cycle
	 */
	switch (*state) {
	case TIMA_OVERFLOWED: {
		*state = TIMA_NORMAL;
		*tima = *tma;
		// Request an interrupt (0x04 == TIMER interrupt)
		gb->memory.io_registers[IF_ADDR - IO_REGS_ADDR] |= 0x04;
		break;
	}
	default:
		break;
	}

	bool old = timer_signal(gb);
	// This handles DIV increment, DIV is just bits 13..6 of timer (timer counts M-cycles, see read8)
	++gb->timer;

	tima_update(gb, old);
}

static void cpu_tick(Gameboy *gb) {
	++gb->cpu.cycle;
	timer_tick(gb);
}
