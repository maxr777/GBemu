#include "gameboy.h"

void nop(Gameboy *gb) {
	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

// TODO: think about removing the dest argument
// maybe switch based on RegisterName?
// maybe inline the functions in the switch statement?
void ld_r16_n16(Gameboy *gb, u16 *dest, const u16 val) {
	*dest = val;

	gb->cpu.regs[PC].full += 3;
	gb->cpu.cycle += 3;
}
