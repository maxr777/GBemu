#include "gameboy.h"

void nop(Gameboy *gb) {
	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}
