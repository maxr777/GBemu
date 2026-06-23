#include "../platform/desktop.h"
#include "gameboy.h"

/*
 * I think passing the state and then the dest and val pointers isn't the cleanest
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

void ld_r8_r8(Gameboy *gb, u8 *dest, const u8 src) {
	*dest = src;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void ld_r8_n8(Gameboy *gb, u8 *dest, const u8 val) {
	*dest = val;

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void ld_r16_n16(Gameboy *gb, u16 *dest, const u16 val) {
	*dest = val;

	gb->cpu.regs[PC].full += 3;
	gb->cpu.cycle += 3;
}

void ld_aHL_r8(Gameboy *gb, const u8 src) {
	write8(&gb->memory, gb->cpu.regs[HL].full, src);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_aHL_n8(Gameboy *gb, const u8 val) {
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 3;
}

void ld_r8_aHL(Gameboy *gb, u8 *dest) {
	*dest = read8(&gb->memory, gb->cpu.regs[HL].full);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_a16_A(Gameboy *gb, const u16 addr) {
	write8(&gb->memory, addr, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_addr16_A(Gameboy *gb, const u16 addr) {
	write8(&gb->memory, addr, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 3;
	gb->cpu.cycle += 4;
}

void ldh_addr16_A(Gameboy *gb, const u16 addr) {
	write8(&gb->memory, addr, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 3;
}

void ldh_aC_A(Gameboy *gb) {
	write8(&gb->memory, 0xFF00 + gb->cpu.regs[BC].low, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_A_a16(Gameboy *gb, u16 addr) {
	gb->cpu.regs[AF].high = read8(&gb->memory, addr);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_A_addr16(Gameboy *gb, u16 addr) {
	gb->cpu.regs[AF].high = read8(&gb->memory, addr);

	gb->cpu.regs[PC].full += 3;
	gb->cpu.cycle += 4;
}

void ldh_A_addr16(Gameboy *gb, const u16 addr) {
	gb->cpu.regs[AF].high = read8(&gb->memory, addr);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 3;
}

void ldh_A_aC(Gameboy *gb) {
	gb->cpu.regs[AF].high = read8(&gb->memory, 0xFF00 + gb->cpu.regs[BC].low);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_aHLi_A(Gameboy *gb) {
	write8(&gb->memory, gb->cpu.regs[HL].full, gb->cpu.regs[AF].high);
	++gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_aHLd_A(Gameboy *gb) {
	write8(&gb->memory, gb->cpu.regs[HL].full, gb->cpu.regs[AF].high);
	--gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_A_aHLi(Gameboy *gb) {
	gb->cpu.regs[AF].high = read8(&gb->memory, gb->cpu.regs[HL].full);
	++gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void ld_A_aHLd(Gameboy *gb) {
	gb->cpu.regs[AF].high = read8(&gb->memory, gb->cpu.regs[HL].full);
	--gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

// ================ 8-BIT ARITHMETIC ================

void add_A_r8(Gameboy *gb, const u8 src) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) + (src & 0x0F) > 0x0F);
	set_flag(&gb->cpu, C, (gb->cpu.regs[AF].high + src) > 0xFF);

	gb->cpu.regs[AF].high += src;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void add_A_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) + (val & 0x0F) > 0x0F);
	set_flag(&gb->cpu, C, (gb->cpu.regs[AF].high + val) > 0xFF);

	gb->cpu.regs[AF].high += val;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void add_A_n8(Gameboy *gb, const u8 val) {
	set_flag(&gb->cpu, N, false);
	(gb->cpu.regs[AF].high & 0x0F) + (val & 0x0F) > 0x0F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	(gb->cpu.regs[AF].high + val) > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high += val;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void adc_A_r8(Gameboy *gb, const u8 src) {
	set_flag(&gb->cpu, N, false);
	bool c = get_flag(&gb->cpu, C);

	(gb->cpu.regs[AF].high & 0x0F) + (src & 0x0F) + c > 0x0F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	(u16) gb->cpu.regs[AF].high + src + c > 0x00FF ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high += src + c;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void adc_A_aHL(Gameboy *gb) {
	set_flag(&gb->cpu, N, false);
	bool c = get_flag(&gb->cpu, C);
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);

	(gb->cpu.regs[AF].high & 0x0F) + (val & 0x0F) + c > 0x0F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	(u16) gb->cpu.regs[AF].high + val + c > 0x00FF ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high += val + c;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void adc_A_n8(Gameboy *gb, const u8 val) {
	set_flag(&gb->cpu, N, false);
	bool c = get_flag(&gb->cpu, C);

	(gb->cpu.regs[AF].high & 0x0F) + (val & 0x0F) + c > 0x0F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	(u16) gb->cpu.regs[AF].high + val + c > 0x00FF ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high += val + c;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void sub_A_r8(Gameboy *gb, const u8 src) {
	set_flag(&gb->cpu, N, true);

	(src & 0x0F) > (gb->cpu.regs[AF].high & 0x0F) ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	src > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high -= src;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void sub_A_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, N, true);
	(val & 0x0F) > (gb->cpu.regs[AF].high & 0x0F) ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	val > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high -= val;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void sub_A_n8(Gameboy *gb, const u8 val) {
	set_flag(&gb->cpu, N, true);
	(val & 0x0F) > (gb->cpu.regs[AF].high & 0x0F) ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	val > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high -= val;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void sbc_A_r8(Gameboy *gb, const u8 src) {
	set_flag(&gb->cpu, N, true);
	bool c = get_flag(&gb->cpu, C);

	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) < ((src & 0x0F) + c));
	set_flag(&gb->cpu, C, (src + c) > gb->cpu.regs[AF].high);

	gb->cpu.regs[AF].high -= (src + c);
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void sbc_A_aHL(Gameboy *gb) {
	set_flag(&gb->cpu, N, true);
	bool c = get_flag(&gb->cpu, C);
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);

	((val + c) & 0x0F) > (gb->cpu.regs[AF].high & 0x0F) ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	val + c > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high -= (val + c);
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void sbc_A_n8(Gameboy *gb, const u8 val) {
	set_flag(&gb->cpu, N, true);
	bool c = get_flag(&gb->cpu, C);

	((val + c) & 0x0F) > (gb->cpu.regs[AF].high & 0x0F) ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	val + c > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[AF].high -= (val + c);
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void dec_r8(Gameboy *gb, u8 *dest) {
	// lower 4 bits only borrow if they're 0000
	((*dest)-- & 0x0F) == 0x00 ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);

	set_flag(&gb->cpu, Z, !(*dest));
	set_flag(&gb->cpu, N, true);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void dec_aHL(Gameboy *gb) {
	u8 result = read8(&gb->memory, gb->cpu.regs[HL].full);

	(result-- & 0x0F) == 0x00 ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, true);

	write8(&gb->memory, gb->cpu.regs[HL].full, result);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 3;
}

void inc_r8(Gameboy *gb, u8 *dest) {
	((*dest)++ & 0x0F) == 0x0F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);

	set_flag(&gb->cpu, Z, !(*dest));
	set_flag(&gb->cpu, N, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void inc_aHL(Gameboy *gb) {
	u8 result = read8(&gb->memory, gb->cpu.regs[HL].full);

	(result++ & 0x0F) == 0x0F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, false);

	write8(&gb->memory, gb->cpu.regs[HL].full, result);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 3;
}

void cp_A_r8(Gameboy *gb, const u8 src) {
	u8 result = gb->cpu.regs[AF].high - src;

	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, true);
	(src & 0x0F) > (gb->cpu.regs[AF].high & 0x0F) ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	src > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void cp_A_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	u8 result = gb->cpu.regs[AF].high - val;

	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, true);
	(val & 0x0F) > (gb->cpu.regs[AF].high & 0x0F) ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	val > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void cp_A_n8(Gameboy *gb, const u8 val) {
	u8 result = gb->cpu.regs[AF].high - val;

	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, true);
	(val & 0x0F) > (gb->cpu.regs[AF].high & 0x0F) ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	val > gb->cpu.regs[AF].high ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

// ================ 16-BIT ARITHMETIC ================

void add_HL_r16(Gameboy *gb, const u16 src) {
	set_flag(&gb->cpu, N, false);

	set_flag(&gb->cpu, C, gb->cpu.regs[HL].full + src > 0xFFFF);

	bool half_carry = (gb->cpu.regs[HL].full & 0x0FFF) + (src & 0x0FFF) > 0x0FFF;
	set_flag(&gb->cpu, H, half_carry);

	gb->cpu.regs[HL].full += src;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void dec_r16(Gameboy *gb, u16 *dest) {
	--(*dest);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void inc_r16(Gameboy *gb, u16 *dest) {
	++(*dest);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

// ================ BITWISE LOGIC ================

void and_A_r8(Gameboy *gb, const u8 src) {
	gb->cpu.regs[AF].high &= src;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void and_A_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	gb->cpu.regs[AF].high &= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void and_A_n8(Gameboy *gb, const u8 val) {
	gb->cpu.regs[AF].high &= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void or_A_r8(Gameboy *gb, const u8 src) {
	gb->cpu.regs[AF].high |= src;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void or_A_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	gb->cpu.regs[AF].high |= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void or_A_n8(Gameboy *gb, const u8 val) {
	gb->cpu.regs[AF].high |= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void cpl(Gameboy *gb) {
	gb->cpu.regs[AF].high = ~gb->cpu.regs[AF].high;

	set_flag(&gb->cpu, N, true);
	set_flag(&gb->cpu, H, true);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void xor_A_r8(Gameboy *gb, const u8 src) {
	gb->cpu.regs[AF].high ^= src;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void xor_A_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	gb->cpu.regs[AF].high ^= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void xor_A_n8(Gameboy *gb, const u8 val) {
	gb->cpu.regs[AF].high ^= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

// ================ BIT FLAGS ================

void bit_u3_r8(Gameboy *gb, const int bit_num, const u8 src) {
	src & (1 << bit_num) ? set_flag(&gb->cpu, Z, false) : set_flag(&gb->cpu, Z, true);

	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void bit_u3_aHL(Gameboy *gb, const int bit_num) {
	read8(&gb->memory, gb->cpu.regs[HL].full) & (1 << bit_num) ? set_flag(&gb->cpu, Z, false) : set_flag(&gb->cpu, Z, true);

	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void res_u3_r8(Gameboy *gb, const int bit_num, u8 *src) {
	*src &= ~(1 << bit_num);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void res_u3_aHL(Gameboy *gb, const int bit_num) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	val &= ~(1 << bit_num);
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

void set_u3_r8(Gameboy *gb, const int bit_num, u8 *src) {
	*src |= (1 << bit_num);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void set_u3_aHL(Gameboy *gb, const int bit_num) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	val |= (1 << bit_num);
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

// ================ BIT SHIFTS ================

void rla(Gameboy *gb) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	bool old_carry = get_flag(&gb->cpu, C);
	bool new_carry = (gb->cpu.regs[AF].high & 0x80) == 0x80;
	set_flag(&gb->cpu, C, new_carry);

	gb->cpu.regs[AF].high <<= 1;

	if (old_carry)
		gb->cpu.regs[AF].high |= 1;
	else
		gb->cpu.regs[AF].high &= ~1;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void rlca(Gameboy *gb) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	bool carry = (gb->cpu.regs[AF].high & 0x80) == 0x80;
	set_flag(&gb->cpu, C, carry);

	gb->cpu.regs[AF].high <<= 1;

	if (carry)
		gb->cpu.regs[AF].high |= 1;
	else
		gb->cpu.regs[AF].high &= ~1;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void rra(Gameboy *gb) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	bool old_carry = get_flag(&gb->cpu, C);
	bool new_carry = (gb->cpu.regs[AF].high & 0x01) == 0x01;
	set_flag(&gb->cpu, C, new_carry);

	gb->cpu.regs[AF].high >>= 1;

	if (old_carry)
		gb->cpu.regs[AF].high |= (1 << 7);
	else
		gb->cpu.regs[AF].high &= ~(1 << 7);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void rrca(Gameboy *gb) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	bool carry = (gb->cpu.regs[AF].high & 0x01) == 0x01;
	set_flag(&gb->cpu, C, carry);

	gb->cpu.regs[AF].high >>= 1;
	if (carry)
		gb->cpu.regs[AF].high |= (1 << 7);
	else
		gb->cpu.regs[AF].high &= ~(1 << 7);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void rlc_r8(Gameboy *gb, u8 *src) {
	bool carry = *src & 0x80;
	set_flag(&gb->cpu, C, carry);
	*src <<= 1;
	*src |= carry;
	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void rlc_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	bool carry = val & 0x80;
	set_flag(&gb->cpu, C, carry);
	val <<= 1;
	val |= carry;
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

void rrc_r8(Gameboy *gb, u8 *src) {
	bool carry = *src & 0x01;
	set_flag(&gb->cpu, C, carry);
	*src >>= 1;
	*src |= (carry << 7);
	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void rrc_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	bool carry = val & 0x01;
	set_flag(&gb->cpu, C, carry);
	val >>= 1;
	val |= (carry << 7);
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

void rl_r8(Gameboy *gb, u8 *src) {
	bool carry = *src & 0x80;
	*src <<= 1;
	*src |= get_flag(&gb->cpu, C);
	set_flag(&gb->cpu, C, carry);

	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void rl_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	bool carry = val & 0x80;
	val <<= 1;
	val |= get_flag(&gb->cpu, C);
	set_flag(&gb->cpu, C, carry);
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

void rr_r8(Gameboy *gb, u8 *src) {
	bool carry = *src & 0x01;
	*src >>= 1;
	*src |= (get_flag(&gb->cpu, C) << 7);
	set_flag(&gb->cpu, C, carry);

	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void rr_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	bool carry = val & 0x01;
	val >>= 1;
	val |= (get_flag(&gb->cpu, C) << 7);
	set_flag(&gb->cpu, C, carry);
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

void sla_r8(Gameboy *gb, u8 *src) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, *src & 0x80);
	*src <<= 1;
	set_flag(&gb->cpu, Z, !(*src));

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void sla_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, val & 0x80);
	val <<= 1;
	write8(&gb->memory, gb->cpu.regs[HL].full, val);
	set_flag(&gb->cpu, Z, !val);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

void sra_r8(Gameboy *gb, u8 *src) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, *src & 0x01);
	bool sign_bit = *src & 0x80;
	*src >>= 1;
	set_flag(&gb->cpu, Z, !(*src));
	*src |= (sign_bit << 7);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void sra_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, val & 0x01);
	bool sign_bit = val & 0x80;
	val >>= 1;
	set_flag(&gb->cpu, Z, !val);
	val |= (sign_bit << 7);
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

void swap_r8(Gameboy *gb, u8 *src) {
	u8 high = *src & 0xF0;
	u8 low = *src & 0x0F;
	*src = (low << 4) | (high >> 4);

	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void swap_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);
	u8 high = val & 0xF0;
	u8 low = val & 0x0F;
	val = (low << 4) | (high >> 4);
	write8(&gb->memory, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

void srl_r8(Gameboy *gb, u8 *src) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, *src & 0x01);
	*src >>= 1;
	set_flag(&gb->cpu, Z, !(*src));

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 2;
}

void srl_aHL(Gameboy *gb) {
	u8 val = read8(&gb->memory, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, val & 0x01);
	val >>= 1;
	write8(&gb->memory, gb->cpu.regs[HL].full, val);
	set_flag(&gb->cpu, Z, !(val));

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

// ================ JUMPS ================

void call_n16(Gameboy *gb, const u16 addr) {
	gb->cpu.regs[SP].full -= 2;
	write16(&gb->memory, gb->cpu.regs[SP].full, gb->cpu.regs[PC].full + 3);

	gb->cpu.regs[PC].full = addr;

	gb->cpu.cycle += 6;
}

void call_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const u16 addr) {
	if (get_flag(&gb->cpu, flag) == flag_state) {
		gb->cpu.regs[SP].full -= 2;
		write16(&gb->memory, gb->cpu.regs[SP].full, gb->cpu.regs[PC].full + 3);
		gb->cpu.regs[PC].full = addr;
		gb->cpu.cycle += 6;
	} else {
		gb->cpu.regs[PC].full += 3;
		gb->cpu.cycle += 3;
	}
}

void jp_n16(Gameboy *gb, const u16 addr) {
	gb->cpu.regs[PC].full = addr;

	gb->cpu.cycle += 4;
}

void jp_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const u16 addr) {
	if (get_flag(&gb->cpu, flag) == flag_state) {
		gb->cpu.regs[PC].full = addr;
		gb->cpu.cycle += 4;
	} else {
		gb->cpu.regs[PC].full += 3;
		gb->cpu.cycle += 3;
	}
}

void jp_aHL(Gameboy *gb) {
	gb->cpu.regs[PC].full = gb->cpu.regs[HL].full;

	gb->cpu.cycle += 1;
}

// Naming might be weird but that's how it's named in rgbds, so I went
// with that for consistency (it's easier to search in the docs this way).
// It's n16 because it jumps to address n16, but it uses an 8-bit offset instead.
void jr_n16(Gameboy *gb, const i8 offset) {
	gb->cpu.regs[PC].full += 2;
	gb->cpu.regs[PC].full += offset;
	gb->cpu.cycle += 3;
}

// Naming might be weird but that's how it's named in rgbds, so I went
// with that for consistency (it's easier to search in the docs this way).
// It's n16 because it jumps to address n16, but it uses an 8-bit offset instead.
void jr_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const i8 offset) {
	gb->cpu.regs[PC].full += 2;
	if (get_flag(&gb->cpu, flag) == flag_state) {
		gb->cpu.regs[PC].full += offset;
		gb->cpu.cycle += 3;
	} else {
		gb->cpu.cycle += 2;
	}
}

void rst(Gameboy *gb, const u8 vec) {
	gb->cpu.regs[SP].full -= 2;
	write16(&gb->memory, gb->cpu.regs[SP].full, gb->cpu.regs[PC].full + 1);

	gb->cpu.regs[PC].full = vec;
	gb->cpu.cycle += 4;
}

void ret(Gameboy *gb) {
	gb->cpu.regs[PC].low = read8(&gb->memory, gb->cpu.regs[SP].full);
	++gb->cpu.regs[SP].full;
	gb->cpu.regs[PC].high = read8(&gb->memory, gb->cpu.regs[SP].full);
	++gb->cpu.regs[SP].full;

	gb->cpu.cycle += 4;
}

void ret_cc(Gameboy *gb, const int flag, const bool flag_state) {
	if (get_flag(&gb->cpu, flag) == flag_state) {
		gb->cpu.regs[PC].low = read8(&gb->memory, gb->cpu.regs[SP].full);
		++gb->cpu.regs[SP].full;
		gb->cpu.regs[PC].high = read8(&gb->memory, gb->cpu.regs[SP].full);
		++gb->cpu.regs[SP].full;
		gb->cpu.cycle += 5;
	} else {
		gb->cpu.regs[PC].full += 1;
		gb->cpu.cycle += 2;
	}
}

void reti(Gameboy *gb) {
	gb->cpu.ime = true;

	gb->cpu.regs[PC].low = read8(&gb->memory, gb->cpu.regs[SP].full);
	++gb->cpu.regs[SP].full;
	gb->cpu.regs[PC].high = read8(&gb->memory, gb->cpu.regs[SP].full);
	++gb->cpu.regs[SP].full;

	gb->cpu.cycle += 4;
}

// ================ CARRY FLAG INSTRUCTIONS ================

void ccf(Gameboy *gb) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	get_flag(&gb->cpu, C) ? set_flag(&gb->cpu, C, false) : set_flag(&gb->cpu, C, true);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void scf(Gameboy *gb) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, true);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

// ================ STACK INSTRUCTIONS ================

void pop_r16(Gameboy *gb, u16 *src) {
	*src = read16(&gb->memory, gb->cpu.regs[SP].full);
	gb->cpu.regs[SP].full += 2;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 3;
}

void pop_AF(Gameboy *gb) {
	gb->cpu.regs[AF].full = read16(&gb->memory, gb->cpu.regs[SP].full);
	gb->cpu.regs[AF].low &= 0xF0;
	gb->cpu.regs[SP].full += 2;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 3;
}

void push_r16(Gameboy *gb, const u16 src) {
	gb->cpu.regs[SP].full -= 2;
	write16(&gb->memory, gb->cpu.regs[SP].full, src);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 4;
}

void push_AF(Gameboy *gb) {
	gb->cpu.regs[SP].full -= 2;
	write16(&gb->memory, gb->cpu.regs[SP].full, gb->cpu.regs[AF].full & 0xFFF0);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 4;
}

void ld_addr16_SP(Gameboy *gb, const u16 addr) {
	write16(&gb->memory, addr, gb->cpu.regs[SP].full);

	gb->cpu.regs[PC].full += 3;
	gb->cpu.cycle += 5;
}

void ld_HL_SPe8(Gameboy *gb, const i8 val) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	(gb->cpu.regs[SP].full & 0x000F) + (val & 0x0F) > 0x000F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	(gb->cpu.regs[SP].full & 0x00FF) + (val & 0xFF) > 0x00FF ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[HL].full = gb->cpu.regs[SP].full + val;

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 3;
}

void ld_SP_HL(Gameboy *gb) {
	gb->cpu.regs[SP].full = gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 2;
}

void add_SP_n8(Gameboy *gb, const i8 val) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	(gb->cpu.regs[SP].full & 0x000F) + (val & 0x0F) > 0x000F ? set_flag(&gb->cpu, H, true) : set_flag(&gb->cpu, H, false);
	(gb->cpu.regs[SP].full & 0x00FF) + (val & 0xFF) > 0x00FF ? set_flag(&gb->cpu, C, true) : set_flag(&gb->cpu, C, false);

	gb->cpu.regs[SP].full += val;

	gb->cpu.regs[PC].full += 2;
	gb->cpu.cycle += 4;
}

// ================ INTERRUPTS ================

void di(Gameboy *gb) {
	gb->cpu.ime = false;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void ei(Gameboy *gb) {
	gb->cpu.ime_enable_counter = 1;

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void halt(Gameboy *gb) {
	platform_error_log("halt() is a stub - TODO implementation\n");

	gb->cpu.regs[PC].full += 1;
}

// ================ MISC ================

void daa(Gameboy *gb) {
	bool
	    sub = get_flag(&gb->cpu, N),
	    hcarry = get_flag(&gb->cpu, H),
	    carry = get_flag(&gb->cpu, C);

	u8 adj = 0;
	if (sub) {
		if (hcarry) adj += 0x06;
		if (carry) adj += 0x60;
		gb->cpu.regs[AF].high -= adj;
	} else {
		if (hcarry || (gb->cpu.regs[AF].high & 0x0F) > 0x09) adj += 0x06;
		if (carry || gb->cpu.regs[AF].high > 0x99) {
			adj += 0x60;
			set_flag(&gb->cpu, C, true);
		}
		gb->cpu.regs[AF].high += adj;
	}

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void nop(Gameboy *gb) {
	gb->cpu.regs[PC].full += 1;
	gb->cpu.cycle += 1;
}

void stop_n8(Gameboy *gb, const u8 val) {
	platform_error_log("stop() is a stub - TODO implementation\n");
	// TODO: Remember about the divider register (0xFF04)

	gb->cpu.regs[PC].full += 2;
}
