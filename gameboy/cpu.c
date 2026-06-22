#include "../platform/desktop.h"
#include "gameboy.h"

/*
 * I think passing the staty and then the dest and val pointers isn't the cleanest
 * but the alternatives (that I thought of, e.g. just passing an enum and then
 * having a switch inside the functions) are even less clean.
 *
 * I pass the whole state everywhere because some functions also need the access
 * to memory (for reads and writes), so I think it's better to just do it everywhere
 * for the sake of a more uniform API.
 */

bool get_flag(const CPU *cpu, const int flag) {
	switch (flag) {
	case Z:
		return (cpu->regs[AF].low & 0x80);
	case N:
		return (cpu->regs[AF].low & 0x40);
	case H:
		return (cpu->regs[AF].low & 0x20);
	case C:
		return (cpu->regs[AF].low & 0x10);
	default:
		platform_error_log("check_flag() got an incorrect flag int (flags are 0-3 in the Flags enum)\n");
		return false;
	}
}

void set_flag(CPU *cpu, const int flag, const bool value) {
	switch (flag) {
	case Z:
		if (value)
			cpu->regs[AF].low |= 0x80;
		else
			cpu->regs[AF].low &= ~0x80;
		return;
	case N:
		if (value)
			cpu->regs[AF].low |= 0x40;
		else
			cpu->regs[AF].low &= ~0x40;
		return;
	case H:
		if (value)
			cpu->regs[AF].low |= 0x20;
		else
			cpu->regs[AF].low &= ~0x20;
		return;
	case C:
		if (value)
			cpu->regs[AF].low |= 0x10;
		else
			cpu->regs[AF].low &= ~0x10;
		return;
	default:
		platform_error_log("set_flag() got an incorrect flag int (flags are 0-3 in the Flags enum)\n");
		return;
	}
}

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

void ld_a16_A(Gameboy *gb, const uint16_t addr) {
	write8(&gb->memory, addr, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void inc_r16(Gameboy *gb, uint16_t *dest) {
	++(*dest);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void inc_r8(Gameboy *gb, uint8_t *dest) {
	((*dest)++ & 0x0F) == 0x0F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);

	set_flag(&gb->cpu, Z, !(*dest));
	set_flag(&gb->cpu, N, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void dec_r8(Gameboy *gb, uint8_t *dest) {
	// lower 4 bits only borrow if they're 0000
	((*dest)-- & 0x0F) == 0x00 ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);

	set_flag(&gb->cpu, Z, !(*dest));
	set_flag(&gb->cpu, N, true);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void ld_r8_n8(Gameboy *gb, uint8_t *dest, const uint8_t val) {
	*dest = val;

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}
