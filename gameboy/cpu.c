#include "../platform/platform.h"
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

static bool get_flag(const CPU *cpu, const int flag) {
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

static void set_flag(CPU *cpu, const int flag, const bool value) {
	switch (flag) {
	case Z:
		if (value) {
			cpu->regs[AF].low |= 0x80;
		} else {
			cpu->regs[AF].low &= ~0x80;
		}
		return;
	case N:
		if (value) {
			cpu->regs[AF].low |= 0x40;
		} else {
			cpu->regs[AF].low &= ~0x40;
		}
		return;
	case H:
		if (value) {
			cpu->regs[AF].low |= 0x20;
		} else {
			cpu->regs[AF].low &= ~0x20;
		}
		return;
	case C:
		if (value) {
			cpu->regs[AF].low |= 0x10;
		} else {
			cpu->regs[AF].low &= ~0x10;
		}
		return;
	default:
		platform_error_log("set_flag() got an incorrect flag int (flags are 0-3 in the Flags enum)\n");
		return;
	}
}

/*
 * Every cycle count below is in M-cycles, and an instruction's cycles are not added up by hand:
 * - Each cpu_read8()/cpu_write8() costs 1, including the opcode and operand fetches, which
 *   happen in gameboy_step() and opcode_execute() before the instruction's function is called.
 * - Whatever is left is an internal cycle (the CPU is busy, but the bus is idle) - that's cpu_tick().
 *
 * For example, inc_r8 is 1 (fetch), inc_aHL is 3 (fetch, read, write) and inc_r16 is 2 (fetch, internal).
 */

/*
 * PUSH, CALL and RST all do this: an internal cycle (SP decrement), then the high byte, then the low byte.
 * cpu_write16() can't be used here, because it writes the low byte first.
 */
static void stack_push16(Gameboy *gb, const u16 val) {
	cpu_tick(gb);
	--gb->cpu.regs[SP].full;
	cpu_write8(gb, gb->cpu.regs[SP].full, val >> 8);
	--gb->cpu.regs[SP].full;
	cpu_write8(gb, gb->cpu.regs[SP].full, val & 0xFF);
}

static u16 stack_pop16(Gameboy *gb) {
	u8 low = cpu_read8(gb, gb->cpu.regs[SP].full);
	++gb->cpu.regs[SP].full;
	u8 high = cpu_read8(gb, gb->cpu.regs[SP].full);
	++gb->cpu.regs[SP].full;

	return low | (high << 8);
}

static void ld_r8_r8(Gameboy *gb, u8 *dest, const u8 src) {
	*dest = src;

	gb->cpu.regs[PC].full += 1;
}

static void ld_r8_n8(Gameboy *gb, u8 *dest, const u8 val) {
	*dest = val;

	gb->cpu.regs[PC].full += 2;
}

static void ld_r16_n16(Gameboy *gb, u16 *dest, const u16 val) {
	*dest = val;

	gb->cpu.regs[PC].full += 3;
}

static void ld_aHL_r8(Gameboy *gb, const u8 src) {
	cpu_write8(gb, gb->cpu.regs[HL].full, src);

	gb->cpu.regs[PC].full += 1;
}

static void ld_aHL_n8(Gameboy *gb, const u8 val) {
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	gb->cpu.regs[PC].full += 2;
}

static void ld_r8_aHL(Gameboy *gb, u8 *dest) {
	*dest = cpu_read8(gb, gb->cpu.regs[HL].full);

	gb->cpu.regs[PC].full += 1;
}

static void ld_a16_A(Gameboy *gb, const u16 addr) {
	cpu_write8(gb, addr, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void ld_addr16_A(Gameboy *gb, const u16 addr) {
	cpu_write8(gb, addr, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 3;
}

static void ldh_addr16_A(Gameboy *gb, const u16 addr) {
	cpu_write8(gb, addr, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
}

static void ldh_aC_A(Gameboy *gb) {
	cpu_write8(gb, 0xFF00 + gb->cpu.regs[BC].low, gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void ld_A_a16(Gameboy *gb, u16 addr) {
	gb->cpu.regs[AF].high = cpu_read8(gb, addr);

	gb->cpu.regs[PC].full += 1;
}

static void ld_A_addr16(Gameboy *gb, u16 addr) {
	gb->cpu.regs[AF].high = cpu_read8(gb, addr);

	gb->cpu.regs[PC].full += 3;
}

static void ldh_A_addr16(Gameboy *gb, const u16 addr) {
	gb->cpu.regs[AF].high = cpu_read8(gb, addr);

	gb->cpu.regs[PC].full += 2;
}

static void ldh_A_aC(Gameboy *gb) {
	gb->cpu.regs[AF].high = cpu_read8(gb, 0xFF00 + gb->cpu.regs[BC].low);

	gb->cpu.regs[PC].full += 1;
}

static void ld_aHLi_A(Gameboy *gb) {
	cpu_write8(gb, gb->cpu.regs[HL].full, gb->cpu.regs[AF].high);
	++gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
}

static void ld_aHLd_A(Gameboy *gb) {
	cpu_write8(gb, gb->cpu.regs[HL].full, gb->cpu.regs[AF].high);
	--gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
}

static void ld_A_aHLi(Gameboy *gb) {
	gb->cpu.regs[AF].high = cpu_read8(gb, gb->cpu.regs[HL].full);
	++gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
}

static void ld_A_aHLd(Gameboy *gb) {
	gb->cpu.regs[AF].high = cpu_read8(gb, gb->cpu.regs[HL].full);
	--gb->cpu.regs[HL].full;

	gb->cpu.regs[PC].full += 1;
}

// ================ 8-BIT ARITHMETIC ================

static void add_A_r8(Gameboy *gb, const u8 src) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) + (src & 0x0F) > 0x0F);
	set_flag(&gb->cpu, C, (gb->cpu.regs[AF].high + src) > 0xFF);

	gb->cpu.regs[AF].high += src;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void add_A_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) + (val & 0x0F) > 0x0F);
	set_flag(&gb->cpu, C, (gb->cpu.regs[AF].high + val) > 0xFF);

	gb->cpu.regs[AF].high += val;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void add_A_n8(Gameboy *gb, const u8 val) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) + (val & 0x0F) > 0x0F);
	set_flag(&gb->cpu, C, (gb->cpu.regs[AF].high + val) > 0xFF);

	gb->cpu.regs[AF].high += val;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
}

static void adc_A_r8(Gameboy *gb, const u8 src) {
	set_flag(&gb->cpu, N, false);
	bool c = get_flag(&gb->cpu, C);

	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) + (src & 0x0F) + c > 0x0F);
	set_flag(&gb->cpu, C, (u16)gb->cpu.regs[AF].high + src + c > 0x00FF);

	gb->cpu.regs[AF].high += src + c;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void adc_A_aHL(Gameboy *gb) {
	set_flag(&gb->cpu, N, false);
	bool c = get_flag(&gb->cpu, C);
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) + (val & 0x0F) + c > 0x0F);
	set_flag(&gb->cpu, C, (u16)gb->cpu.regs[AF].high + val + c > 0x00FF);

	gb->cpu.regs[AF].high += val + c;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void adc_A_n8(Gameboy *gb, const u8 val) {
	set_flag(&gb->cpu, N, false);
	bool c = get_flag(&gb->cpu, C);

	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) + (val & 0x0F) + c > 0x0F);
	set_flag(&gb->cpu, C, (u16)gb->cpu.regs[AF].high + val + c > 0x00FF);

	gb->cpu.regs[AF].high += val + c;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
}

static void sub_A_r8(Gameboy *gb, const u8 src) {
	set_flag(&gb->cpu, N, true);

	set_flag(&gb->cpu, H, (src & 0x0F) > (gb->cpu.regs[AF].high & 0x0F));
	set_flag(&gb->cpu, C, src > gb->cpu.regs[AF].high);

	gb->cpu.regs[AF].high -= src;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void sub_A_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, N, true);
	set_flag(&gb->cpu, H, (val & 0x0F) > (gb->cpu.regs[AF].high & 0x0F));
	set_flag(&gb->cpu, C, val > gb->cpu.regs[AF].high);

	gb->cpu.regs[AF].high -= val;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void sub_A_n8(Gameboy *gb, const u8 val) {
	set_flag(&gb->cpu, N, true);
	set_flag(&gb->cpu, H, (val & 0x0F) > (gb->cpu.regs[AF].high & 0x0F));
	set_flag(&gb->cpu, C, val > gb->cpu.regs[AF].high);

	gb->cpu.regs[AF].high -= val;
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
}

static void sbc_A_r8(Gameboy *gb, const u8 src) {
	set_flag(&gb->cpu, N, true);
	bool c = get_flag(&gb->cpu, C);

	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) < ((src & 0x0F) + c));
	set_flag(&gb->cpu, C, (src + c) > gb->cpu.regs[AF].high);

	gb->cpu.regs[AF].high -= (src + c);
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void sbc_A_aHL(Gameboy *gb) {
	set_flag(&gb->cpu, N, true);
	bool c = get_flag(&gb->cpu, C);
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, H, (gb->cpu.regs[AF].high & 0x0F) < ((val & 0x0F) + c));
	set_flag(&gb->cpu, C, val + c > gb->cpu.regs[AF].high);

	gb->cpu.regs[AF].high -= (val + c);
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void sbc_A_n8(Gameboy *gb, const u8 val) {
	set_flag(&gb->cpu, N, true);
	bool c = get_flag(&gb->cpu, C);

	set_flag(&gb->cpu, H, ((val + c) & 0x0F) > (gb->cpu.regs[AF].high & 0x0F));
	set_flag(&gb->cpu, C, val + c > gb->cpu.regs[AF].high);

	gb->cpu.regs[AF].high -= (val + c);
	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
}

static void dec_r8(Gameboy *gb, u8 *dest) {
	// lower 4 bits only borrow if they're 0000
	set_flag(&gb->cpu, H, ((*dest)-- & 0x0F) == 0x00);

	set_flag(&gb->cpu, Z, !(*dest));
	set_flag(&gb->cpu, N, true);

	gb->cpu.regs[PC].full += 1;
}

static void dec_aHL(Gameboy *gb) {
	u8 result = cpu_read8(gb, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, H, (result-- & 0x0F) == 0x00);
	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, true);

	cpu_write8(gb, gb->cpu.regs[HL].full, result);

	gb->cpu.regs[PC].full += 1;
}

static void inc_r8(Gameboy *gb, u8 *dest) {
	set_flag(&gb->cpu, H, ((*dest)++ & 0x0F) == 0x0F);

	set_flag(&gb->cpu, Z, !(*dest));
	set_flag(&gb->cpu, N, false);

	gb->cpu.regs[PC].full += 1;
}

static void inc_aHL(Gameboy *gb) {
	u8 result = cpu_read8(gb, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, H, (result++ & 0x0F) == 0x0F);
	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, false);

	cpu_write8(gb, gb->cpu.regs[HL].full, result);

	gb->cpu.regs[PC].full += 1;
}

static void cp_A_r8(Gameboy *gb, const u8 src) {
	u8 result = gb->cpu.regs[AF].high - src;

	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, true);
	set_flag(&gb->cpu, H, (src & 0x0F) > (gb->cpu.regs[AF].high & 0x0F));
	set_flag(&gb->cpu, C, src > gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void cp_A_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	u8 result = gb->cpu.regs[AF].high - val;

	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, true);
	set_flag(&gb->cpu, H, (val & 0x0F) > (gb->cpu.regs[AF].high & 0x0F));
	set_flag(&gb->cpu, C, val > gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 1;
}

static void cp_A_n8(Gameboy *gb, const u8 val) {
	u8 result = gb->cpu.regs[AF].high - val;

	set_flag(&gb->cpu, Z, !result);
	set_flag(&gb->cpu, N, true);
	set_flag(&gb->cpu, H, (val & 0x0F) > (gb->cpu.regs[AF].high & 0x0F));
	set_flag(&gb->cpu, C, val > gb->cpu.regs[AF].high);

	gb->cpu.regs[PC].full += 2;
}

// ================ 16-BIT ARITHMETIC ================

static void add_HL_r16(Gameboy *gb, const u16 src) {
	set_flag(&gb->cpu, N, false);

	set_flag(&gb->cpu, C, gb->cpu.regs[HL].full + src > 0xFFFF);

	bool half_carry = (gb->cpu.regs[HL].full & 0x0FFF) + (src & 0x0FFF) > 0x0FFF;
	set_flag(&gb->cpu, H, half_carry);

	gb->cpu.regs[HL].full += src;

	cpu_tick(gb); // Internal: the 16-bit add takes a second ALU pass

	gb->cpu.regs[PC].full += 1;
}

static void dec_r16(Gameboy *gb, u16 *dest) {
	--(*dest);

	cpu_tick(gb); // Internal: 16-bit decrement

	gb->cpu.regs[PC].full += 1;
}

static void inc_r16(Gameboy *gb, u16 *dest) {
	++(*dest);

	cpu_tick(gb); // Internal: 16-bit increment

	gb->cpu.regs[PC].full += 1;
}

// ================ BITWISE LOGIC ================

static void and_A_r8(Gameboy *gb, const u8 src) {
	gb->cpu.regs[AF].high &= src;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
}

static void and_A_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	gb->cpu.regs[AF].high &= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
}

static void and_A_n8(Gameboy *gb, const u8 val) {
	gb->cpu.regs[AF].high &= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
}

static void or_A_r8(Gameboy *gb, const u8 src) {
	gb->cpu.regs[AF].high |= src;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
}

static void or_A_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	gb->cpu.regs[AF].high |= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
}

static void or_A_n8(Gameboy *gb, const u8 val) {
	gb->cpu.regs[AF].high |= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
}

static void cpl(Gameboy *gb) {
	gb->cpu.regs[AF].high = ~gb->cpu.regs[AF].high;

	set_flag(&gb->cpu, N, true);
	set_flag(&gb->cpu, H, true);

	gb->cpu.regs[PC].full += 1;
}

static void xor_A_r8(Gameboy *gb, const u8 src) {
	gb->cpu.regs[AF].high ^= src;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
}

static void xor_A_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	gb->cpu.regs[AF].high ^= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
}

static void xor_A_n8(Gameboy *gb, const u8 val) {
	gb->cpu.regs[AF].high ^= val;

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 2;
}

// ================ BIT FLAGS ================

static void bit_u3_r8(Gameboy *gb, const int bit_num, const u8 src) {
	set_flag(&gb->cpu, Z, !(src & (1 << bit_num)));

	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);

	gb->cpu.regs[PC].full += 1;
}

static void bit_u3_aHL(Gameboy *gb, const int bit_num) {
	set_flag(&gb->cpu, Z, !(cpu_read8(gb, gb->cpu.regs[HL].full) & (1 << bit_num)));

	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, true);

	gb->cpu.regs[PC].full += 1;
}

static void res_u3_r8(Gameboy *gb, const int bit_num, u8 *src) {
	*src &= ~(1 << bit_num);

	gb->cpu.regs[PC].full += 1;
}

static void res_u3_aHL(Gameboy *gb, const int bit_num) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	val &= ~(1 << bit_num);
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	gb->cpu.regs[PC].full += 1;
}

static void set_u3_r8(Gameboy *gb, const int bit_num, u8 *src) {
	*src |= (1 << bit_num);

	gb->cpu.regs[PC].full += 1;
}

static void set_u3_aHL(Gameboy *gb, const int bit_num) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	val |= (1 << bit_num);
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	gb->cpu.regs[PC].full += 1;
}

// ================ BIT SHIFTS ================

static void rla(Gameboy *gb) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	bool old_carry = get_flag(&gb->cpu, C);
	bool new_carry = (gb->cpu.regs[AF].high & 0x80) == 0x80;
	set_flag(&gb->cpu, C, new_carry);

	gb->cpu.regs[AF].high <<= 1;

	if (old_carry) {
		gb->cpu.regs[AF].high |= 1;
	} else {
		gb->cpu.regs[AF].high &= ~1;
	}

	gb->cpu.regs[PC].full += 1;
}

static void rlca(Gameboy *gb) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	bool carry = (gb->cpu.regs[AF].high & 0x80) == 0x80;
	set_flag(&gb->cpu, C, carry);

	gb->cpu.regs[AF].high <<= 1;

	if (carry) {
		gb->cpu.regs[AF].high |= 1;
	} else {
		gb->cpu.regs[AF].high &= ~1;
	}

	gb->cpu.regs[PC].full += 1;
}

static void rra(Gameboy *gb) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	bool old_carry = get_flag(&gb->cpu, C);
	bool new_carry = (gb->cpu.regs[AF].high & 0x01) == 0x01;
	set_flag(&gb->cpu, C, new_carry);

	gb->cpu.regs[AF].high >>= 1;

	if (old_carry) {
		gb->cpu.regs[AF].high |= (1 << 7);
	} else {
		gb->cpu.regs[AF].high &= ~(1 << 7);
	}

	gb->cpu.regs[PC].full += 1;
}

static void rrca(Gameboy *gb) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	bool carry = (gb->cpu.regs[AF].high & 0x01) == 0x01;
	set_flag(&gb->cpu, C, carry);

	gb->cpu.regs[AF].high >>= 1;
	if (carry) {
		gb->cpu.regs[AF].high |= (1 << 7);
	} else {
		gb->cpu.regs[AF].high &= ~(1 << 7);
	}

	gb->cpu.regs[PC].full += 1;
}

static void rlc_r8(Gameboy *gb, u8 *src) {
	bool carry = *src & 0x80;
	set_flag(&gb->cpu, C, carry);
	*src <<= 1;
	*src |= carry;
	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void rlc_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	bool carry = val & 0x80;
	set_flag(&gb->cpu, C, carry);
	val <<= 1;
	val |= carry;
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void rrc_r8(Gameboy *gb, u8 *src) {
	bool carry = *src & 0x01;
	set_flag(&gb->cpu, C, carry);
	*src >>= 1;
	*src |= (carry << 7);
	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void rrc_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	bool carry = val & 0x01;
	set_flag(&gb->cpu, C, carry);
	val >>= 1;
	val |= (carry << 7);
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void rl_r8(Gameboy *gb, u8 *src) {
	bool carry = *src & 0x80;
	*src <<= 1;
	*src |= get_flag(&gb->cpu, C);
	set_flag(&gb->cpu, C, carry);

	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void rl_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	bool carry = val & 0x80;
	val <<= 1;
	val |= get_flag(&gb->cpu, C);
	set_flag(&gb->cpu, C, carry);
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void rr_r8(Gameboy *gb, u8 *src) {
	bool carry = *src & 0x01;
	*src >>= 1;
	*src |= (get_flag(&gb->cpu, C) << 7);
	set_flag(&gb->cpu, C, carry);

	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void rr_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	bool carry = val & 0x01;
	val >>= 1;
	val |= (get_flag(&gb->cpu, C) << 7);
	set_flag(&gb->cpu, C, carry);
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void sla_r8(Gameboy *gb, u8 *src) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, *src & 0x80);
	*src <<= 1;
	set_flag(&gb->cpu, Z, !(*src));

	gb->cpu.regs[PC].full += 1;
}

static void sla_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, val & 0x80);
	val <<= 1;
	cpu_write8(gb, gb->cpu.regs[HL].full, val);
	set_flag(&gb->cpu, Z, !val);

	gb->cpu.regs[PC].full += 1;
}

static void sra_r8(Gameboy *gb, u8 *src) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, *src & 0x01);
	bool sign_bit = *src & 0x80;
	*src >>= 1;
	set_flag(&gb->cpu, Z, !(*src));
	*src |= (sign_bit << 7);

	gb->cpu.regs[PC].full += 1;
}

static void sra_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, val & 0x01);
	bool sign_bit = val & 0x80;
	val >>= 1;
	set_flag(&gb->cpu, Z, !val);
	val |= (sign_bit << 7);
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	gb->cpu.regs[PC].full += 1;
}

static void swap_r8(Gameboy *gb, u8 *src) {
	u8 high = *src & 0xF0;
	u8 low = *src & 0x0F;
	*src = (low << 4) | (high >> 4);

	set_flag(&gb->cpu, Z, !(*src));
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
}

static void swap_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);
	u8 high = val & 0xF0;
	u8 low = val & 0x0F;
	val = (low << 4) | (high >> 4);
	cpu_write8(gb, gb->cpu.regs[HL].full, val);

	set_flag(&gb->cpu, Z, !val);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, false);

	gb->cpu.regs[PC].full += 1;
}

static void srl_r8(Gameboy *gb, u8 *src) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, *src & 0x01);
	*src >>= 1;
	set_flag(&gb->cpu, Z, !(*src));

	gb->cpu.regs[PC].full += 1;
}

static void srl_aHL(Gameboy *gb) {
	u8 val = cpu_read8(gb, gb->cpu.regs[HL].full);

	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, val & 0x01);
	val >>= 1;
	cpu_write8(gb, gb->cpu.regs[HL].full, val);
	set_flag(&gb->cpu, Z, !(val));

	gb->cpu.regs[PC].full += 1;
}

// ================ JUMPS ================

static void call_n16(Gameboy *gb, const u16 addr) {
	stack_push16(gb, gb->cpu.regs[PC].full + 3);

	gb->cpu.regs[PC].full = addr;
}

static void call_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const u16 addr) {
	if (get_flag(&gb->cpu, flag) == flag_state) {
		stack_push16(gb, gb->cpu.regs[PC].full + 3);
		gb->cpu.regs[PC].full = addr;
	} else {
		gb->cpu.regs[PC].full += 3;
	}
}

static void jp_n16(Gameboy *gb, const u16 addr) {
	gb->cpu.regs[PC].full = addr;

	cpu_tick(gb); // Internal: load the new PC
}

static void jp_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const u16 addr) {
	if (get_flag(&gb->cpu, flag) == flag_state) {
		gb->cpu.regs[PC].full = addr;
		cpu_tick(gb); // Internal: load the new PC, only when the jump is taken
	} else {
		gb->cpu.regs[PC].full += 3;
	}
}

static void jp_aHL(Gameboy *gb) {
	gb->cpu.regs[PC].full = gb->cpu.regs[HL].full;
}

/*
 * Naming might be weird but that's how it's named in rgbds, so I went
 * with that for consistency (it's easier to search in the docs this way).
 * It's n16 because it jumps to address n16, but it uses an 8-bit offset instead.
 */
static void jr_n16(Gameboy *gb, const i8 offset) {
	gb->cpu.regs[PC].full += 2;
	gb->cpu.regs[PC].full += offset;
	cpu_tick(gb); // Internal: add the offset to PC
}

/*
 * Naming might be weird but that's how it's named in rgbds, so I went
 * with that for consistency (it's easier to search in the docs this way).
 * It's n16 because it jumps to address n16, but it uses an 8-bit offset instead.
 */
static void jr_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const i8 offset) {
	gb->cpu.regs[PC].full += 2;
	if (get_flag(&gb->cpu, flag) == flag_state) {
		gb->cpu.regs[PC].full += offset;
		cpu_tick(gb); // Internal: add the offset to PC, only when the jump is taken
	}
}

static void rst(Gameboy *gb, const u8 vec) {
	stack_push16(gb, gb->cpu.regs[PC].full + 1);

	gb->cpu.regs[PC].full = vec;
}

static void ret(Gameboy *gb) {
	gb->cpu.regs[PC].full = stack_pop16(gb);

	cpu_tick(gb); // Internal: load the new PC
}

static void ret_cc(Gameboy *gb, const int flag, const bool flag_state) {
	cpu_tick(gb); // Internal: check the condition, this happens even when the return isn't taken

	if (get_flag(&gb->cpu, flag) == flag_state) {
		gb->cpu.regs[PC].full = stack_pop16(gb);
		cpu_tick(gb); // Internal: load the new PC
	} else {
		gb->cpu.regs[PC].full += 1;
	}
}

static void reti(Gameboy *gb) {
	gb->cpu.ime = true;

	gb->cpu.regs[PC].full = stack_pop16(gb);

	cpu_tick(gb); // Internal: load the new PC
}

// ================ CARRY FLAG INSTRUCTIONS ================

static void ccf(Gameboy *gb) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, !get_flag(&gb->cpu, C));

	gb->cpu.regs[PC].full += 1;
}

static void scf(Gameboy *gb) {
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, false);
	set_flag(&gb->cpu, C, true);

	gb->cpu.regs[PC].full += 1;
}

// ================ STACK INSTRUCTIONS ================

static void pop_r16(Gameboy *gb, u16 *src) {
	*src = stack_pop16(gb);

	gb->cpu.regs[PC].full += 1;
}

static void pop_AF(Gameboy *gb) {
	gb->cpu.regs[AF].full = stack_pop16(gb);
	gb->cpu.regs[AF].low &= 0xF0;

	gb->cpu.regs[PC].full += 1;
}

static void push_r16(Gameboy *gb, const u16 src) {
	stack_push16(gb, src);

	gb->cpu.regs[PC].full += 1;
}

static void push_AF(Gameboy *gb) {
	stack_push16(gb, gb->cpu.regs[AF].full & 0xFFF0);

	gb->cpu.regs[PC].full += 1;
}

static void ld_addr16_SP(Gameboy *gb, const u16 addr) {
	cpu_write16(gb, addr, gb->cpu.regs[SP].full);

	gb->cpu.regs[PC].full += 3;
}

static void ld_HL_SPe8(Gameboy *gb, const i8 val) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, (gb->cpu.regs[SP].full & 0x000F) + (val & 0x0F) > 0x000F);
	set_flag(&gb->cpu, C, (gb->cpu.regs[SP].full & 0x00FF) + (val & 0xFF) > 0x00FF);

	gb->cpu.regs[HL].full = gb->cpu.regs[SP].full + val;

	cpu_tick(gb); // Internal: add the offset to SP

	gb->cpu.regs[PC].full += 2;
}

static void ld_SP_HL(Gameboy *gb) {
	gb->cpu.regs[SP].full = gb->cpu.regs[HL].full;

	cpu_tick(gb); // Internal: 16-bit register copy

	gb->cpu.regs[PC].full += 1;
}

static void add_SP_n8(Gameboy *gb, const i8 val) {
	set_flag(&gb->cpu, Z, false);
	set_flag(&gb->cpu, N, false);
	set_flag(&gb->cpu, H, (gb->cpu.regs[SP].full & 0x000F) + (val & 0x0F) > 0x000F);
	set_flag(&gb->cpu, C, (gb->cpu.regs[SP].full & 0x00FF) + (val & 0xFF) > 0x00FF);

	gb->cpu.regs[SP].full += val;

	cpu_tick(gb); // Internal: add the offset to the low byte of SP
	cpu_tick(gb); // Internal: carry into the high byte of SP

	gb->cpu.regs[PC].full += 2;
}

// ================ INTERRUPTS ================

static void di(Gameboy *gb) {
	gb->cpu.ime = false;
	gb->cpu.ime_pending = false;

	gb->cpu.regs[PC].full += 1;
}

static void ei(Gameboy *gb) {
	gb->cpu.ime_pending = true;

	gb->cpu.regs[PC].full += 1;
}

static void halt(Gameboy *gb) {
	// See interrupt_handle() for explanation on the ANDs
	bool pending = gb->memory.ie & gb->memory.io_registers[IF_ADDR - IO_REGS_ADDR] & 0x1F;

	/*
	 * !IME && pending: The CPU continues execution after the HALT, but the byte after it is
	 * read twice in a row (PC is not incremented, due to a hardware bug).
	 */
	if (!gb->cpu.ime && pending) {
		gb->cpu.halt_bug = true;
	} else {
		gb->cpu.halted = true;
	}

	gb->cpu.regs[PC].full += 1;
}

// ================ MISC ================

static void daa(Gameboy *gb) {
	bool
	    sub = get_flag(&gb->cpu, N),
	    hcarry = get_flag(&gb->cpu, H),
	    carry = get_flag(&gb->cpu, C);

	u8 adj = 0;
	if (sub) {
		if (hcarry) {
			adj += 0x06;
		}
		if (carry) {
			adj += 0x60;
		}
		gb->cpu.regs[AF].high -= adj;
	} else {
		if (hcarry || (gb->cpu.regs[AF].high & 0x0F) > 0x09) {
			adj += 0x06;
		}
		if (carry || gb->cpu.regs[AF].high > 0x99) {
			adj += 0x60;
			set_flag(&gb->cpu, C, true);
		}
		gb->cpu.regs[AF].high += adj;
	}

	set_flag(&gb->cpu, Z, !gb->cpu.regs[AF].high);
	set_flag(&gb->cpu, H, false);

	gb->cpu.regs[PC].full += 1;
}

static void nop(Gameboy *gb) {
	gb->cpu.regs[PC].full += 1;
}

static void stop_n8(Gameboy *gb, const u8 val) {
	platform_error_log("stop() is a stub - TODO implementation\n");
	/*
	 * TODO: "(DIV register): Additionally, this register is reset when executing the stop instruction,
	 * and only begins ticking again once stop mode ends."
	 */
	gb->cpu.regs[PC].full += 2;
}
