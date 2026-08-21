#include "gameboy.h"
#include "../misc/types.h"
#include "../platform/desktop.h"
#include <stdbool.h>

static u8 BOOT_ROM[] = {
    0x31, 0xfe, 0xff, 0x21, 0xff, 0x9f, 0xaf, 0x32, 0xcb, 0x7c, 0x20, 0xfa,
    0x0e, 0x11, 0x21, 0x26, 0xff, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3,
    0x32, 0xe2, 0x0c, 0x3e, 0x77, 0x32, 0xe2, 0x11, 0x04, 0x01, 0x21, 0x10,
    0x80, 0x1a, 0xcd, 0xb8, 0x00, 0x1a, 0xcb, 0x37, 0xcd, 0xb8, 0x00, 0x13,
    0x7b, 0xfe, 0x34, 0x20, 0xf0, 0x11, 0xcc, 0x00, 0x06, 0x08, 0x1a, 0x13,
    0x22, 0x23, 0x05, 0x20, 0xf9, 0x21, 0x04, 0x99, 0x01, 0x0c, 0x01, 0xcd,
    0xb1, 0x00, 0x3e, 0x19, 0x77, 0x21, 0x24, 0x99, 0x0e, 0x0c, 0xcd, 0xb1,
    0x00, 0x3e, 0x91, 0xe0, 0x40, 0x06, 0x10, 0x11, 0xd4, 0x00, 0x78, 0xe0,
    0x43, 0x05, 0x7b, 0xfe, 0xd8, 0x28, 0x04, 0x1a, 0xe0, 0x47, 0x13, 0x0e,
    0x1c, 0xcd, 0xa7, 0x00, 0xaf, 0x90, 0xe0, 0x43, 0x05, 0x0e, 0x1c, 0xcd,
    0xa7, 0x00, 0xaf, 0xb0, 0x20, 0xe0, 0xe0, 0x43, 0x3e, 0x83, 0xcd, 0x9f,
    0x00, 0x0e, 0x27, 0xcd, 0xa7, 0x00, 0x3e, 0xc1, 0xcd, 0x9f, 0x00, 0x11,
    0x8a, 0x01, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x1b, 0x7a, 0xb3, 0x20,
    0xf5, 0x18, 0x49, 0x0e, 0x13, 0xe2, 0x0c, 0x3e, 0x87, 0xe2, 0xc9, 0xf0,
    0x44, 0xfe, 0x90, 0x20, 0xfa, 0x0d, 0x20, 0xf7, 0xc9, 0x78, 0x22, 0x04,
    0x0d, 0x20, 0xfa, 0xc9, 0x47, 0x0e, 0x04, 0xaf, 0xc5, 0xcb, 0x10, 0x17,
    0xc1, 0xcb, 0x10, 0x17, 0x0d, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9,
    0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c, 0x00, 0x54, 0xa8, 0xfc,
    0x42, 0x4f, 0x4f, 0x54, 0x49, 0x58, 0x2e, 0x44, 0x4d, 0x47, 0x20, 0x76,
    0x31, 0x2e, 0x32, 0x00, 0x3e, 0xff, 0xc6, 0x01, 0x0b, 0x1e, 0xd8, 0x21,
    0x4d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3e, 0x01, 0xe0, 0x50};

Gameboy
gameboy_initialize(const char *filepath) {
	Gameboy gb = {};

	platform_game_load(filepath, &gb.rom);

	gb.rom.boot_rom = BOOT_ROM;
	gb.rom.boot_rom_enabled = true;

	return gb;
}

void
opcode_execute(const u8 opcode, Gameboy *gb, const bool debug) {
	if (gb->cpu.prefix) {
		// ==================== PREFIX ====================
		switch (opcode) {
		case 0x00:
			if (debug) platform_instruction_log(gb, opcode, "RLC B");
			rlc_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x01:
			if (debug) platform_instruction_log(gb, opcode, "RLC C");
			rlc_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x02:
			if (debug) platform_instruction_log(gb, opcode, "RLC D");
			rlc_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x03:
			if (debug) platform_instruction_log(gb, opcode, "RLC E");
			rlc_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x04:
			if (debug) platform_instruction_log(gb, opcode, "RLC H");
			rlc_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x05:
			if (debug) platform_instruction_log(gb, opcode, "RLC L");
			rlc_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x06:
			if (debug) platform_instruction_log(gb, opcode, "RLC [HL]");
			rlc_aHL(gb);
			break;
		case 0x07:
			if (debug) platform_instruction_log(gb, opcode, "RLC A");
			rlc_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x08:
			if (debug) platform_instruction_log(gb, opcode, "RRC B");
			rrc_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x09:
			if (debug) platform_instruction_log(gb, opcode, "RRC C");
			rrc_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x0A:
			if (debug) platform_instruction_log(gb, opcode, "RRC D");
			rrc_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x0B:
			if (debug) platform_instruction_log(gb, opcode, "RRC E");
			rrc_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x0C:
			if (debug) platform_instruction_log(gb, opcode, "RRC H");
			rrc_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x0D:
			if (debug) platform_instruction_log(gb, opcode, "RRC L");
			rrc_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x0E:
			if (debug) platform_instruction_log(gb, opcode, "RRC [HL]");
			rrc_aHL(gb);
			break;
		case 0x0F:
			if (debug) platform_instruction_log(gb, opcode, "RRC A");
			rrc_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x10:
			if (debug) platform_instruction_log(gb, opcode, "RL B");
			rl_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x11:
			if (debug) platform_instruction_log(gb, opcode, "RL C");
			rl_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x12:
			if (debug) platform_instruction_log(gb, opcode, "RL D");
			rl_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x13:
			if (debug) platform_instruction_log(gb, opcode, "RL E");
			rl_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x14:
			if (debug) platform_instruction_log(gb, opcode, "RL H");
			rl_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x15:
			if (debug) platform_instruction_log(gb, opcode, "RL L");
			rl_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x16:
			if (debug) platform_instruction_log(gb, opcode, "RL [HL]");
			rl_aHL(gb);
			break;
		case 0x17:
			if (debug) platform_instruction_log(gb, opcode, "RL A");
			rl_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x18:
			if (debug) platform_instruction_log(gb, opcode, "RR B");
			rr_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x19:
			if (debug) platform_instruction_log(gb, opcode, "RR C");
			rr_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x1A:
			if (debug) platform_instruction_log(gb, opcode, "RR D");
			rr_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x1B:
			if (debug) platform_instruction_log(gb, opcode, "RR E");
			rr_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x1C:
			if (debug) platform_instruction_log(gb, opcode, "RR H");
			rr_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x1D:
			if (debug) platform_instruction_log(gb, opcode, "RR L");
			rr_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x1E:
			if (debug) platform_instruction_log(gb, opcode, "RR [HL]");
			rr_aHL(gb);
			break;
		case 0x1F:
			if (debug) platform_instruction_log(gb, opcode, "RR A");
			rr_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x20:
			if (debug) platform_instruction_log(gb, opcode, "SLA B");
			sla_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x21:
			if (debug) platform_instruction_log(gb, opcode, "SLA C");
			sla_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x22:
			if (debug) platform_instruction_log(gb, opcode, "SLA D");
			sla_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x23:
			if (debug) platform_instruction_log(gb, opcode, "SLA E");
			sla_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x24:
			if (debug) platform_instruction_log(gb, opcode, "SLA H");
			sla_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x25:
			if (debug) platform_instruction_log(gb, opcode, "SLA L");
			sla_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x26:
			if (debug) platform_instruction_log(gb, opcode, "SLA [HL]");
			sla_aHL(gb);
			break;
		case 0x27:
			if (debug) platform_instruction_log(gb, opcode, "SLA A");
			sla_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x28:
			if (debug) platform_instruction_log(gb, opcode, "SRA B");
			sra_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x29:
			if (debug) platform_instruction_log(gb, opcode, "SRA C");
			sra_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x2A:
			if (debug) platform_instruction_log(gb, opcode, "SRA D");
			sra_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x2B:
			if (debug) platform_instruction_log(gb, opcode, "SRA E");
			sra_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x2C:
			if (debug) platform_instruction_log(gb, opcode, "SRA H");
			sra_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x2D:
			if (debug) platform_instruction_log(gb, opcode, "SRA L");
			sra_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x2E:
			if (debug) platform_instruction_log(gb, opcode, "SRA [HL]");
			sra_aHL(gb);
			break;
		case 0x2F:
			if (debug) platform_instruction_log(gb, opcode, "SRA A");
			sra_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x30:
			if (debug) platform_instruction_log(gb, opcode, "SWAP B");
			swap_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x31:
			if (debug) platform_instruction_log(gb, opcode, "SWAP C");
			swap_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x32:
			if (debug) platform_instruction_log(gb, opcode, "SWAP D");
			swap_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x33:
			if (debug) platform_instruction_log(gb, opcode, "SWAP E");
			swap_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x34:
			if (debug) platform_instruction_log(gb, opcode, "SWAP H");
			swap_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x35:
			if (debug) platform_instruction_log(gb, opcode, "SWAP L");
			swap_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x36:
			if (debug) platform_instruction_log(gb, opcode, "SWAP [HL]");
			swap_aHL(gb);
			break;
		case 0x37:
			if (debug) platform_instruction_log(gb, opcode, "SWAP A");
			swap_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x38:
			if (debug) platform_instruction_log(gb, opcode, "SRL B");
			srl_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x39:
			if (debug) platform_instruction_log(gb, opcode, "SRL C");
			srl_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x3A:
			if (debug) platform_instruction_log(gb, opcode, "SRL D");
			srl_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x3B:
			if (debug) platform_instruction_log(gb, opcode, "SRL E");
			srl_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x3C:
			if (debug) platform_instruction_log(gb, opcode, "SRL H");
			srl_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x3D:
			if (debug) platform_instruction_log(gb, opcode, "SRL L");
			srl_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x3E:
			if (debug) platform_instruction_log(gb, opcode, "SRL [HL]");
			srl_aHL(gb);
			break;
		case 0x3F:
			if (debug) platform_instruction_log(gb, opcode, "SRL A");
			srl_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x40:
			if (debug) platform_instruction_log(gb, opcode, "BIT 0, B");
			bit_u3_r8(gb, 0, gb->cpu.regs[BC].high);
			break;
		case 0x41:
			if (debug) platform_instruction_log(gb, opcode, "BIT 0, C");
			bit_u3_r8(gb, 0, gb->cpu.regs[BC].low);
			break;
		case 0x42:
			if (debug) platform_instruction_log(gb, opcode, "BIT 0, D");
			bit_u3_r8(gb, 0, gb->cpu.regs[DE].high);
			break;
		case 0x43:
			if (debug) platform_instruction_log(gb, opcode, "BIT 0, E");
			bit_u3_r8(gb, 0, gb->cpu.regs[DE].low);
			break;
		case 0x44:
			if (debug) platform_instruction_log(gb, opcode, "BIT 0, H");
			bit_u3_r8(gb, 0, gb->cpu.regs[HL].high);
			break;
		case 0x45:
			if (debug) platform_instruction_log(gb, opcode, "BIT 0, L");
			bit_u3_r8(gb, 0, gb->cpu.regs[HL].low);
			break;
		case 0x46:
			if (debug) platform_instruction_log(gb, opcode, "BIT 0, [HL]");
			bit_u3_aHL(gb, 0);
			break;
		case 0x47:
			if (debug) platform_instruction_log(gb, opcode, "BIT 0, A");
			bit_u3_r8(gb, 0, gb->cpu.regs[AF].high);
			break;
		case 0x48:
			if (debug) platform_instruction_log(gb, opcode, "BIT 1, B");
			bit_u3_r8(gb, 1, gb->cpu.regs[BC].high);
			break;
		case 0x49:
			if (debug) platform_instruction_log(gb, opcode, "BIT 1, C");
			bit_u3_r8(gb, 1, gb->cpu.regs[BC].low);
			break;
		case 0x4A:
			if (debug) platform_instruction_log(gb, opcode, "BIT 1, D");
			bit_u3_r8(gb, 1, gb->cpu.regs[DE].high);
			break;
		case 0x4B:
			if (debug) platform_instruction_log(gb, opcode, "BIT 1, E");
			bit_u3_r8(gb, 1, gb->cpu.regs[DE].low);
			break;
		case 0x4C:
			if (debug) platform_instruction_log(gb, opcode, "BIT 1, H");
			bit_u3_r8(gb, 1, gb->cpu.regs[HL].high);
			break;
		case 0x4D:
			if (debug) platform_instruction_log(gb, opcode, "BIT 1, L");
			bit_u3_r8(gb, 1, gb->cpu.regs[HL].low);
			break;
		case 0x4E:
			if (debug) platform_instruction_log(gb, opcode, "BIT 1, [HL]");
			bit_u3_aHL(gb, 1);
			break;
		case 0x4F:
			if (debug) platform_instruction_log(gb, opcode, "BIT 1, A");
			bit_u3_r8(gb, 1, gb->cpu.regs[AF].high);
			break;
		case 0x50:
			if (debug) platform_instruction_log(gb, opcode, "BIT 2, B");
			bit_u3_r8(gb, 2, gb->cpu.regs[BC].high);
			break;
		case 0x51:
			if (debug) platform_instruction_log(gb, opcode, "BIT 2, C");
			bit_u3_r8(gb, 2, gb->cpu.regs[BC].low);
			break;
		case 0x52:
			if (debug) platform_instruction_log(gb, opcode, "BIT 2, D");
			bit_u3_r8(gb, 2, gb->cpu.regs[DE].high);
			break;
		case 0x53:
			if (debug) platform_instruction_log(gb, opcode, "BIT 2, E");
			bit_u3_r8(gb, 2, gb->cpu.regs[DE].low);
			break;
		case 0x54:
			if (debug) platform_instruction_log(gb, opcode, "BIT 2, H");
			bit_u3_r8(gb, 2, gb->cpu.regs[HL].high);
			break;
		case 0x55:
			if (debug) platform_instruction_log(gb, opcode, "BIT 2, L");
			bit_u3_r8(gb, 2, gb->cpu.regs[HL].low);
			break;
		case 0x56:
			if (debug) platform_instruction_log(gb, opcode, "BIT 2, [HL]");
			bit_u3_aHL(gb, 2);
			break;
		case 0x57:
			if (debug) platform_instruction_log(gb, opcode, "BIT 2, A");
			bit_u3_r8(gb, 2, gb->cpu.regs[AF].high);
			break;
		case 0x58:
			if (debug) platform_instruction_log(gb, opcode, "BIT 3, B");
			bit_u3_r8(gb, 3, gb->cpu.regs[BC].high);
			break;
		case 0x59:
			if (debug) platform_instruction_log(gb, opcode, "BIT 3, C");
			bit_u3_r8(gb, 3, gb->cpu.regs[BC].low);
			break;
		case 0x5A:
			if (debug) platform_instruction_log(gb, opcode, "BIT 3, D");
			bit_u3_r8(gb, 3, gb->cpu.regs[DE].high);
			break;
		case 0x5B:
			if (debug) platform_instruction_log(gb, opcode, "BIT 3, E");
			bit_u3_r8(gb, 3, gb->cpu.regs[DE].low);
			break;
		case 0x5C:
			if (debug) platform_instruction_log(gb, opcode, "BIT 3, H");
			bit_u3_r8(gb, 3, gb->cpu.regs[HL].high);
			break;
		case 0x5D:
			if (debug) platform_instruction_log(gb, opcode, "BIT 3, L");
			bit_u3_r8(gb, 3, gb->cpu.regs[HL].low);
			break;
		case 0x5E:
			if (debug) platform_instruction_log(gb, opcode, "BIT 3, [HL]");
			bit_u3_aHL(gb, 3);
			break;
		case 0x5F:
			if (debug) platform_instruction_log(gb, opcode, "BIT 3, A");
			bit_u3_r8(gb, 3, gb->cpu.regs[AF].high);
			break;
		case 0x60:
			if (debug) platform_instruction_log(gb, opcode, "BIT 4, B");
			bit_u3_r8(gb, 4, gb->cpu.regs[BC].high);
			break;
		case 0x61:
			if (debug) platform_instruction_log(gb, opcode, "BIT 4, C");
			bit_u3_r8(gb, 4, gb->cpu.regs[BC].low);
			break;
		case 0x62:
			if (debug) platform_instruction_log(gb, opcode, "BIT 4, D");
			bit_u3_r8(gb, 4, gb->cpu.regs[DE].high);
			break;
		case 0x63:
			if (debug) platform_instruction_log(gb, opcode, "BIT 4, E");
			bit_u3_r8(gb, 4, gb->cpu.regs[DE].low);
			break;
		case 0x64:
			if (debug) platform_instruction_log(gb, opcode, "BIT 4, H");
			bit_u3_r8(gb, 4, gb->cpu.regs[HL].high);
			break;
		case 0x65:
			if (debug) platform_instruction_log(gb, opcode, "BIT 4, L");
			bit_u3_r8(gb, 4, gb->cpu.regs[HL].low);
			break;
		case 0x66:
			if (debug) platform_instruction_log(gb, opcode, "BIT 4, [HL]");
			bit_u3_aHL(gb, 4);
			break;
		case 0x67:
			if (debug) platform_instruction_log(gb, opcode, "BIT 4, A");
			bit_u3_r8(gb, 4, gb->cpu.regs[AF].high);
			break;
		case 0x68:
			if (debug) platform_instruction_log(gb, opcode, "BIT 5, B");
			bit_u3_r8(gb, 5, gb->cpu.regs[BC].high);
			break;
		case 0x69:
			if (debug) platform_instruction_log(gb, opcode, "BIT 5, C");
			bit_u3_r8(gb, 5, gb->cpu.regs[BC].low);
			break;
		case 0x6A:
			if (debug) platform_instruction_log(gb, opcode, "BIT 5, D");
			bit_u3_r8(gb, 5, gb->cpu.regs[DE].high);
			break;
		case 0x6B:
			if (debug) platform_instruction_log(gb, opcode, "BIT 5, E");
			bit_u3_r8(gb, 5, gb->cpu.regs[DE].low);
			break;
		case 0x6C:
			if (debug) platform_instruction_log(gb, opcode, "BIT 5, H");
			bit_u3_r8(gb, 5, gb->cpu.regs[HL].high);
			break;
		case 0x6D:
			if (debug) platform_instruction_log(gb, opcode, "BIT 5, L");
			bit_u3_r8(gb, 5, gb->cpu.regs[HL].low);
			break;
		case 0x6E:
			if (debug) platform_instruction_log(gb, opcode, "BIT 5, [HL]");
			bit_u3_aHL(gb, 5);
			break;
		case 0x6F:
			if (debug) platform_instruction_log(gb, opcode, "BIT 5, A");
			bit_u3_r8(gb, 5, gb->cpu.regs[AF].high);
			break;
		case 0x70:
			if (debug) platform_instruction_log(gb, opcode, "BIT 6, B");
			bit_u3_r8(gb, 6, gb->cpu.regs[BC].high);
			break;
		case 0x71:
			if (debug) platform_instruction_log(gb, opcode, "BIT 6, C");
			bit_u3_r8(gb, 6, gb->cpu.regs[BC].low);
			break;
		case 0x72:
			if (debug) platform_instruction_log(gb, opcode, "BIT 6, D");
			bit_u3_r8(gb, 6, gb->cpu.regs[DE].high);
			break;
		case 0x73:
			if (debug) platform_instruction_log(gb, opcode, "BIT 6, E");
			bit_u3_r8(gb, 6, gb->cpu.regs[DE].low);
			break;
		case 0x74:
			if (debug) platform_instruction_log(gb, opcode, "BIT 6, H");
			bit_u3_r8(gb, 6, gb->cpu.regs[HL].high);
			break;
		case 0x75:
			if (debug) platform_instruction_log(gb, opcode, "BIT 6, L");
			bit_u3_r8(gb, 6, gb->cpu.regs[HL].low);
			break;
		case 0x76:
			if (debug) platform_instruction_log(gb, opcode, "BIT 6, [HL]");
			bit_u3_aHL(gb, 6);
			break;
		case 0x77:
			if (debug) platform_instruction_log(gb, opcode, "BIT 6, A");
			bit_u3_r8(gb, 6, gb->cpu.regs[AF].high);
			break;
		case 0x78:
			if (debug) platform_instruction_log(gb, opcode, "BIT 7, B");
			bit_u3_r8(gb, 7, gb->cpu.regs[BC].high);
			break;
		case 0x79:
			if (debug) platform_instruction_log(gb, opcode, "BIT 7, C");
			bit_u3_r8(gb, 7, gb->cpu.regs[BC].low);
			break;
		case 0x7A:
			if (debug) platform_instruction_log(gb, opcode, "BIT 7, D");
			bit_u3_r8(gb, 7, gb->cpu.regs[DE].high);
			break;
		case 0x7B:
			if (debug) platform_instruction_log(gb, opcode, "BIT 7, E");
			bit_u3_r8(gb, 7, gb->cpu.regs[DE].low);
			break;
		case 0x7C:
			if (debug) platform_instruction_log(gb, opcode, "BIT 7, H");
			bit_u3_r8(gb, 7, gb->cpu.regs[HL].high);
			break;
		case 0x7D:
			if (debug) platform_instruction_log(gb, opcode, "BIT 7, L");
			bit_u3_r8(gb, 7, gb->cpu.regs[HL].low);
			break;
		case 0x7E:
			if (debug) platform_instruction_log(gb, opcode, "BIT 7, [HL]");
			bit_u3_aHL(gb, 7);
			break;
		case 0x7F:
			if (debug) platform_instruction_log(gb, opcode, "BIT 7, A");
			bit_u3_r8(gb, 7, gb->cpu.regs[AF].high);
			break;
		case 0x80:
			if (debug) platform_instruction_log(gb, opcode, "RES 0, B");
			res_u3_r8(gb, 0, &gb->cpu.regs[BC].high);
			break;
		case 0x81:
			if (debug) platform_instruction_log(gb, opcode, "RES 0, C");
			res_u3_r8(gb, 0, &gb->cpu.regs[BC].low);
			break;
		case 0x82:
			if (debug) platform_instruction_log(gb, opcode, "RES 0, D");
			res_u3_r8(gb, 0, &gb->cpu.regs[DE].high);
			break;
		case 0x83:
			if (debug) platform_instruction_log(gb, opcode, "RES 0, E");
			res_u3_r8(gb, 0, &gb->cpu.regs[DE].low);
			break;
		case 0x84:
			if (debug) platform_instruction_log(gb, opcode, "RES 0, H");
			res_u3_r8(gb, 0, &gb->cpu.regs[HL].high);
			break;
		case 0x85:
			if (debug) platform_instruction_log(gb, opcode, "RES 0, L");
			res_u3_r8(gb, 0, &gb->cpu.regs[HL].low);
			break;
		case 0x86:
			if (debug) platform_instruction_log(gb, opcode, "RES 0, [HL]");
			res_u3_aHL(gb, 0);
			break;
		case 0x87:
			if (debug) platform_instruction_log(gb, opcode, "RES 0, A");
			res_u3_r8(gb, 0, &gb->cpu.regs[AF].high);
			break;
		case 0x88:
			if (debug) platform_instruction_log(gb, opcode, "RES 1, B");
			res_u3_r8(gb, 1, &gb->cpu.regs[BC].high);
			break;
		case 0x89:
			if (debug) platform_instruction_log(gb, opcode, "RES 1, C");
			res_u3_r8(gb, 1, &gb->cpu.regs[BC].low);
			break;
		case 0x8A:
			if (debug) platform_instruction_log(gb, opcode, "RES 1, D");
			res_u3_r8(gb, 1, &gb->cpu.regs[DE].high);
			break;
		case 0x8B:
			if (debug) platform_instruction_log(gb, opcode, "RES 1, E");
			res_u3_r8(gb, 1, &gb->cpu.regs[DE].low);
			break;
		case 0x8C:
			if (debug) platform_instruction_log(gb, opcode, "RES 1, H");
			res_u3_r8(gb, 1, &gb->cpu.regs[HL].high);
			break;
		case 0x8D:
			if (debug) platform_instruction_log(gb, opcode, "RES 1, L");
			res_u3_r8(gb, 1, &gb->cpu.regs[HL].low);
			break;
		case 0x8E:
			if (debug) platform_instruction_log(gb, opcode, "RES 1, [HL]");
			res_u3_aHL(gb, 1);
			break;
		case 0x8F:
			if (debug) platform_instruction_log(gb, opcode, "RES 1, A");
			res_u3_r8(gb, 1, &gb->cpu.regs[AF].high);
			break;
		case 0x90:
			if (debug) platform_instruction_log(gb, opcode, "RES 2, B");
			res_u3_r8(gb, 2, &gb->cpu.regs[BC].high);
			break;
		case 0x91:
			if (debug) platform_instruction_log(gb, opcode, "RES 2, C");
			res_u3_r8(gb, 2, &gb->cpu.regs[BC].low);
			break;
		case 0x92:
			if (debug) platform_instruction_log(gb, opcode, "RES 2, D");
			res_u3_r8(gb, 2, &gb->cpu.regs[DE].high);
			break;
		case 0x93:
			if (debug) platform_instruction_log(gb, opcode, "RES 2, E");
			res_u3_r8(gb, 2, &gb->cpu.regs[DE].low);
			break;
		case 0x94:
			if (debug) platform_instruction_log(gb, opcode, "RES 2, H");
			res_u3_r8(gb, 2, &gb->cpu.regs[HL].high);
			break;
		case 0x95:
			if (debug) platform_instruction_log(gb, opcode, "RES 2, L");
			res_u3_r8(gb, 2, &gb->cpu.regs[HL].low);
			break;
		case 0x96:
			if (debug) platform_instruction_log(gb, opcode, "RES 2, [HL]");
			res_u3_aHL(gb, 2);
			break;
		case 0x97:
			if (debug) platform_instruction_log(gb, opcode, "RES 2, A");
			res_u3_r8(gb, 2, &gb->cpu.regs[AF].high);
			break;
		case 0x98:
			if (debug) platform_instruction_log(gb, opcode, "RES 3, B");
			res_u3_r8(gb, 3, &gb->cpu.regs[BC].high);
			break;
		case 0x99:
			if (debug) platform_instruction_log(gb, opcode, "RES 3, C");
			res_u3_r8(gb, 3, &gb->cpu.regs[BC].low);
			break;
		case 0x9A:
			if (debug) platform_instruction_log(gb, opcode, "RES 3, D");
			res_u3_r8(gb, 3, &gb->cpu.regs[DE].high);
			break;
		case 0x9B:
			if (debug) platform_instruction_log(gb, opcode, "RES 3, E");
			res_u3_r8(gb, 3, &gb->cpu.regs[DE].low);
			break;
		case 0x9C:
			if (debug) platform_instruction_log(gb, opcode, "RES 3, H");
			res_u3_r8(gb, 3, &gb->cpu.regs[HL].high);
			break;
		case 0x9D:
			if (debug) platform_instruction_log(gb, opcode, "RES 3, L");
			res_u3_r8(gb, 3, &gb->cpu.regs[HL].low);
			break;
		case 0x9E:
			if (debug) platform_instruction_log(gb, opcode, "RES 3, [HL]");
			res_u3_aHL(gb, 3);
			break;
		case 0x9F:
			if (debug) platform_instruction_log(gb, opcode, "RES 3, A");
			res_u3_r8(gb, 3, &gb->cpu.regs[AF].high);
			break;
		case 0xA0:
			if (debug) platform_instruction_log(gb, opcode, "RES 4, B");
			res_u3_r8(gb, 4, &gb->cpu.regs[BC].high);
			break;
		case 0xA1:
			if (debug) platform_instruction_log(gb, opcode, "RES 4, C");
			res_u3_r8(gb, 4, &gb->cpu.regs[BC].low);
			break;
		case 0xA2:
			if (debug) platform_instruction_log(gb, opcode, "RES 4, D");
			res_u3_r8(gb, 4, &gb->cpu.regs[DE].high);
			break;
		case 0xA3:
			if (debug) platform_instruction_log(gb, opcode, "RES 4, E");
			res_u3_r8(gb, 4, &gb->cpu.regs[DE].low);
			break;
		case 0xA4:
			if (debug) platform_instruction_log(gb, opcode, "RES 4, H");
			res_u3_r8(gb, 4, &gb->cpu.regs[HL].high);
			break;
		case 0xA5:
			if (debug) platform_instruction_log(gb, opcode, "RES 4, L");
			res_u3_r8(gb, 4, &gb->cpu.regs[HL].low);
			break;
		case 0xA6:
			if (debug) platform_instruction_log(gb, opcode, "RES 4, [HL]");
			res_u3_aHL(gb, 4);
			break;
		case 0xA7:
			if (debug) platform_instruction_log(gb, opcode, "RES 4, A");
			res_u3_r8(gb, 4, &gb->cpu.regs[AF].high);
			break;
		case 0xA8:
			if (debug) platform_instruction_log(gb, opcode, "RES 5, B");
			res_u3_r8(gb, 5, &gb->cpu.regs[BC].high);
			break;
		case 0xA9:
			if (debug) platform_instruction_log(gb, opcode, "RES 5, C");
			res_u3_r8(gb, 5, &gb->cpu.regs[BC].low);
			break;
		case 0xAA:
			if (debug) platform_instruction_log(gb, opcode, "RES 5, D");
			res_u3_r8(gb, 5, &gb->cpu.regs[DE].high);
			break;
		case 0xAB:
			if (debug) platform_instruction_log(gb, opcode, "RES 5, E");
			res_u3_r8(gb, 5, &gb->cpu.regs[DE].low);
			break;
		case 0xAC:
			if (debug) platform_instruction_log(gb, opcode, "RES 5, H");
			res_u3_r8(gb, 5, &gb->cpu.regs[HL].high);
			break;
		case 0xAD:
			if (debug) platform_instruction_log(gb, opcode, "RES 5, L");
			res_u3_r8(gb, 5, &gb->cpu.regs[HL].low);
			break;
		case 0xAE:
			if (debug) platform_instruction_log(gb, opcode, "RES 5, [HL]");
			res_u3_aHL(gb, 5);
			break;
		case 0xAF:
			if (debug) platform_instruction_log(gb, opcode, "RES 5, A");
			res_u3_r8(gb, 5, &gb->cpu.regs[AF].high);
			break;
		case 0xB0:
			if (debug) platform_instruction_log(gb, opcode, "RES 6, B");
			res_u3_r8(gb, 6, &gb->cpu.regs[BC].high);
			break;
		case 0xB1:
			if (debug) platform_instruction_log(gb, opcode, "RES 6, C");
			res_u3_r8(gb, 6, &gb->cpu.regs[BC].low);
			break;
		case 0xB2:
			if (debug) platform_instruction_log(gb, opcode, "RES 6, D");
			res_u3_r8(gb, 6, &gb->cpu.regs[DE].high);
			break;
		case 0xB3:
			if (debug) platform_instruction_log(gb, opcode, "RES 6, E");
			res_u3_r8(gb, 6, &gb->cpu.regs[DE].low);
			break;
		case 0xB4:
			if (debug) platform_instruction_log(gb, opcode, "RES 6, H");
			res_u3_r8(gb, 6, &gb->cpu.regs[HL].high);
			break;
		case 0xB5:
			if (debug) platform_instruction_log(gb, opcode, "RES 6, L");
			res_u3_r8(gb, 6, &gb->cpu.regs[HL].low);
			break;
		case 0xB6:
			if (debug) platform_instruction_log(gb, opcode, "RES 6, [HL]");
			res_u3_aHL(gb, 6);
			break;
		case 0xB7:
			if (debug) platform_instruction_log(gb, opcode, "RES 6, A");
			res_u3_r8(gb, 6, &gb->cpu.regs[AF].high);
			break;
		case 0xB8:
			if (debug) platform_instruction_log(gb, opcode, "RES 7, B");
			res_u3_r8(gb, 7, &gb->cpu.regs[BC].high);
			break;
		case 0xB9:
			if (debug) platform_instruction_log(gb, opcode, "RES 7, C");
			res_u3_r8(gb, 7, &gb->cpu.regs[BC].low);
			break;
		case 0xBA:
			if (debug) platform_instruction_log(gb, opcode, "RES 7, D");
			res_u3_r8(gb, 7, &gb->cpu.regs[DE].high);
			break;
		case 0xBB:
			if (debug) platform_instruction_log(gb, opcode, "RES 7, E");
			res_u3_r8(gb, 7, &gb->cpu.regs[DE].low);
			break;
		case 0xBC:
			if (debug) platform_instruction_log(gb, opcode, "RES 7, H");
			res_u3_r8(gb, 7, &gb->cpu.regs[HL].high);
			break;
		case 0xBD:
			if (debug) platform_instruction_log(gb, opcode, "RES 7, L");
			res_u3_r8(gb, 7, &gb->cpu.regs[HL].low);
			break;
		case 0xBE:
			if (debug) platform_instruction_log(gb, opcode, "RES 7, [HL]");
			res_u3_aHL(gb, 7);
			break;
		case 0xBF:
			if (debug) platform_instruction_log(gb, opcode, "RES 7, A");
			res_u3_r8(gb, 7, &gb->cpu.regs[AF].high);
			break;
		case 0xC0:
			if (debug) platform_instruction_log(gb, opcode, "SET 0, B");
			set_u3_r8(gb, 0, &gb->cpu.regs[BC].high);
			break;
		case 0xC1:
			if (debug) platform_instruction_log(gb, opcode, "SET 0, C");
			set_u3_r8(gb, 0, &gb->cpu.regs[BC].low);
			break;
		case 0xC2:
			if (debug) platform_instruction_log(gb, opcode, "SET 0, D");
			set_u3_r8(gb, 0, &gb->cpu.regs[DE].high);
			break;
		case 0xC3:
			if (debug) platform_instruction_log(gb, opcode, "SET 0, E");
			set_u3_r8(gb, 0, &gb->cpu.regs[DE].low);
			break;
		case 0xC4:
			if (debug) platform_instruction_log(gb, opcode, "SET 0, H");
			set_u3_r8(gb, 0, &gb->cpu.regs[HL].high);
			break;
		case 0xC5:
			if (debug) platform_instruction_log(gb, opcode, "SET 0, L");
			set_u3_r8(gb, 0, &gb->cpu.regs[HL].low);
			break;
		case 0xC6:
			if (debug) platform_instruction_log(gb, opcode, "SET 0, [HL]");
			set_u3_aHL(gb, 0);
			break;
		case 0xC7:
			if (debug) platform_instruction_log(gb, opcode, "SET 0, A");
			set_u3_r8(gb, 0, &gb->cpu.regs[AF].high);
			break;
		case 0xC8:
			if (debug) platform_instruction_log(gb, opcode, "SET 1, B");
			set_u3_r8(gb, 1, &gb->cpu.regs[BC].high);
			break;
		case 0xC9:
			if (debug) platform_instruction_log(gb, opcode, "SET 1, C");
			set_u3_r8(gb, 1, &gb->cpu.regs[BC].low);
			break;
		case 0xCA:
			if (debug) platform_instruction_log(gb, opcode, "SET 1, D");
			set_u3_r8(gb, 1, &gb->cpu.regs[DE].high);
			break;
		case 0xCB:
			if (debug) platform_instruction_log(gb, opcode, "SET 1, E");
			set_u3_r8(gb, 1, &gb->cpu.regs[DE].low);
			break;
		case 0xCC:
			if (debug) platform_instruction_log(gb, opcode, "SET 1, H");
			set_u3_r8(gb, 1, &gb->cpu.regs[HL].high);
			break;
		case 0xCD:
			if (debug) platform_instruction_log(gb, opcode, "SET 1, L");
			set_u3_r8(gb, 1, &gb->cpu.regs[HL].low);
			break;
		case 0xCE:
			if (debug) platform_instruction_log(gb, opcode, "SET 1, [HL]");
			set_u3_aHL(gb, 1);
			break;
		case 0xCF:
			if (debug) platform_instruction_log(gb, opcode, "SET 1, A");
			set_u3_r8(gb, 1, &gb->cpu.regs[AF].high);
			break;
		case 0xD0:
			if (debug) platform_instruction_log(gb, opcode, "SET 2, B");
			set_u3_r8(gb, 2, &gb->cpu.regs[BC].high);
			break;
		case 0xD1:
			if (debug) platform_instruction_log(gb, opcode, "SET 2, C");
			set_u3_r8(gb, 2, &gb->cpu.regs[BC].low);
			break;
		case 0xD2:
			if (debug) platform_instruction_log(gb, opcode, "SET 2, D");
			set_u3_r8(gb, 2, &gb->cpu.regs[DE].high);
			break;
		case 0xD3:
			if (debug) platform_instruction_log(gb, opcode, "SET 2, E");
			set_u3_r8(gb, 2, &gb->cpu.regs[DE].low);
			break;
		case 0xD4:
			if (debug) platform_instruction_log(gb, opcode, "SET 2, H");
			set_u3_r8(gb, 2, &gb->cpu.regs[HL].high);
			break;
		case 0xD5:
			if (debug) platform_instruction_log(gb, opcode, "SET 2, L");
			set_u3_r8(gb, 2, &gb->cpu.regs[HL].low);
			break;
		case 0xD6:
			if (debug) platform_instruction_log(gb, opcode, "SET 2, [HL]");
			set_u3_aHL(gb, 2);
			break;
		case 0xD7:
			if (debug) platform_instruction_log(gb, opcode, "SET 2, A");
			set_u3_r8(gb, 2, &gb->cpu.regs[AF].high);
			break;
		case 0xD8:
			if (debug) platform_instruction_log(gb, opcode, "SET 3, B");
			set_u3_r8(gb, 3, &gb->cpu.regs[BC].high);
			break;
		case 0xD9:
			if (debug) platform_instruction_log(gb, opcode, "SET 3, C");
			set_u3_r8(gb, 3, &gb->cpu.regs[BC].low);
			break;
		case 0xDA:
			if (debug) platform_instruction_log(gb, opcode, "SET 3, D");
			set_u3_r8(gb, 3, &gb->cpu.regs[DE].high);
			break;
		case 0xDB:
			if (debug) platform_instruction_log(gb, opcode, "SET 3, E");
			set_u3_r8(gb, 3, &gb->cpu.regs[DE].low);
			break;
		case 0xDC:
			if (debug) platform_instruction_log(gb, opcode, "SET 3, H");
			set_u3_r8(gb, 3, &gb->cpu.regs[HL].high);
			break;
		case 0xDD:
			if (debug) platform_instruction_log(gb, opcode, "SET 3, L");
			set_u3_r8(gb, 3, &gb->cpu.regs[HL].low);
			break;
		case 0xDE:
			if (debug) platform_instruction_log(gb, opcode, "SET 3, [HL]");
			set_u3_aHL(gb, 3);
			break;
		case 0xDF:
			if (debug) platform_instruction_log(gb, opcode, "SET 3, A");
			set_u3_r8(gb, 3, &gb->cpu.regs[AF].high);
			break;
		case 0xE0:
			if (debug) platform_instruction_log(gb, opcode, "SET 4, B");
			set_u3_r8(gb, 4, &gb->cpu.regs[BC].high);
			break;
		case 0xE1:
			if (debug) platform_instruction_log(gb, opcode, "SET 4, C");
			set_u3_r8(gb, 4, &gb->cpu.regs[BC].low);
			break;
		case 0xE2:
			if (debug) platform_instruction_log(gb, opcode, "SET 4, D");
			set_u3_r8(gb, 4, &gb->cpu.regs[DE].high);
			break;
		case 0xE3:
			if (debug) platform_instruction_log(gb, opcode, "SET 4, E");
			set_u3_r8(gb, 4, &gb->cpu.regs[DE].low);
			break;
		case 0xE4:
			if (debug) platform_instruction_log(gb, opcode, "SET 4, H");
			set_u3_r8(gb, 4, &gb->cpu.regs[HL].high);
			break;
		case 0xE5:
			if (debug) platform_instruction_log(gb, opcode, "SET 4, L");
			set_u3_r8(gb, 4, &gb->cpu.regs[HL].low);
			break;
		case 0xE6:
			if (debug) platform_instruction_log(gb, opcode, "SET 4, [HL]");
			set_u3_aHL(gb, 4);
			break;
		case 0xE7:
			if (debug) platform_instruction_log(gb, opcode, "SET 4, A");
			set_u3_r8(gb, 4, &gb->cpu.regs[AF].high);
			break;
		case 0xE8:
			if (debug) platform_instruction_log(gb, opcode, "SET 5, B");
			set_u3_r8(gb, 5, &gb->cpu.regs[BC].high);
			break;
		case 0xE9:
			if (debug) platform_instruction_log(gb, opcode, "SET 5, C");
			set_u3_r8(gb, 5, &gb->cpu.regs[BC].low);
			break;
		case 0xEA:
			if (debug) platform_instruction_log(gb, opcode, "SET 5, D");
			set_u3_r8(gb, 5, &gb->cpu.regs[DE].high);
			break;
		case 0xEB:
			if (debug) platform_instruction_log(gb, opcode, "SET 5, E");
			set_u3_r8(gb, 5, &gb->cpu.regs[DE].low);
			break;
		case 0xEC:
			if (debug) platform_instruction_log(gb, opcode, "SET 5, H");
			set_u3_r8(gb, 5, &gb->cpu.regs[HL].high);
			break;
		case 0xED:
			if (debug) platform_instruction_log(gb, opcode, "SET 5, L");
			set_u3_r8(gb, 5, &gb->cpu.regs[HL].low);
			break;
		case 0xEE:
			if (debug) platform_instruction_log(gb, opcode, "SET 5, [HL]");
			set_u3_aHL(gb, 5);
			break;
		case 0xEF:
			if (debug) platform_instruction_log(gb, opcode, "SET 5, A");
			set_u3_r8(gb, 5, &gb->cpu.regs[AF].high);
			break;
		case 0xF0:
			if (debug) platform_instruction_log(gb, opcode, "SET 6, B");
			set_u3_r8(gb, 6, &gb->cpu.regs[BC].high);
			break;
		case 0xF1:
			if (debug) platform_instruction_log(gb, opcode, "SET 6, C");
			set_u3_r8(gb, 6, &gb->cpu.regs[BC].low);
			break;
		case 0xF2:
			if (debug) platform_instruction_log(gb, opcode, "SET 6, D");
			set_u3_r8(gb, 6, &gb->cpu.regs[DE].high);
			break;
		case 0xF3:
			if (debug) platform_instruction_log(gb, opcode, "SET 6, E");
			set_u3_r8(gb, 6, &gb->cpu.regs[DE].low);
			break;
		case 0xF4:
			if (debug) platform_instruction_log(gb, opcode, "SET 6, H");
			set_u3_r8(gb, 6, &gb->cpu.regs[HL].high);
			break;
		case 0xF5:
			if (debug) platform_instruction_log(gb, opcode, "SET 6, L");
			set_u3_r8(gb, 6, &gb->cpu.regs[HL].low);
			break;
		case 0xF6:
			if (debug) platform_instruction_log(gb, opcode, "SET 6, [HL]");
			set_u3_aHL(gb, 6);
			break;
		case 0xF7:
			if (debug) platform_instruction_log(gb, opcode, "SET 6, A");
			set_u3_r8(gb, 6, &gb->cpu.regs[AF].high);
			break;
		case 0xF8:
			if (debug) platform_instruction_log(gb, opcode, "SET 7, B");
			set_u3_r8(gb, 7, &gb->cpu.regs[BC].high);
			break;
		case 0xF9:
			if (debug) platform_instruction_log(gb, opcode, "SET 7, C");
			set_u3_r8(gb, 7, &gb->cpu.regs[BC].low);
			break;
		case 0xFA:
			if (debug) platform_instruction_log(gb, opcode, "SET 7, D");
			set_u3_r8(gb, 7, &gb->cpu.regs[DE].high);
			break;
		case 0xFB:
			if (debug) platform_instruction_log(gb, opcode, "SET 7, E");
			set_u3_r8(gb, 7, &gb->cpu.regs[DE].low);
			break;
		case 0xFC:
			if (debug) platform_instruction_log(gb, opcode, "SET 7, H");
			set_u3_r8(gb, 7, &gb->cpu.regs[HL].high);
			break;
		case 0xFD:
			if (debug) platform_instruction_log(gb, opcode, "SET 7, L");
			set_u3_r8(gb, 7, &gb->cpu.regs[HL].low);
			break;
		case 0xFE:
			if (debug) platform_instruction_log(gb, opcode, "SET 7, [HL]");
			set_u3_aHL(gb, 7);
			break;
		case 0xFF:
			if (debug) platform_instruction_log(gb, opcode, "SET 7, A");
			set_u3_r8(gb, 7, &gb->cpu.regs[AF].high);
			break;

		default:
			if (debug) platform_instruction_log(gb, opcode, "INVALID PREFIX");
			gb->cpu.regs[PC].full += 1;
			gb->cpu.cycle += 1;
			break;
		}
		gb->cpu.prefix = false;
	} else {
		// ==================== NON-PREFIX ====================
		switch (opcode) {
		case 0x00:
			if (debug) platform_instruction_log(gb, opcode, "NOP");
			nop(gb);
			break;
		case 0x01: {
			if (debug) platform_instruction_log(gb, opcode, "LD BC, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_r16_n16(gb, &gb->cpu.regs[BC].full, n16);
		} break;
		case 0x02:
			if (debug) platform_instruction_log(gb, opcode, "LD [BC], A");
			ld_a16_A(gb, gb->cpu.regs[BC].full);
			break;
		case 0x03:
			if (debug) platform_instruction_log(gb, opcode, "INC BC");
			inc_r16(gb, &gb->cpu.regs[BC].full);
			break;
		case 0x04:
			if (debug) platform_instruction_log(gb, opcode, "INC B");
			inc_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x05:
			if (debug) platform_instruction_log(gb, opcode, "DEC B");
			dec_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x06: {
			if (debug) platform_instruction_log(gb, opcode, "LD B, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[BC].high, n8);
		} break;
		case 0x07:
			if (debug) platform_instruction_log(gb, opcode, "RLCA");
			rlca(gb);
			break;
		case 0x08: {
			if (debug) platform_instruction_log(gb, opcode, "LD [n16], SP");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_addr16_SP(gb, n16);
		} break;
		case 0x09:
			if (debug) platform_instruction_log(gb, opcode, "ADD HL, BC");
			add_HL_r16(gb, gb->cpu.regs[BC].full);
			break;
		case 0x0A:
			if (debug) platform_instruction_log(gb, opcode, "LD A, [BC]");
			ld_A_a16(gb, gb->cpu.regs[BC].full);
			break;
		case 0x0B:
			if (debug) platform_instruction_log(gb, opcode, "DEC BC");
			dec_r16(gb, &gb->cpu.regs[BC].full);
			break;
		case 0x0C:
			if (debug) platform_instruction_log(gb, opcode, "INC C");
			inc_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x0D:
			if (debug) platform_instruction_log(gb, opcode, "DEC C");
			dec_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x0E: {
			if (debug) platform_instruction_log(gb, opcode, "LD C, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[BC].low, n8);
		} break;
		case 0x0F:
			if (debug) platform_instruction_log(gb, opcode, "RRCA");
			rrca(gb);
			break;
		case 0x10: {
			if (debug) platform_instruction_log(gb, opcode, "STOP n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			stop_n8(gb, n8);
		} break;
		case 0x11: {
			if (debug) platform_instruction_log(gb, opcode, "LD DE, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_r16_n16(gb, &gb->cpu.regs[DE].full, n16);
		} break;
		case 0x12:
			if (debug) platform_instruction_log(gb, opcode, "LD [DE], A");
			ld_a16_A(gb, gb->cpu.regs[DE].full);
			break;
		case 0x13:
			if (debug) platform_instruction_log(gb, opcode, "INC DE");
			inc_r16(gb, &gb->cpu.regs[DE].full);
			break;
		case 0x14:
			if (debug) platform_instruction_log(gb, opcode, "INC D");
			inc_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x15:
			if (debug) platform_instruction_log(gb, opcode, "DEC D");
			dec_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x16: {
			if (debug) platform_instruction_log(gb, opcode, "LD D, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[DE].high, n8);
		} break;
		case 0x17:
			if (debug) platform_instruction_log(gb, opcode, "RLA");
			rla(gb);
			break;
		case 0x18: {
			if (debug) platform_instruction_log(gb, opcode, "JR n16");
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_n16(gb, offset);
			break;
		}
		case 0x19:
			if (debug) platform_instruction_log(gb, opcode, "ADD HL, DE");
			add_HL_r16(gb, gb->cpu.regs[DE].full);
			break;
		case 0x1A:
			if (debug) platform_instruction_log(gb, opcode, "LD A, [DE]");
			ld_A_a16(gb, gb->cpu.regs[DE].full);
			break;
		case 0x1B:
			if (debug) platform_instruction_log(gb, opcode, "DEC DE");
			dec_r16(gb, &gb->cpu.regs[DE].full);
			break;
		case 0x1C:
			if (debug) platform_instruction_log(gb, opcode, "INC E");
			inc_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x1D:
			if (debug) platform_instruction_log(gb, opcode, "DEC E");
			dec_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x1E: {
			if (debug) platform_instruction_log(gb, opcode, "LD E, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[DE].low, n8);
		} break;
		case 0x1F:
			if (debug) platform_instruction_log(gb, opcode, "RRA");
			rra(gb);
			break;
		case 0x20: {
			if (debug) platform_instruction_log(gb, opcode, "JR NZ, n16");
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_cc_n16(gb, Z, false, offset);
			break;
		}
		case 0x21: {
			if (debug) platform_instruction_log(gb, opcode, "LD HL, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_r16_n16(gb, &gb->cpu.regs[HL].full, n16);
		} break;
		case 0x22:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL+], A");
			ld_aHLi_A(gb);
			break;
		case 0x23:
			if (debug) platform_instruction_log(gb, opcode, "INC HL");
			inc_r16(gb, &gb->cpu.regs[HL].full);
			break;
		case 0x24:
			if (debug) platform_instruction_log(gb, opcode, "INC H");
			inc_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x25:
			if (debug) platform_instruction_log(gb, opcode, "DEC H");
			dec_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x26: {
			if (debug) platform_instruction_log(gb, opcode, "LD H, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[HL].high, n8);
		} break;
		case 0x27:
			if (debug) platform_instruction_log(gb, opcode, "DAA");
			daa(gb);
			break;
		case 0x28: {
			if (debug) platform_instruction_log(gb, opcode, "JR Z, n16");
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_cc_n16(gb, Z, true, offset);
			break;
		}
		case 0x29:
			if (debug) platform_instruction_log(gb, opcode, "ADD HL, HL");
			add_HL_r16(gb, gb->cpu.regs[HL].full);
			break;
		case 0x2A:
			if (debug) platform_instruction_log(gb, opcode, "LD A, [HL+]");
			ld_A_aHLi(gb);
			break;
		case 0x2B:
			if (debug) platform_instruction_log(gb, opcode, "DEC HL");
			dec_r16(gb, &gb->cpu.regs[HL].full);
			break;
		case 0x2C:
			if (debug) platform_instruction_log(gb, opcode, "INC L");
			inc_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x2D:
			if (debug) platform_instruction_log(gb, opcode, "DEC L");
			dec_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x2E: {
			if (debug) platform_instruction_log(gb, opcode, "LD L, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[HL].low, n8);
		} break;
		case 0x2F:
			if (debug) platform_instruction_log(gb, opcode, "CPL");
			cpl(gb);
			break;
		case 0x30: {
			if (debug) platform_instruction_log(gb, opcode, "JR NC, n16");
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_cc_n16(gb, C, false, offset);
			break;
		}
		case 0x31: {
			if (debug) platform_instruction_log(gb, opcode, "LD SP, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_r16_n16(gb, &gb->cpu.regs[SP].full, n16);
		} break;
		case 0x32:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL-], A");
			ld_aHLd_A(gb);
			break;
		case 0x33:
			if (debug) platform_instruction_log(gb, opcode, "INC SP");
			inc_r16(gb, &gb->cpu.regs[SP].full);
			break;
		case 0x34:
			if (debug) platform_instruction_log(gb, opcode, "INC [HL]");
			inc_aHL(gb);
			break;
		case 0x35:
			if (debug) platform_instruction_log(gb, opcode, "DEC [HL]");
			dec_aHL(gb);
			break;
		case 0x36: {
			if (debug) platform_instruction_log(gb, opcode, "LD [HL], n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_aHL_n8(gb, n8);
		} break;
		case 0x37:
			if (debug) platform_instruction_log(gb, opcode, "SCF");
			scf(gb);
			break;
		case 0x38: {
			if (debug) platform_instruction_log(gb, opcode, "JR C, n16");
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_cc_n16(gb, C, true, offset);
			break;
		}
		case 0x39:
			if (debug) platform_instruction_log(gb, opcode, "ADD HL, SP");
			add_HL_r16(gb, gb->cpu.regs[SP].full);
			break;
		case 0x3A:
			if (debug) platform_instruction_log(gb, opcode, "LD A, [HL-]");
			ld_A_aHLd(gb);
			break;
		case 0x3B:
			if (debug) platform_instruction_log(gb, opcode, "DEC SP");
			dec_r16(gb, &gb->cpu.regs[SP].full);
			break;
		case 0x3C:
			if (debug) platform_instruction_log(gb, opcode, "INC A");
			inc_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x3D:
			if (debug) platform_instruction_log(gb, opcode, "DEC A");
			dec_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x3E: {
			if (debug) platform_instruction_log(gb, opcode, "LD A, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[AF].high, n8);
		} break;
		case 0x3F:
			if (debug) platform_instruction_log(gb, opcode, "CCF");
			ccf(gb);
			break;
		case 0x40:
			if (debug) platform_instruction_log(gb, opcode, "LD B, B");
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[BC].high);
			break;
		case 0x41:
			if (debug) platform_instruction_log(gb, opcode, "LD B, C");
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[BC].low);
			break;
		case 0x42:
			if (debug) platform_instruction_log(gb, opcode, "LD B, D");
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[DE].high);
			break;
		case 0x43:
			if (debug) platform_instruction_log(gb, opcode, "LD B, E");
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[DE].low);
			break;
		case 0x44:
			if (debug) platform_instruction_log(gb, opcode, "LD B, H");
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[HL].high);
			break;
		case 0x45:
			if (debug) platform_instruction_log(gb, opcode, "LD B, L");
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[HL].low);
			break;
		case 0x46:
			if (debug) platform_instruction_log(gb, opcode, "LD B, [HL]");
			ld_r8_aHL(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x47:
			if (debug) platform_instruction_log(gb, opcode, "LD B, A");
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[AF].high);
			break;
		case 0x48:
			if (debug) platform_instruction_log(gb, opcode, "LD C, B");
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[BC].high);
			break;
		case 0x49:
			if (debug) platform_instruction_log(gb, opcode, "LD C, C");
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[BC].low);
			break;
		case 0x4A:
			if (debug) platform_instruction_log(gb, opcode, "LD C, D");
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[DE].high);
			break;
		case 0x4B:
			if (debug) platform_instruction_log(gb, opcode, "LD C, E");
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[DE].low);
			break;
		case 0x4C:
			if (debug) platform_instruction_log(gb, opcode, "LD C, H");
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[HL].high);
			break;
		case 0x4D:
			if (debug) platform_instruction_log(gb, opcode, "LD C, L");
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[HL].low);
			break;
		case 0x4E:
			if (debug) platform_instruction_log(gb, opcode, "LD C, [HL]");
			ld_r8_aHL(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x4F:
			if (debug) platform_instruction_log(gb, opcode, "LD C, A");
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[AF].high);
			break;
		case 0x50:
			if (debug) platform_instruction_log(gb, opcode, "LD D, B");
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[BC].high);
			break;
		case 0x51:
			if (debug) platform_instruction_log(gb, opcode, "LD D, C");
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[BC].low);
			break;
		case 0x52:
			if (debug) platform_instruction_log(gb, opcode, "LD D, D");
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[DE].high);
			break;
		case 0x53:
			if (debug) platform_instruction_log(gb, opcode, "LD D, E");
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[DE].low);
			break;
		case 0x54:
			if (debug) platform_instruction_log(gb, opcode, "LD D, H");
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[HL].high);
			break;
		case 0x55:
			if (debug) platform_instruction_log(gb, opcode, "LD D, L");
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[HL].low);
			break;
		case 0x56:
			if (debug) platform_instruction_log(gb, opcode, "LD D, [HL]");
			ld_r8_aHL(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x57:
			if (debug) platform_instruction_log(gb, opcode, "LD D, A");
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[AF].high);
			break;
		case 0x58:
			if (debug) platform_instruction_log(gb, opcode, "LD E, B");
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[BC].high);
			break;
		case 0x59:
			if (debug) platform_instruction_log(gb, opcode, "LD E, C");
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[BC].low);
			break;
		case 0x5A:
			if (debug) platform_instruction_log(gb, opcode, "LD E, D");
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[DE].high);
			break;
		case 0x5B:
			if (debug) platform_instruction_log(gb, opcode, "LD E, E");
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[DE].low);
			break;
		case 0x5C:
			if (debug) platform_instruction_log(gb, opcode, "LD E, H");
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[HL].high);
			break;
		case 0x5D:
			if (debug) platform_instruction_log(gb, opcode, "LD E, L");
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[HL].low);
			break;
		case 0x5E:
			if (debug) platform_instruction_log(gb, opcode, "LD E, [HL]");
			ld_r8_aHL(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x5F:
			if (debug) platform_instruction_log(gb, opcode, "LD E, A");
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[AF].high);
			break;
		case 0x60:
			if (debug) platform_instruction_log(gb, opcode, "LD H, B");
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[BC].high);
			break;
		case 0x61:
			if (debug) platform_instruction_log(gb, opcode, "LD H, C");
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[BC].low);
			break;
		case 0x62:
			if (debug) platform_instruction_log(gb, opcode, "LD H, D");
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[DE].high);
			break;
		case 0x63:
			if (debug) platform_instruction_log(gb, opcode, "LD H, E");
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[DE].low);
			break;
		case 0x64:
			if (debug) platform_instruction_log(gb, opcode, "LD H, H");
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[HL].high);
			break;
		case 0x65:
			if (debug) platform_instruction_log(gb, opcode, "LD H, L");
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[HL].low);
			break;
		case 0x66:
			if (debug) platform_instruction_log(gb, opcode, "LD H, [HL]");
			ld_r8_aHL(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x67:
			if (debug) platform_instruction_log(gb, opcode, "LD H, A");
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[AF].high);
			break;
		case 0x68:
			if (debug) platform_instruction_log(gb, opcode, "LD L, B");
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[BC].high);
			break;
		case 0x69:
			if (debug) platform_instruction_log(gb, opcode, "LD L, C");
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[BC].low);
			break;
		case 0x6A:
			if (debug) platform_instruction_log(gb, opcode, "LD L, D");
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[DE].high);
			break;
		case 0x6B:
			if (debug) platform_instruction_log(gb, opcode, "LD L, E");
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[DE].low);
			break;
		case 0x6C:
			if (debug) platform_instruction_log(gb, opcode, "LD L, H");
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[HL].high);
			break;
		case 0x6D:
			if (debug) platform_instruction_log(gb, opcode, "LD L, L");
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[HL].low);
			break;
		case 0x6E:
			if (debug) platform_instruction_log(gb, opcode, "LD L, [HL]");
			ld_r8_aHL(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x6F:
			if (debug) platform_instruction_log(gb, opcode, "LD L, A");
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[AF].high);
			break;
		case 0x70:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL], B");
			ld_aHL_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x71:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL], C");
			ld_aHL_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x72:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL], D");
			ld_aHL_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x73:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL], E");
			ld_aHL_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x74:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL], H");
			ld_aHL_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x75:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL], L");
			ld_aHL_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x76:
			if (debug) platform_instruction_log(gb, opcode, "HALT");
			halt(gb);
			break;
		case 0x77:
			if (debug) platform_instruction_log(gb, opcode, "LD [HL], A");
			ld_aHL_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0x78:
			if (debug) platform_instruction_log(gb, opcode, "LD A, B");
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[BC].high);
			break;
		case 0x79:
			if (debug) platform_instruction_log(gb, opcode, "LD A, C");
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[BC].low);
			break;
		case 0x7A:
			if (debug) platform_instruction_log(gb, opcode, "LD A, D");
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[DE].high);
			break;
		case 0x7B:
			if (debug) platform_instruction_log(gb, opcode, "LD A, E");
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[DE].low);
			break;
		case 0x7C:
			if (debug) platform_instruction_log(gb, opcode, "LD A, H");
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[HL].high);
			break;
		case 0x7D:
			if (debug) platform_instruction_log(gb, opcode, "LD A, L");
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[HL].low);
			break;
		case 0x7E:
			if (debug) platform_instruction_log(gb, opcode, "LD A, [HL]");
			ld_r8_aHL(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x7F:
			if (debug) platform_instruction_log(gb, opcode, "LD A, A");
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[AF].high);
			break;
		case 0x80:
			if (debug) platform_instruction_log(gb, opcode, "ADD A, B");
			add_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x81:
			if (debug) platform_instruction_log(gb, opcode, "ADD A, C");
			add_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x82:
			if (debug) platform_instruction_log(gb, opcode, "ADD A, D");
			add_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x83:
			if (debug) platform_instruction_log(gb, opcode, "ADD A, E");
			add_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x84:
			if (debug) platform_instruction_log(gb, opcode, "ADD A, H");
			add_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x85:
			if (debug) platform_instruction_log(gb, opcode, "ADD A, L");
			add_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x86:
			if (debug) platform_instruction_log(gb, opcode, "ADD A, [HL]");
			add_A_aHL(gb);
			break;
		case 0x87:
			if (debug) platform_instruction_log(gb, opcode, "ADD A, A");
			add_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0x88:
			if (debug) platform_instruction_log(gb, opcode, "ADC A, B");
			adc_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x89:
			if (debug) platform_instruction_log(gb, opcode, "ADC A, C");
			adc_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x8A:
			if (debug) platform_instruction_log(gb, opcode, "ADC A, D");
			adc_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x8B:
			if (debug) platform_instruction_log(gb, opcode, "ADC A, E");
			adc_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x8C:
			if (debug) platform_instruction_log(gb, opcode, "ADC A, H");
			adc_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x8D:
			if (debug) platform_instruction_log(gb, opcode, "ADC A, L");
			adc_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x8E:
			if (debug) platform_instruction_log(gb, opcode, "ADC A, [HL]");
			adc_A_aHL(gb);
			break;
		case 0x8F:
			if (debug) platform_instruction_log(gb, opcode, "ADC A, A");
			adc_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0x90:
			if (debug) platform_instruction_log(gb, opcode, "SUB A, B");
			sub_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x91:
			if (debug) platform_instruction_log(gb, opcode, "SUB A, C");
			sub_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x92:
			if (debug) platform_instruction_log(gb, opcode, "SUB A, D");
			sub_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x93:
			if (debug) platform_instruction_log(gb, opcode, "SUB A, E");
			sub_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x94:
			if (debug) platform_instruction_log(gb, opcode, "SUB A, H");
			sub_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x95:
			if (debug) platform_instruction_log(gb, opcode, "SUB A, L");
			sub_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x96:
			if (debug) platform_instruction_log(gb, opcode, "SUB A, [HL]");
			sub_A_aHL(gb);
			break;
		case 0x97:
			if (debug) platform_instruction_log(gb, opcode, "SUB A, A");
			sub_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0x98:
			if (debug) platform_instruction_log(gb, opcode, "SBC A, B");
			sbc_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x99:
			if (debug) platform_instruction_log(gb, opcode, "SBC A, C");
			sbc_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x9A:
			if (debug) platform_instruction_log(gb, opcode, "SBC A, D");
			sbc_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x9B:
			if (debug) platform_instruction_log(gb, opcode, "SBC A, E");
			sbc_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x9C:
			if (debug) platform_instruction_log(gb, opcode, "SBC A, H");
			sbc_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x9D:
			if (debug) platform_instruction_log(gb, opcode, "SBC A, L");
			sbc_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x9E:
			if (debug) platform_instruction_log(gb, opcode, "SBC A, [HL]");
			sbc_A_aHL(gb);
			break;
		case 0x9F:
			if (debug) platform_instruction_log(gb, opcode, "SBC A, A");
			sbc_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xA0:
			if (debug) platform_instruction_log(gb, opcode, "AND A, B");
			and_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0xA1:
			if (debug) platform_instruction_log(gb, opcode, "AND A, C");
			and_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0xA2:
			if (debug) platform_instruction_log(gb, opcode, "AND A, D");
			and_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0xA3:
			if (debug) platform_instruction_log(gb, opcode, "AND A, E");
			and_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0xA4:
			if (debug) platform_instruction_log(gb, opcode, "AND A, H");
			and_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0xA5:
			if (debug) platform_instruction_log(gb, opcode, "AND A, L");
			and_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0xA6:
			if (debug) platform_instruction_log(gb, opcode, "AND A, [HL]");
			and_A_aHL(gb);
			break;
		case 0xA7:
			if (debug) platform_instruction_log(gb, opcode, "AND A, A");
			and_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xA8:
			if (debug) platform_instruction_log(gb, opcode, "XOR A, B");
			xor_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0xA9:
			if (debug) platform_instruction_log(gb, opcode, "XOR A, C");
			xor_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0xAA:
			if (debug) platform_instruction_log(gb, opcode, "XOR A, D");
			xor_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0xAB:
			if (debug) platform_instruction_log(gb, opcode, "XOR A, E");
			xor_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0xAC:
			if (debug) platform_instruction_log(gb, opcode, "XOR A, H");
			xor_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0xAD:
			if (debug) platform_instruction_log(gb, opcode, "XOR A, L");
			xor_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0xAE:
			if (debug) platform_instruction_log(gb, opcode, "XOR A, [HL]");
			xor_A_aHL(gb);
			break;
		case 0xAF:
			if (debug) platform_instruction_log(gb, opcode, "XOR A, A");
			xor_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xB0:
			if (debug) platform_instruction_log(gb, opcode, "OR A, B");
			or_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0xB1:
			if (debug) platform_instruction_log(gb, opcode, "OR A, C");
			or_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0xB2:
			if (debug) platform_instruction_log(gb, opcode, "OR A, D");
			or_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0xB3:
			if (debug) platform_instruction_log(gb, opcode, "OR A, E");
			or_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0xB4:
			if (debug) platform_instruction_log(gb, opcode, "OR A, H");
			or_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0xB5:
			if (debug) platform_instruction_log(gb, opcode, "OR A, L");
			or_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0xB6:
			if (debug) platform_instruction_log(gb, opcode, "OR A, [HL]");
			or_A_aHL(gb);
			break;
		case 0xB7:
			if (debug) platform_instruction_log(gb, opcode, "OR A, A");
			or_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xB8:
			if (debug) platform_instruction_log(gb, opcode, "CP A, B");
			cp_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0xB9:
			if (debug) platform_instruction_log(gb, opcode, "CP A, C");
			cp_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0xBA:
			if (debug) platform_instruction_log(gb, opcode, "CP A, D");
			cp_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0xBB:
			if (debug) platform_instruction_log(gb, opcode, "CP A, E");
			cp_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0xBC:
			if (debug) platform_instruction_log(gb, opcode, "CP A, H");
			cp_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0xBD:
			if (debug) platform_instruction_log(gb, opcode, "CP A, L");
			cp_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0xBE:
			if (debug) platform_instruction_log(gb, opcode, "CP A, [HL]");
			cp_A_aHL(gb);
			break;
		case 0xBF:
			if (debug) platform_instruction_log(gb, opcode, "CP A, A");
			cp_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xC0:
			if (debug) platform_instruction_log(gb, opcode, "RET NZ");
			ret_cc(gb, Z, false);
			break;
		case 0xC1:
			if (debug) platform_instruction_log(gb, opcode, "POP BC");
			pop_r16(gb, &gb->cpu.regs[BC].full);
			break;
		case 0xC2: {
			if (debug) platform_instruction_log(gb, opcode, "JP NZ, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_cc_n16(gb, Z, false, n16);
			break;
		}
		case 0xC3: {
			if (debug) platform_instruction_log(gb, opcode, "JP n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_n16(gb, n16);
			break;
		}
		case 0xC4: {
			if (debug) platform_instruction_log(gb, opcode, "CALL NZ, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_cc_n16(gb, Z, false, n16);
			break;
		}
		case 0xC5:
			if (debug) platform_instruction_log(gb, opcode, "PUSH BC");
			push_r16(gb, gb->cpu.regs[BC].full);
			break;
		case 0xC6: {
			if (debug) platform_instruction_log(gb, opcode, "ADD A, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			add_A_n8(gb, n8);
			break;
		}
		case 0xC7:
			if (debug) platform_instruction_log(gb, opcode, "RST $00");
			rst(gb, 0x00);
			break;
		case 0xC8:
			if (debug) platform_instruction_log(gb, opcode, "RET Z");
			ret_cc(gb, Z, true);
			break;
		case 0xC9:
			if (debug) platform_instruction_log(gb, opcode, "RET");
			ret(gb);
			break;
		case 0xCA: {
			if (debug) platform_instruction_log(gb, opcode, "JP Z, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_cc_n16(gb, Z, true, n16);
			break;
		}
		case 0xCB:
			if (debug) platform_instruction_log(gb, opcode, "PREFIX TOGGLE");
			gb->cpu.prefix = true;
			gb->cpu.regs[PC].full += 1;
			break;
		case 0xCC: {
			if (debug) platform_instruction_log(gb, opcode, "CALL Z, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_cc_n16(gb, Z, true, n16);
			break;
		}
		case 0xCD: {
			if (debug) platform_instruction_log(gb, opcode, "CALL n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_n16(gb, n16);
			break;
		}
		case 0xCE: {
			if (debug) platform_instruction_log(gb, opcode, "ADC A, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			adc_A_n8(gb, n8);
			break;
		}
		case 0xCF:
			if (debug) platform_instruction_log(gb, opcode, "RST $08");
			rst(gb, 0x08);
			break;
		case 0xD0:
			if (debug) platform_instruction_log(gb, opcode, "RET NC");
			ret_cc(gb, C, false);
			break;
		case 0xD1:
			if (debug) platform_instruction_log(gb, opcode, "POP DE");
			pop_r16(gb, &gb->cpu.regs[DE].full);
			break;
		case 0xD2: {
			if (debug) platform_instruction_log(gb, opcode, "JP NC, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_cc_n16(gb, C, false, n16);
			break;
		}
		case 0xD4: {
			if (debug) platform_instruction_log(gb, opcode, "CALL NC, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_cc_n16(gb, C, false, n16);
			break;
		}
		case 0xD5:
			if (debug) platform_instruction_log(gb, opcode, "PUSH DE");
			push_r16(gb, gb->cpu.regs[DE].full);
			break;
		case 0xD6: {
			if (debug) platform_instruction_log(gb, opcode, "SUB A, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			sub_A_n8(gb, n8);
			break;
		}
		case 0xD7:
			if (debug) platform_instruction_log(gb, opcode, "RST $10");
			rst(gb, 0x10);
			break;
		case 0xD8:
			if (debug) platform_instruction_log(gb, opcode, "RET C");
			ret_cc(gb, C, true);
			break;
		case 0xD9:
			if (debug) platform_instruction_log(gb, opcode, "RETI");
			reti(gb);
			break;
		case 0xDA: {
			if (debug) platform_instruction_log(gb, opcode, "JP C, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_cc_n16(gb, C, true, n16);
			break;
		}
		case 0xDC: {
			if (debug) platform_instruction_log(gb, opcode, "CALL C, n16");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_cc_n16(gb, C, true, n16);
			break;
		}
		case 0xDE: {
			if (debug) platform_instruction_log(gb, opcode, "SBC A, n8");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			sbc_A_n8(gb, n8);
			break;
		}
		case 0xDF:
			if (debug) platform_instruction_log(gb, opcode, "RST $18");
			rst(gb, 0x18);
			break;
		case 0xE0: {
			if (debug) platform_instruction_log(gb, opcode, "LDH [n16], A");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ldh_addr16_A(gb, 0xFF00 + n8);
			break;
		}
		case 0xE1:
			if (debug) platform_instruction_log(gb, opcode, "POP HL");
			pop_r16(gb, &gb->cpu.regs[HL].full);
			break;
		case 0xE2:
			if (debug) platform_instruction_log(gb, opcode, "LDH [C], A");
			ldh_aC_A(gb);
			break;
		case 0xE5:
			if (debug) platform_instruction_log(gb, opcode, "PUSH HL");
			push_r16(gb, gb->cpu.regs[HL].full);
			break;
		case 0xE6: {
			if (debug) platform_instruction_log(gb, opcode, "AND A, n8");
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			and_A_n8(gb, n8);
		} break;
		case 0xE7:
			if (debug) platform_instruction_log(gb, opcode, "RST $20");
			rst(gb, 0x20);
			break;
		case 0xE8: {
			if (debug) platform_instruction_log(gb, opcode, "ADD SP, n8");
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			add_SP_n8(gb, n8);
		} break;
		case 0xE9:
			if (debug) platform_instruction_log(gb, opcode, "JP HL");
			jp_aHL(gb);
			break;
		case 0xEA: {
			if (debug) platform_instruction_log(gb, opcode, "LD [n16], A");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_addr16_A(gb, n16);
			break;
		}
		case 0xEE: {
			if (debug) platform_instruction_log(gb, opcode, "XOR A, n8");
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			xor_A_n8(gb, n8);
		} break;
		case 0xEF:
			if (debug) platform_instruction_log(gb, opcode, "RST $28");
			rst(gb, 0x28);
			break;
		case 0xF0: {
			if (debug) platform_instruction_log(gb, opcode, "LDH A, [n16]");
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ldh_A_addr16(gb, 0xFF00 + n8);
			break;
		}
		case 0xF1:
			if (debug) platform_instruction_log(gb, opcode, "POP AF");
			pop_AF(gb);
			break;
		case 0xF2:
			if (debug) platform_instruction_log(gb, opcode, "LDH A, [C]");
			ldh_A_aC(gb);
			break;
		case 0xF3:
			if (debug) platform_instruction_log(gb, opcode, "DI");
			di(gb);
			break;
		case 0xF5:
			if (debug) platform_instruction_log(gb, opcode, "PUSH AF");
			push_AF(gb);
			break;
		case 0xF6: {
			if (debug) platform_instruction_log(gb, opcode, "OR A, n8");
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			or_A_n8(gb, n8);
		} break;
		case 0xF7:
			if (debug) platform_instruction_log(gb, opcode, "RST $30");
			rst(gb, 0x30);
			break;
		case 0xF8: {
			if (debug) platform_instruction_log(gb, opcode, "LD HL, SP+n8");
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			ld_HL_SPe8(gb, n8);
		} break;
		case 0xF9:
			if (debug) platform_instruction_log(gb, opcode, "LD SP, HL");
			ld_SP_HL(gb);
			break;
		case 0xFA: {
			if (debug) platform_instruction_log(gb, opcode, "LD A, [n16]");
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_A_addr16(gb, n16);
			break;
		}
		case 0xFB:
			if (debug) platform_instruction_log(gb, opcode, "EI");
			ei(gb);
			break;
		case 0xFE: {
			if (debug) platform_instruction_log(gb, opcode, "CP A, n8");
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			cp_A_n8(gb, n8);
		} break;
		case 0xFF:
			if (debug) platform_instruction_log(gb, opcode, "RST $38");
			rst(gb, 0x38);
			break;
		default:
			if (debug) platform_instruction_log(gb, opcode, "INVALID OPCODE");
			gb->cpu.regs[PC].full += 1;
			gb->cpu.cycle += 1;
			break;
		}
	}
}
