#include "gameboy.h"
#include "../platform/desktop.h"
#include "../misc/types.h"

#include "cpu.c"

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

void opcode_execute(const u8 opcode, Gameboy *gb, const bool debug) {
	if (gb->cpu.prefix) {
		// ==================== PREFIX ====================
		// switch (opcode) {
		// case 0x00:
		// 	platform_instruction_log(opcode, "RLC B");
		// 	rlc_r8(&gb->cpu.regs[BC].high);
		// 	break;
		// 	case 0x01:
		// 		platform_instruction_log(opcode, "RLC C");
		// 		rlc_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x02:
		// 		platform_instruction_log(opcode, "RLC D");
		// 		rlc_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x03:
		// 		platform_instruction_log(opcode, "RLC E");
		// 		rlc_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x04:
		// 		platform_instruction_log(opcode, "RLC H");
		// 		rlc_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x05:
		// 		platform_instruction_log(opcode, "RLC L");
		// 		rlc_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x06:
		// 		platform_instruction_log(opcode, "RLC [HL]");
		// 		rlc_aHL();
		// 		break;
		// 	case 0x07:
		// 		platform_instruction_log(opcode, "RLC A");
		// 		rlc_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x08:
		// 		platform_instruction_log(opcode, "RRC B");
		// 		rrc_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x09:
		// 		platform_instruction_log(opcode, "RRC C");
		// 		rrc_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x0A:
		// 		platform_instruction_log(opcode, "RRC D");
		// 		rrc_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x0B:
		// 		platform_instruction_log(opcode, "RRC E");
		// 		rrc_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x0C:
		// 		platform_instruction_log(opcode, "RRC H");
		// 		rrc_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x0D:
		// 		platform_instruction_log(opcode, "RRC L");
		// 		rrc_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x0E:
		// 		platform_instruction_log(opcode, "RRC [HL]");
		// 		rrc_aHL();
		// 		break;
		// 	case 0x0F:
		// 		platform_instruction_log(opcode, "RRC A");
		// 		rrc_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x10:
		// 		platform_instruction_log(opcode, "RL B");
		// 		rl_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x11:
		// 		platform_instruction_log(opcode, "RL C");
		// 		rl_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x12:
		// 		platform_instruction_log(opcode, "RL D");
		// 		rl_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x13:
		// 		platform_instruction_log(opcode, "RL E");
		// 		rl_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x14:
		// 		platform_instruction_log(opcode, "RL H");
		// 		rl_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x15:
		// 		platform_instruction_log(opcode, "RL L");
		// 		rl_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x16:
		// 		platform_instruction_log(opcode, "RL [HL]");
		// 		rl_aHL();
		// 		break;
		// 	case 0x17:
		// 		platform_instruction_log(opcode, "RL A");
		// 		rl_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x18:
		// 		platform_instruction_log(opcode, "RR B");
		// 		rr_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x19:
		// 		platform_instruction_log(opcode, "RR C");
		// 		rr_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x1A:
		// 		platform_instruction_log(opcode, "RR D");
		// 		rr_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x1B:
		// 		platform_instruction_log(opcode, "RR E");
		// 		rr_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x1C:
		// 		platform_instruction_log(opcode, "RR H");
		// 		rr_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x1D:
		// 		platform_instruction_log(opcode, "RR L");
		// 		rr_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x1E:
		// 		platform_instruction_log(opcode, "RR [HL]");
		// 		rr_aHL();
		// 		break;
		// 	case 0x1F:
		// 		platform_instruction_log(opcode, "RR A");
		// 		rr_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x20:
		// 		platform_instruction_log(opcode, "SLA B");
		// 		sla_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x21:
		// 		platform_instruction_log(opcode, "SLA C");
		// 		sla_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x22:
		// 		platform_instruction_log(opcode, "SLA D");
		// 		sla_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x23:
		// 		platform_instruction_log(opcode, "SLA E");
		// 		sla_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x24:
		// 		platform_instruction_log(opcode, "SLA H");
		// 		sla_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x25:
		// 		platform_instruction_log(opcode, "SLA L");
		// 		sla_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x26:
		// 		platform_instruction_log(opcode, "SLA [HL]");
		// 		sla_aHL();
		// 		break;
		// 	case 0x27:
		// 		platform_instruction_log(opcode, "SLA A");
		// 		sla_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x28:
		// 		platform_instruction_log(opcode, "SRA B");
		// 		sra_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x29:
		// 		platform_instruction_log(opcode, "SRA C");
		// 		sra_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x2A:
		// 		platform_instruction_log(opcode, "SRA D");
		// 		sra_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x2B:
		// 		platform_instruction_log(opcode, "SRA E");
		// 		sra_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x2C:
		// 		platform_instruction_log(opcode, "SRA H");
		// 		sra_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x2D:
		// 		platform_instruction_log(opcode, "SRA L");
		// 		sra_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x2E:
		// 		platform_instruction_log(opcode, "SRA [HL]");
		// 		sra_aHL();
		// 		break;
		// 	case 0x2F:
		// 		platform_instruction_log(opcode, "SRA A");
		// 		sra_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x30:
		// 		platform_instruction_log(opcode, "SWAP B");
		// 		swap_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x31:
		// 		platform_instruction_log(opcode, "SWAP C");
		// 		swap_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x32:
		// 		platform_instruction_log(opcode, "SWAP D");
		// 		swap_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x33:
		// 		platform_instruction_log(opcode, "SWAP E");
		// 		swap_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x34:
		// 		platform_instruction_log(opcode, "SWAP H");
		// 		swap_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x35:
		// 		platform_instruction_log(opcode, "SWAP L");
		// 		swap_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x36:
		// 		platform_instruction_log(opcode, "SWAP [HL]");
		// 		swap_aHL();
		// 		break;
		// 	case 0x37:
		// 		platform_instruction_log(opcode, "SWAP A");
		// 		swap_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x38:
		// 		platform_instruction_log(opcode, "SRL B");
		// 		srl_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x39:
		// 		platform_instruction_log(opcode, "SRL C");
		// 		srl_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x3A:
		// 		platform_instruction_log(opcode, "SRL D");
		// 		srl_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x3B:
		// 		platform_instruction_log(opcode, "SRL E");
		// 		srl_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x3C:
		// 		platform_instruction_log(opcode, "SRL H");
		// 		srl_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x3D:
		// 		platform_instruction_log(opcode, "SRL L");
		// 		srl_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x3E:
		// 		platform_instruction_log(opcode, "SRL [HL]");
		// 		srl_aHL();
		// 		break;
		// 	case 0x3F:
		// 		platform_instruction_log(opcode, "SRL A");
		// 		srl_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x40:
		// 		platform_instruction_log(opcode, "BIT 0, B");
		// 		bit_u3_r8(0, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x41:
		// 		platform_instruction_log(opcode, "BIT 0, C");
		// 		bit_u3_r8(0, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x42:
		// 		platform_instruction_log(opcode, "BIT 0, D");
		// 		bit_u3_r8(0, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x43:
		// 		platform_instruction_log(opcode, "BIT 0, E");
		// 		bit_u3_r8(0, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x44:
		// 		platform_instruction_log(opcode, "BIT 0, H");
		// 		bit_u3_r8(0, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x45:
		// 		platform_instruction_log(opcode, "BIT 0, L");
		// 		bit_u3_r8(0, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x46:
		// 		platform_instruction_log(opcode, "BIT 0, [HL]");
		// 		bit_u3_aHL(0);
		// 		break;
		// 	case 0x47:
		// 		platform_instruction_log(opcode, "BIT 0, A");
		// 		bit_u3_r8(0, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x48:
		// 		platform_instruction_log(opcode, "BIT 1, B");
		// 		bit_u3_r8(1, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x49:
		// 		platform_instruction_log(opcode, "BIT 1, C");
		// 		bit_u3_r8(1, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x4A:
		// 		platform_instruction_log(opcode, "BIT 1, D");
		// 		bit_u3_r8(1, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x4B:
		// 		platform_instruction_log(opcode, "BIT 1, E");
		// 		bit_u3_r8(1, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x4C:
		// 		platform_instruction_log(opcode, "BIT 1, H");
		// 		bit_u3_r8(1, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x4D:
		// 		platform_instruction_log(opcode, "BIT 1, L");
		// 		bit_u3_r8(1, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x4E:
		// 		platform_instruction_log(opcode, "BIT 1, [HL]");
		// 		bit_u3_aHL(1);
		// 		break;
		// 	case 0x4F:
		// 		platform_instruction_log(opcode, "BIT 1, A");
		// 		bit_u3_r8(1, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x50:
		// 		platform_instruction_log(opcode, "BIT 2, B");
		// 		bit_u3_r8(2, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x51:
		// 		platform_instruction_log(opcode, "BIT 2, C");
		// 		bit_u3_r8(2, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x52:
		// 		platform_instruction_log(opcode, "BIT 2, D");
		// 		bit_u3_r8(2, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x53:
		// 		platform_instruction_log(opcode, "BIT 2, E");
		// 		bit_u3_r8(2, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x54:
		// 		platform_instruction_log(opcode, "BIT 2, H");
		// 		bit_u3_r8(2, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x55:
		// 		platform_instruction_log(opcode, "BIT 2, L");
		// 		bit_u3_r8(2, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x56:
		// 		platform_instruction_log(opcode, "BIT 2, [HL]");
		// 		bit_u3_aHL(2);
		// 		break;
		// 	case 0x57:
		// 		platform_instruction_log(opcode, "BIT 2, A");
		// 		bit_u3_r8(2, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x58:
		// 		platform_instruction_log(opcode, "BIT 3, B");
		// 		bit_u3_r8(3, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x59:
		// 		platform_instruction_log(opcode, "BIT 3, C");
		// 		bit_u3_r8(3, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x5A:
		// 		platform_instruction_log(opcode, "BIT 3, D");
		// 		bit_u3_r8(3, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x5B:
		// 		platform_instruction_log(opcode, "BIT 3, E");
		// 		bit_u3_r8(3, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x5C:
		// 		platform_instruction_log(opcode, "BIT 3, H");
		// 		bit_u3_r8(3, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x5D:
		// 		platform_instruction_log(opcode, "BIT 3, L");
		// 		bit_u3_r8(3, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x5E:
		// 		platform_instruction_log(opcode, "BIT 3, [HL]");
		// 		bit_u3_aHL(3);
		// 		break;
		// 	case 0x5F:
		// 		platform_instruction_log(opcode, "BIT 3, A");
		// 		bit_u3_r8(3, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x60:
		// 		platform_instruction_log(opcode, "BIT 4, B");
		// 		bit_u3_r8(4, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x61:
		// 		platform_instruction_log(opcode, "BIT 4, C");
		// 		bit_u3_r8(4, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x62:
		// 		platform_instruction_log(opcode, "BIT 4, D");
		// 		bit_u3_r8(4, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x63:
		// 		platform_instruction_log(opcode, "BIT 4, E");
		// 		bit_u3_r8(4, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x64:
		// 		platform_instruction_log(opcode, "BIT 4, H");
		// 		bit_u3_r8(4, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x65:
		// 		platform_instruction_log(opcode, "BIT 4, L");
		// 		bit_u3_r8(4, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x66:
		// 		platform_instruction_log(opcode, "BIT 4, [HL]");
		// 		bit_u3_aHL(4);
		// 		break;
		// 	case 0x67:
		// 		platform_instruction_log(opcode, "BIT 4, A");
		// 		bit_u3_r8(4, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x68:
		// 		platform_instruction_log(opcode, "BIT 5, B");
		// 		bit_u3_r8(5, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x69:
		// 		platform_instruction_log(opcode, "BIT 5, C");
		// 		bit_u3_r8(5, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x6A:
		// 		platform_instruction_log(opcode, "BIT 5, D");
		// 		bit_u3_r8(5, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x6B:
		// 		platform_instruction_log(opcode, "BIT 5, E");
		// 		bit_u3_r8(5, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x6C:
		// 		platform_instruction_log(opcode, "BIT 5, H");
		// 		bit_u3_r8(5, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x6D:
		// 		platform_instruction_log(opcode, "BIT 5, L");
		// 		bit_u3_r8(5, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x6E:
		// 		platform_instruction_log(opcode, "BIT 5, [HL]");
		// 		bit_u3_aHL(5);
		// 		break;
		// 	case 0x6F:
		// 		platform_instruction_log(opcode, "BIT 5, A");
		// 		bit_u3_r8(5, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x70:
		// 		platform_instruction_log(opcode, "BIT 6, B");
		// 		bit_u3_r8(6, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x71:
		// 		platform_instruction_log(opcode, "BIT 6, C");
		// 		bit_u3_r8(6, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x72:
		// 		platform_instruction_log(opcode, "BIT 6, D");
		// 		bit_u3_r8(6, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x73:
		// 		platform_instruction_log(opcode, "BIT 6, E");
		// 		bit_u3_r8(6, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x74:
		// 		platform_instruction_log(opcode, "BIT 6, H");
		// 		bit_u3_r8(6, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x75:
		// 		platform_instruction_log(opcode, "BIT 6, L");
		// 		bit_u3_r8(6, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x76:
		// 		platform_instruction_log(opcode, "BIT 6, [HL]");
		// 		bit_u3_aHL(6);
		// 		break;
		// 	case 0x77:
		// 		platform_instruction_log(opcode, "BIT 6, A");
		// 		bit_u3_r8(6, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x78:
		// 		platform_instruction_log(opcode, "BIT 7, B");
		// 		bit_u3_r8(7, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x79:
		// 		platform_instruction_log(opcode, "BIT 7, C");
		// 		bit_u3_r8(7, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x7A:
		// 		platform_instruction_log(opcode, "BIT 7, D");
		// 		bit_u3_r8(7, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x7B:
		// 		platform_instruction_log(opcode, "BIT 7, E");
		// 		bit_u3_r8(7, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x7C:
		// 		platform_instruction_log(opcode, "BIT 7, H");
		// 		bit_u3_r8(7, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x7D:
		// 		platform_instruction_log(opcode, "BIT 7, L");
		// 		bit_u3_r8(7, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x7E:
		// 		platform_instruction_log(opcode, "BIT 7, [HL]");
		// 		bit_u3_aHL(7);
		// 		break;
		// 	case 0x7F:
		// 		platform_instruction_log(opcode, "BIT 7, A");
		// 		bit_u3_r8(7, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x80:
		// 		platform_instruction_log(opcode, "RES 0, B");
		// 		res_u3_r8(0, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x81:
		// 		platform_instruction_log(opcode, "RES 0, C");
		// 		res_u3_r8(0, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x82:
		// 		platform_instruction_log(opcode, "RES 0, D");
		// 		res_u3_r8(0, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x83:
		// 		platform_instruction_log(opcode, "RES 0, E");
		// 		res_u3_r8(0, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x84:
		// 		platform_instruction_log(opcode, "RES 0, H");
		// 		res_u3_r8(0, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x85:
		// 		platform_instruction_log(opcode, "RES 0, L");
		// 		res_u3_r8(0, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x86:
		// 		platform_instruction_log(opcode, "RES 0, [HL]");
		// 		res_u3_aHL(0);
		// 		break;
		// 	case 0x87:
		// 		platform_instruction_log(opcode, "RES 0, A");
		// 		res_u3_r8(0, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x88:
		// 		platform_instruction_log(opcode, "RES 1, B");
		// 		res_u3_r8(1, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x89:
		// 		platform_instruction_log(opcode, "RES 1, C");
		// 		res_u3_r8(1, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x8A:
		// 		platform_instruction_log(opcode, "RES 1, D");
		// 		res_u3_r8(1, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x8B:
		// 		platform_instruction_log(opcode, "RES 1, E");
		// 		res_u3_r8(1, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x8C:
		// 		platform_instruction_log(opcode, "RES 1, H");
		// 		res_u3_r8(1, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x8D:
		// 		platform_instruction_log(opcode, "RES 1, L");
		// 		res_u3_r8(1, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x8E:
		// 		platform_instruction_log(opcode, "RES 1, [HL]");
		// 		res_u3_aHL(1);
		// 		break;
		// 	case 0x8F:
		// 		platform_instruction_log(opcode, "RES 1, A");
		// 		res_u3_r8(1, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x90:
		// 		platform_instruction_log(opcode, "RES 2, B");
		// 		res_u3_r8(2, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x91:
		// 		platform_instruction_log(opcode, "RES 2, C");
		// 		res_u3_r8(2, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x92:
		// 		platform_instruction_log(opcode, "RES 2, D");
		// 		res_u3_r8(2, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x93:
		// 		platform_instruction_log(opcode, "RES 2, E");
		// 		res_u3_r8(2, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x94:
		// 		platform_instruction_log(opcode, "RES 2, H");
		// 		res_u3_r8(2, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x95:
		// 		platform_instruction_log(opcode, "RES 2, L");
		// 		res_u3_r8(2, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x96:
		// 		platform_instruction_log(opcode, "RES 2, [HL]");
		// 		res_u3_aHL(2);
		// 		break;
		// 	case 0x97:
		// 		platform_instruction_log(opcode, "RES 2, A");
		// 		res_u3_r8(2, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x98:
		// 		platform_instruction_log(opcode, "RES 3, B");
		// 		res_u3_r8(3, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x99:
		// 		platform_instruction_log(opcode, "RES 3, C");
		// 		res_u3_r8(3, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x9A:
		// 		platform_instruction_log(opcode, "RES 3, D");
		// 		res_u3_r8(3, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x9B:
		// 		platform_instruction_log(opcode, "RES 3, E");
		// 		res_u3_r8(3, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x9C:
		// 		platform_instruction_log(opcode, "RES 3, H");
		// 		res_u3_r8(3, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x9D:
		// 		platform_instruction_log(opcode, "RES 3, L");
		// 		res_u3_r8(3, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x9E:
		// 		platform_instruction_log(opcode, "RES 3, [HL]");
		// 		res_u3_aHL(3);
		// 		break;
		// 	case 0x9F:
		// 		platform_instruction_log(opcode, "RES 3, A");
		// 		res_u3_r8(3, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xA0:
		// 		platform_instruction_log(opcode, "RES 4, B");
		// 		res_u3_r8(4, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xA1:
		// 		platform_instruction_log(opcode, "RES 4, C");
		// 		res_u3_r8(4, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xA2:
		// 		platform_instruction_log(opcode, "RES 4, D");
		// 		res_u3_r8(4, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xA3:
		// 		platform_instruction_log(opcode, "RES 4, E");
		// 		res_u3_r8(4, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xA4:
		// 		platform_instruction_log(opcode, "RES 4, H");
		// 		res_u3_r8(4, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xA5:
		// 		platform_instruction_log(opcode, "RES 4, L");
		// 		res_u3_r8(4, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xA6:
		// 		platform_instruction_log(opcode, "RES 4, [HL]");
		// 		res_u3_aHL(4);
		// 		break;
		// 	case 0xA7:
		// 		platform_instruction_log(opcode, "RES 4, A");
		// 		res_u3_r8(4, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xA8:
		// 		platform_instruction_log(opcode, "RES 5, B");
		// 		res_u3_r8(5, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xA9:
		// 		platform_instruction_log(opcode, "RES 5, C");
		// 		res_u3_r8(5, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xAA:
		// 		platform_instruction_log(opcode, "RES 5, D");
		// 		res_u3_r8(5, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xAB:
		// 		platform_instruction_log(opcode, "RES 5, E");
		// 		res_u3_r8(5, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xAC:
		// 		platform_instruction_log(opcode, "RES 5, H");
		// 		res_u3_r8(5, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xAD:
		// 		platform_instruction_log(opcode, "RES 5, L");
		// 		res_u3_r8(5, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xAE:
		// 		platform_instruction_log(opcode, "RES 5, [HL]");
		// 		res_u3_aHL(5);
		// 		break;
		// 	case 0xAF:
		// 		platform_instruction_log(opcode, "RES 5, A");
		// 		res_u3_r8(5, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xB0:
		// 		platform_instruction_log(opcode, "RES 6, B");
		// 		res_u3_r8(6, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xB1:
		// 		platform_instruction_log(opcode, "RES 6, C");
		// 		res_u3_r8(6, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xB2:
		// 		platform_instruction_log(opcode, "RES 6, D");
		// 		res_u3_r8(6, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xB3:
		// 		platform_instruction_log(opcode, "RES 6, E");
		// 		res_u3_r8(6, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xB4:
		// 		platform_instruction_log(opcode, "RES 6, H");
		// 		res_u3_r8(6, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xB5:
		// 		platform_instruction_log(opcode, "RES 6, L");
		// 		res_u3_r8(6, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xB6:
		// 		platform_instruction_log(opcode, "RES 6, [HL]");
		// 		res_u3_aHL(6);
		// 		break;
		// 	case 0xB7:
		// 		platform_instruction_log(opcode, "RES 6, A");
		// 		res_u3_r8(6, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xB8:
		// 		platform_instruction_log(opcode, "RES 7, B");
		// 		res_u3_r8(7, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xB9:
		// 		platform_instruction_log(opcode, "RES 7, C");
		// 		res_u3_r8(7, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xBA:
		// 		platform_instruction_log(opcode, "RES 7, D");
		// 		res_u3_r8(7, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xBB:
		// 		platform_instruction_log(opcode, "RES 7, E");
		// 		res_u3_r8(7, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xBC:
		// 		platform_instruction_log(opcode, "RES 7, H");
		// 		res_u3_r8(7, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xBD:
		// 		platform_instruction_log(opcode, "RES 7, L");
		// 		res_u3_r8(7, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xBE:
		// 		platform_instruction_log(opcode, "RES 7, [HL]");
		// 		res_u3_aHL(7);
		// 		break;
		// 	case 0xBF:
		// 		platform_instruction_log(opcode, "RES 7, A");
		// 		res_u3_r8(7, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xC0:
		// 		platform_instruction_log(opcode, "SET 0, B");
		// 		set_u3_r8(0, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xC1:
		// 		platform_instruction_log(opcode, "SET 0, C");
		// 		set_u3_r8(0, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xC2:
		// 		platform_instruction_log(opcode, "SET 0, D");
		// 		set_u3_r8(0, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xC3:
		// 		platform_instruction_log(opcode, "SET 0, E");
		// 		set_u3_r8(0, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xC4:
		// 		platform_instruction_log(opcode, "SET 0, H");
		// 		set_u3_r8(0, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xC5:
		// 		platform_instruction_log(opcode, "SET 0, L");
		// 		set_u3_r8(0, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xC6:
		// 		platform_instruction_log(opcode, "SET 0, [HL]");
		// 		set_u3_aHL(0);
		// 		break;
		// 	case 0xC7:
		// 		platform_instruction_log(opcode, "SET 0, A");
		// 		set_u3_r8(0, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xC8:
		// 		platform_instruction_log(opcode, "SET 1, B");
		// 		set_u3_r8(1, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xC9:
		// 		platform_instruction_log(opcode, "SET 1, C");
		// 		set_u3_r8(1, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xCA:
		// 		platform_instruction_log(opcode, "SET 1, D");
		// 		set_u3_r8(1, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xCB:
		// 		platform_instruction_log(opcode, "SET 1, E");
		// 		set_u3_r8(1, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xCC:
		// 		platform_instruction_log(opcode, "SET 1, H");
		// 		set_u3_r8(1, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xCD:
		// 		platform_instruction_log(opcode, "SET 1, L");
		// 		set_u3_r8(1, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xCE:
		// 		platform_instruction_log(opcode, "SET 1, [HL]");
		// 		set_u3_aHL(1);
		// 		break;
		// 	case 0xCF:
		// 		platform_instruction_log(opcode, "SET 1, A");
		// 		set_u3_r8(1, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xD0:
		// 		platform_instruction_log(opcode, "SET 2, B");
		// 		set_u3_r8(2, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xD1:
		// 		platform_instruction_log(opcode, "SET 2, C");
		// 		set_u3_r8(2, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xD2:
		// 		platform_instruction_log(opcode, "SET 2, D");
		// 		set_u3_r8(2, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xD3:
		// 		platform_instruction_log(opcode, "SET 2, E");
		// 		set_u3_r8(2, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xD4:
		// 		platform_instruction_log(opcode, "SET 2, H");
		// 		set_u3_r8(2, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xD5:
		// 		platform_instruction_log(opcode, "SET 2, L");
		// 		set_u3_r8(2, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xD6:
		// 		platform_instruction_log(opcode, "SET 2, [HL]");
		// 		set_u3_aHL(2);
		// 		break;
		// 	case 0xD7:
		// 		platform_instruction_log(opcode, "SET 2, A");
		// 		set_u3_r8(2, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xD8:
		// 		platform_instruction_log(opcode, "SET 3, B");
		// 		set_u3_r8(3, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xD9:
		// 		platform_instruction_log(opcode, "SET 3, C");
		// 		set_u3_r8(3, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xDA:
		// 		platform_instruction_log(opcode, "SET 3, D");
		// 		set_u3_r8(3, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xDB:
		// 		platform_instruction_log(opcode, "SET 3, E");
		// 		set_u3_r8(3, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xDC:
		// 		platform_instruction_log(opcode, "SET 3, H");
		// 		set_u3_r8(3, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xDD:
		// 		platform_instruction_log(opcode, "SET 3, L");
		// 		set_u3_r8(3, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xDE:
		// 		platform_instruction_log(opcode, "SET 3, [HL]");
		// 		set_u3_aHL(3);
		// 		break;
		// 	case 0xDF:
		// 		platform_instruction_log(opcode, "SET 3, A");
		// 		set_u3_r8(3, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xE0:
		// 		platform_instruction_log(opcode, "SET 4, B");
		// 		set_u3_r8(4, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xE1:
		// 		platform_instruction_log(opcode, "SET 4, C");
		// 		set_u3_r8(4, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xE2:
		// 		platform_instruction_log(opcode, "SET 4, D");
		// 		set_u3_r8(4, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xE3:
		// 		platform_instruction_log(opcode, "SET 4, E");
		// 		set_u3_r8(4, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xE4:
		// 		platform_instruction_log(opcode, "SET 4, H");
		// 		set_u3_r8(4, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xE5:
		// 		platform_instruction_log(opcode, "SET 4, L");
		// 		set_u3_r8(4, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xE6:
		// 		platform_instruction_log(opcode, "SET 4, [HL]");
		// 		set_u3_aHL(4);
		// 		break;
		// 	case 0xE7:
		// 		platform_instruction_log(opcode, "SET 4, A");
		// 		set_u3_r8(4, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xE8:
		// 		platform_instruction_log(opcode, "SET 5, B");
		// 		set_u3_r8(5, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xE9:
		// 		platform_instruction_log(opcode, "SET 5, C");
		// 		set_u3_r8(5, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xEA:
		// 		platform_instruction_log(opcode, "SET 5, D");
		// 		set_u3_r8(5, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xEB:
		// 		platform_instruction_log(opcode, "SET 5, E");
		// 		set_u3_r8(5, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xEC:
		// 		platform_instruction_log(opcode, "SET 5, H");
		// 		set_u3_r8(5, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xED:
		// 		platform_instruction_log(opcode, "SET 5, L");
		// 		set_u3_r8(5, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xEE:
		// 		platform_instruction_log(opcode, "SET 5, [HL]");
		// 		set_u3_aHL(5);
		// 		break;
		// 	case 0xEF:
		// 		platform_instruction_log(opcode, "SET 5, A");
		// 		set_u3_r8(5, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xF0:
		// 		platform_instruction_log(opcode, "SET 6, B");
		// 		set_u3_r8(6, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xF1:
		// 		platform_instruction_log(opcode, "SET 6, C");
		// 		set_u3_r8(6, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xF2:
		// 		platform_instruction_log(opcode, "SET 6, D");
		// 		set_u3_r8(6, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xF3:
		// 		platform_instruction_log(opcode, "SET 6, E");
		// 		set_u3_r8(6, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xF4:
		// 		platform_instruction_log(opcode, "SET 6, H");
		// 		set_u3_r8(6, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xF5:
		// 		platform_instruction_log(opcode, "SET 6, L");
		// 		set_u3_r8(6, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xF6:
		// 		platform_instruction_log(opcode, "SET 6, [HL]");
		// 		set_u3_aHL(6);
		// 		break;
		// 	case 0xF7:
		// 		platform_instruction_log(opcode, "SET 6, A");
		// 		set_u3_r8(6, &gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xF8:
		// 		platform_instruction_log(opcode, "SET 7, B");
		// 		set_u3_r8(7, &gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xF9:
		// 		platform_instruction_log(opcode, "SET 7, C");
		// 		set_u3_r8(7, &gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xFA:
		// 		platform_instruction_log(opcode, "SET 7, D");
		// 		set_u3_r8(7, &gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xFB:
		// 		platform_instruction_log(opcode, "SET 7, E");
		// 		set_u3_r8(7, &gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xFC:
		// 		platform_instruction_log(opcode, "SET 7, H");
		// 		set_u3_r8(7, &gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xFD:
		// 		platform_instruction_log(opcode, "SET 7, L");
		// 		set_u3_r8(7, &gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xFE:
		// 		platform_instruction_log(opcode, "SET 7, [HL]");
		// 		set_u3_aHL(7);
		// 		break;
		// 	case 0xFF:
		// 		platform_instruction_log(opcode, "SET 7, A");
		// 		set_u3_r8(7, &gb->cpu.regs[AF].high);
		// 		break;
		// 	}
		// 	gb->cpu.prefix = false;
	} else {
		// 	// ==================== NON-PREFIX ====================
		switch (opcode) {
		case 0x00:
			if (debug) platform_instruction_log(gb, opcode, "NOP");
			nop(gb);
			break;
		// 	case 0x01: {
		// 		platform_instruction_log(opcode, "LD BC, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		ld_r16_n16(&gb->cpu.regs[BC].full, n16);
		// 	} break;
		// 	case 0x02:
		// 		platform_instruction_log(opcode, "LD [BC], A");
		// 		ld_a16_A(gb->cpu.regs[BC].full);
		// 		break;
		// 	case 0x03:
		// 		platform_instruction_log(opcode, "INC BC");
		// 		inc_r16(&gb->cpu.regs[BC].full);
		// 		break;
		// 	case 0x04:
		// 		platform_instruction_log(opcode, "INC B");
		// 		inc_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x05:
		// 		platform_instruction_log(opcode, "DEC B");
		// 		dec_r8(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x06: {
		// 		platform_instruction_log(opcode, "LD B, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ld_r8_n8(&gb->cpu.regs[BC].high, n8);
		// 	} break;
		// 	case 0x07:
		// 		platform_instruction_log(opcode, "RLCA");
		// 		rlca();
		// 		break;
		// 	case 0x08: {
		// 		platform_instruction_log(opcode, "LD [n16], SP");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		ld_addr16_SP(n16);
		// 	} break;
		// 	case 0x09:
		// 		platform_instruction_log(opcode, "ADD HL, BC");
		// 		add_HL_r16(gb->cpu.regs[BC].full);
		// 		break;
		// 	case 0x0A:
		// 		platform_instruction_log(opcode, "LD A, [BC]");
		// 		ld_A_a16(gb->cpu.regs[BC].full);
		// 		break;
		// 	case 0x0B:
		// 		platform_instruction_log(opcode, "DEC BC");
		// 		dec_r16(&gb->cpu.regs[BC].full);
		// 		break;
		// 	case 0x0C:
		// 		platform_instruction_log(opcode, "INC C");
		// 		inc_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x0D:
		// 		platform_instruction_log(opcode, "DEC C");
		// 		dec_r8(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x0E: {
		// 		platform_instruction_log(opcode, "LD C, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ld_r8_n8(&gb->cpu.regs[BC].low, n8);
		// 	} break;
		// 	case 0x0F:
		// 		platform_instruction_log(opcode, "RRCA");
		// 		rrca();
		// 		break;
		// 	case 0x10: {
		// 		platform_instruction_log(opcode, "STOP n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		stop_n8(n8);
		// 	} break;
		// 	case 0x11: {
		// 		platform_instruction_log(opcode, "LD DE, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		ld_r16_n16(&gb->cpu.regs[DE].full, n16);
		// 	} break;
		// 	case 0x12:
		// 		platform_instruction_log(opcode, "LD [DE], A");
		// 		ld_a16_A(gb->cpu.regs[DE].full);
		// 		break;
		// 	case 0x13:
		// 		platform_instruction_log(opcode, "INC DE");
		// 		inc_r16(&gb->cpu.regs[DE].full);
		// 		break;
		// 	case 0x14:
		// 		platform_instruction_log(opcode, "INC D");
		// 		inc_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x15:
		// 		platform_instruction_log(opcode, "DEC D");
		// 		dec_r8(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x16: {
		// 		platform_instruction_log(opcode, "LD D, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ld_r8_n8(&gb->cpu.regs[DE].high, n8);
		// 	} break;
		// 	case 0x17:
		// 		platform_instruction_log(opcode, "RLA");
		// 		rla();
		// 		break;
		// 	case 0x18: {
		// 		platform_instruction_log(opcode, "JR n16");
		// 		i8 offset = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		jr_n16(offset);
		// 		break;
		// 	}
		// 	case 0x19:
		// 		platform_instruction_log(opcode, "ADD HL, DE");
		// 		add_HL_r16(gb->cpu.regs[DE].full);
		// 		break;
		// 	case 0x1A:
		// 		platform_instruction_log(opcode, "LD A, [DE]");
		// 		ld_A_a16(gb->cpu.regs[DE].full);
		// 		break;
		// 	case 0x1B:
		// 		platform_instruction_log(opcode, "DEC DE");
		// 		dec_r16(&gb->cpu.regs[DE].full);
		// 		break;
		// 	case 0x1C:
		// 		platform_instruction_log(opcode, "INC E");
		// 		inc_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x1D:
		// 		platform_instruction_log(opcode, "DEC E");
		// 		dec_r8(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x1E: {
		// 		platform_instruction_log(opcode, "LD E, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ld_r8_n8(&gb->cpu.regs[DE].low, n8);
		// 	} break;
		// 	case 0x1F:
		// 		platform_instruction_log(opcode, "RRA");
		// 		rra();
		// 		break;
		// 	case 0x20: {
		// 		platform_instruction_log(opcode, "JR NZ, n16");
		// 		i8 offset = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		jr_cc_n16(Z, false, offset);
		// 		break;
		// 	}
		// 	case 0x21: {
		// 		platform_instruction_log(opcode, "LD HL, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		ld_r16_n16(&gb->cpu.regs[HL].full, n16);
		// 	} break;
		// 	case 0x22:
		// 		platform_instruction_log(opcode, "LD [HL+], A");
		// 		ld_aHLi_A();
		// 		break;
		// 	case 0x23:
		// 		platform_instruction_log(opcode, "INC HL");
		// 		inc_r16(&gb->cpu.regs[HL].full);
		// 		break;
		// 	case 0x24:
		// 		platform_instruction_log(opcode, "INC H");
		// 		inc_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x25:
		// 		platform_instruction_log(opcode, "DEC H");
		// 		dec_r8(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x26: {
		// 		platform_instruction_log(opcode, "LD H, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ld_r8_n8(&gb->cpu.regs[HL].high, n8);
		// 	} break;
		// 	case 0x27:
		// 		platform_instruction_log(opcode, "DAA");
		// 		daa();
		// 		break;
		// 	case 0x28: {
		// 		platform_instruction_log(opcode, "JR Z, n16");
		// 		i8 offset = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		jr_cc_n16(Z, true, offset);
		// 		break;
		// 	}
		// 	case 0x29:
		// 		platform_instruction_log(opcode, "ADD HL, HL");
		// 		add_HL_r16(gb->cpu.regs[HL].full);
		// 		break;
		// 	case 0x2A:
		// 		platform_instruction_log(opcode, "LD A, [HL+]");
		// 		ld_A_aHLi();
		// 		break;
		// 	case 0x2B:
		// 		platform_instruction_log(opcode, "DEC HL");
		// 		dec_r16(&gb->cpu.regs[HL].full);
		// 		break;
		// 	case 0x2C:
		// 		platform_instruction_log(opcode, "INC L");
		// 		inc_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x2D:
		// 		platform_instruction_log(opcode, "DEC L");
		// 		dec_r8(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x2E: {
		// 		platform_instruction_log(opcode, "LD L, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ld_r8_n8(&gb->cpu.regs[HL].low, n8);
		// 	} break;
		// 	case 0x2F:
		// 		platform_instruction_log(opcode, "CPL");
		// 		cpl();
		// 		break;
		// 	case 0x30: {
		// 		platform_instruction_log(opcode, "JR NC, n16");
		// 		i8 offset = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		jr_cc_n16(C, false, offset);
		// 		break;
		// 	}
		// 	case 0x31: {
		// 		platform_instruction_log(opcode, "LD SP, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		ld_r16_n16(&gb->cpu.regs[SP].full, n16);
		// 	} break;
		// 	case 0x32:
		// 		platform_instruction_log(opcode, "LD [HL-], A");
		// 		ld_aHLd_A();
		// 		break;
		// 	case 0x33:
		// 		platform_instruction_log(opcode, "INC SP");
		// 		inc_r16(&gb->cpu.regs[SP].full);
		// 		break;
		// 	case 0x34:
		// 		platform_instruction_log(opcode, "INC [HL]");
		// 		inc_aHL();
		// 		break;
		// 	case 0x35:
		// 		platform_instruction_log(opcode, "DEC [HL]");
		// 		dec_aHL();
		// 		break;
		// 	case 0x36: {
		// 		platform_instruction_log(opcode, "LD [HL], n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ld_aHL_n8(n8);
		// 	} break;
		// 	case 0x37:
		// 		platform_instruction_log(opcode, "SCF");
		// 		scf();
		// 		break;
		// 	case 0x38: {
		// 		platform_instruction_log(opcode, "JR C, n16");
		// 		i8 offset = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		jr_cc_n16(C, true, offset);
		// 		break;
		// 	}
		// 	case 0x39:
		// 		platform_instruction_log(opcode, "ADD HL, SP");
		// 		add_HL_r16(gb->cpu.regs[SP].full);
		// 		break;
		// 	case 0x3A:
		// 		platform_instruction_log(opcode, "LD A, [HL-]");
		// 		ld_A_aHLd();
		// 		break;
		// 	case 0x3B:
		// 		platform_instruction_log(opcode, "DEC SP");
		// 		dec_r16(&gb->cpu.regs[SP].full);
		// 		break;
		// 	case 0x3C:
		// 		platform_instruction_log(opcode, "INC A");
		// 		inc_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x3D:
		// 		platform_instruction_log(opcode, "DEC A");
		// 		dec_r8(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x3E: {
		// 		platform_instruction_log(opcode, "LD A, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ld_r8_n8(&gb->cpu.regs[AF].high, n8);
		// 	} break;
		// 	case 0x3F:
		// 		platform_instruction_log(opcode, "CCF");
		// 		ccf();
		// 		break;
		// 	case 0x40:
		// 		platform_instruction_log(opcode, "LD B, B");
		// 		ld_r8_r8(&gb->cpu.regs[BC].high, gb->cpu.regs[BC].high);
		// 		if (stop_on_ld_b_b)
		// 			running = false;
		// 		break;
		// 	case 0x41:
		// 		platform_instruction_log(opcode, "LD B, C");
		// 		ld_r8_r8(&gb->cpu.regs[BC].high, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x42:
		// 		platform_instruction_log(opcode, "LD B, D");
		// 		ld_r8_r8(&gb->cpu.regs[BC].high, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x43:
		// 		platform_instruction_log(opcode, "LD B, E");
		// 		ld_r8_r8(&gb->cpu.regs[BC].high, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x44:
		// 		platform_instruction_log(opcode, "LD B, H");
		// 		ld_r8_r8(&gb->cpu.regs[BC].high, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x45:
		// 		platform_instruction_log(opcode, "LD B, L");
		// 		ld_r8_r8(&gb->cpu.regs[BC].high, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x46:
		// 		platform_instruction_log(opcode, "LD B, [HL]");
		// 		ld_r8_aHL(&gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x47:
		// 		platform_instruction_log(opcode, "LD B, A");
		// 		ld_r8_r8(&gb->cpu.regs[BC].high, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x48:
		// 		platform_instruction_log(opcode, "LD C, B");
		// 		ld_r8_r8(&gb->cpu.regs[BC].low, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x49:
		// 		platform_instruction_log(opcode, "LD C, C");
		// 		ld_r8_r8(&gb->cpu.regs[BC].low, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x4A:
		// 		platform_instruction_log(opcode, "LD C, D");
		// 		ld_r8_r8(&gb->cpu.regs[BC].low, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x4B:
		// 		platform_instruction_log(opcode, "LD C, E");
		// 		ld_r8_r8(&gb->cpu.regs[BC].low, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x4C:
		// 		platform_instruction_log(opcode, "LD C, H");
		// 		ld_r8_r8(&gb->cpu.regs[BC].low, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x4D:
		// 		platform_instruction_log(opcode, "LD C, L");
		// 		ld_r8_r8(&gb->cpu.regs[BC].low, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x4E:
		// 		platform_instruction_log(opcode, "LD C, [HL]");
		// 		ld_r8_aHL(&gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x4F:
		// 		platform_instruction_log(opcode, "LD C, A");
		// 		ld_r8_r8(&gb->cpu.regs[BC].low, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x50:
		// 		platform_instruction_log(opcode, "LD D, B");
		// 		ld_r8_r8(&gb->cpu.regs[DE].high, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x51:
		// 		platform_instruction_log(opcode, "LD D, C");
		// 		ld_r8_r8(&gb->cpu.regs[DE].high, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x52:
		// 		platform_instruction_log(opcode, "LD D, D");
		// 		ld_r8_r8(&gb->cpu.regs[DE].high, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x53:
		// 		platform_instruction_log(opcode, "LD D, E");
		// 		ld_r8_r8(&gb->cpu.regs[DE].high, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x54:
		// 		platform_instruction_log(opcode, "LD D, H");
		// 		ld_r8_r8(&gb->cpu.regs[DE].high, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x55:
		// 		platform_instruction_log(opcode, "LD D, L");
		// 		ld_r8_r8(&gb->cpu.regs[DE].high, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x56:
		// 		platform_instruction_log(opcode, "LD D, [HL]");
		// 		ld_r8_aHL(&gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x57:
		// 		platform_instruction_log(opcode, "LD D, A");
		// 		ld_r8_r8(&gb->cpu.regs[DE].high, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x58:
		// 		platform_instruction_log(opcode, "LD E, B");
		// 		ld_r8_r8(&gb->cpu.regs[DE].low, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x59:
		// 		platform_instruction_log(opcode, "LD E, C");
		// 		ld_r8_r8(&gb->cpu.regs[DE].low, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x5A:
		// 		platform_instruction_log(opcode, "LD E, D");
		// 		ld_r8_r8(&gb->cpu.regs[DE].low, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x5B:
		// 		platform_instruction_log(opcode, "LD E, E");
		// 		ld_r8_r8(&gb->cpu.regs[DE].low, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x5C:
		// 		platform_instruction_log(opcode, "LD E, H");
		// 		ld_r8_r8(&gb->cpu.regs[DE].low, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x5D:
		// 		platform_instruction_log(opcode, "LD E, L");
		// 		ld_r8_r8(&gb->cpu.regs[DE].low, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x5E:
		// 		platform_instruction_log(opcode, "LD E, [HL]");
		// 		ld_r8_aHL(&gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x5F:
		// 		platform_instruction_log(opcode, "LD E, A");
		// 		ld_r8_r8(&gb->cpu.regs[DE].low, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x60:
		// 		platform_instruction_log(opcode, "LD H, B");
		// 		ld_r8_r8(&gb->cpu.regs[HL].high, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x61:
		// 		platform_instruction_log(opcode, "LD H, C");
		// 		ld_r8_r8(&gb->cpu.regs[HL].high, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x62:
		// 		platform_instruction_log(opcode, "LD H, D");
		// 		ld_r8_r8(&gb->cpu.regs[HL].high, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x63:
		// 		platform_instruction_log(opcode, "LD H, E");
		// 		ld_r8_r8(&gb->cpu.regs[HL].high, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x64:
		// 		platform_instruction_log(opcode, "LD H, H");
		// 		ld_r8_r8(&gb->cpu.regs[HL].high, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x65:
		// 		platform_instruction_log(opcode, "LD H, L");
		// 		ld_r8_r8(&gb->cpu.regs[HL].high, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x66:
		// 		platform_instruction_log(opcode, "LD H, [HL]");
		// 		ld_r8_aHL(&gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x67:
		// 		platform_instruction_log(opcode, "LD H, A");
		// 		ld_r8_r8(&gb->cpu.regs[HL].high, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x68:
		// 		platform_instruction_log(opcode, "LD L, B");
		// 		ld_r8_r8(&gb->cpu.regs[HL].low, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x69:
		// 		platform_instruction_log(opcode, "LD L, C");
		// 		ld_r8_r8(&gb->cpu.regs[HL].low, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x6A:
		// 		platform_instruction_log(opcode, "LD L, D");
		// 		ld_r8_r8(&gb->cpu.regs[HL].low, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x6B:
		// 		platform_instruction_log(opcode, "LD L, E");
		// 		ld_r8_r8(&gb->cpu.regs[HL].low, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x6C:
		// 		platform_instruction_log(opcode, "LD L, H");
		// 		ld_r8_r8(&gb->cpu.regs[HL].low, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x6D:
		// 		platform_instruction_log(opcode, "LD L, L");
		// 		ld_r8_r8(&gb->cpu.regs[HL].low, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x6E:
		// 		platform_instruction_log(opcode, "LD L, [HL]");
		// 		ld_r8_aHL(&gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x6F:
		// 		platform_instruction_log(opcode, "LD L, A");
		// 		ld_r8_r8(&gb->cpu.regs[HL].low, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x70:
		// 		platform_instruction_log(opcode, "LD [HL], B");
		// 		ld_aHL_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x71:
		// 		platform_instruction_log(opcode, "LD [HL], C");
		// 		ld_aHL_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x72:
		// 		platform_instruction_log(opcode, "LD [HL], D");
		// 		ld_aHL_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x73:
		// 		platform_instruction_log(opcode, "LD [HL], E");
		// 		ld_aHL_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x74:
		// 		platform_instruction_log(opcode, "LD [HL], H");
		// 		ld_aHL_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x75:
		// 		platform_instruction_log(opcode, "LD [HL], L");
		// 		ld_aHL_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x76:
		// 		platform_instruction_log(opcode, "HALT");
		// 		halt();
		// 		break;
		// 	case 0x77:
		// 		platform_instruction_log(opcode, "LD [HL], A");
		// 		ld_aHL_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x78:
		// 		platform_instruction_log(opcode, "LD A, B");
		// 		ld_r8_r8(&gb->cpu.regs[AF].high, gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x79:
		// 		platform_instruction_log(opcode, "LD A, C");
		// 		ld_r8_r8(&gb->cpu.regs[AF].high, gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x7A:
		// 		platform_instruction_log(opcode, "LD A, D");
		// 		ld_r8_r8(&gb->cpu.regs[AF].high, gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x7B:
		// 		platform_instruction_log(opcode, "LD A, E");
		// 		ld_r8_r8(&gb->cpu.regs[AF].high, gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x7C:
		// 		platform_instruction_log(opcode, "LD A, H");
		// 		ld_r8_r8(&gb->cpu.regs[AF].high, gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x7D:
		// 		platform_instruction_log(opcode, "LD A, L");
		// 		ld_r8_r8(&gb->cpu.regs[AF].high, gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x7E:
		// 		platform_instruction_log(opcode, "LD A, [HL]");
		// 		ld_r8_aHL(&gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x7F:
		// 		platform_instruction_log(opcode, "LD A, A");
		// 		ld_r8_r8(&gb->cpu.regs[AF].high, gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x80:
		// 		platform_instruction_log(opcode, "ADD A, B");
		// 		add_A_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x81:
		// 		platform_instruction_log(opcode, "ADD A, C");
		// 		add_A_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x82:
		// 		platform_instruction_log(opcode, "ADD A, D");
		// 		add_A_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x83:
		// 		platform_instruction_log(opcode, "ADD A, E");
		// 		add_A_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x84:
		// 		platform_instruction_log(opcode, "ADD A, H");
		// 		add_A_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x85:
		// 		platform_instruction_log(opcode, "ADD A, L");
		// 		add_A_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x86:
		// 		platform_instruction_log(opcode, "ADD A, [HL]");
		// 		add_A_aHL();
		// 		break;
		// 	case 0x87:
		// 		platform_instruction_log(opcode, "ADD A, A");
		// 		add_A_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x88:
		// 		platform_instruction_log(opcode, "ADC A, B");
		// 		adc_A_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x89:
		// 		platform_instruction_log(opcode, "ADC A, C");
		// 		adc_A_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x8A:
		// 		platform_instruction_log(opcode, "ADC A, D");
		// 		adc_A_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x8B:
		// 		platform_instruction_log(opcode, "ADC A, E");
		// 		adc_A_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x8C:
		// 		platform_instruction_log(opcode, "ADC A, H");
		// 		adc_A_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x8D:
		// 		platform_instruction_log(opcode, "ADC A, L");
		// 		adc_A_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x8E:
		// 		platform_instruction_log(opcode, "ADC A, [HL]");
		// 		adc_A_aHL();
		// 		break;
		// 	case 0x8F:
		// 		platform_instruction_log(opcode, "ADC A, A");
		// 		adc_A_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x90:
		// 		platform_instruction_log(opcode, "SUB A, B");
		// 		sub_A_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x91:
		// 		platform_instruction_log(opcode, "SUB A, C");
		// 		sub_A_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x92:
		// 		platform_instruction_log(opcode, "SUB A, D");
		// 		sub_A_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x93:
		// 		platform_instruction_log(opcode, "SUB A, E");
		// 		sub_A_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x94:
		// 		platform_instruction_log(opcode, "SUB A, H");
		// 		sub_A_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x95:
		// 		platform_instruction_log(opcode, "SUB A, L");
		// 		sub_A_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x96:
		// 		platform_instruction_log(opcode, "SUB A, [HL]");
		// 		sub_A_aHL();
		// 		break;
		// 	case 0x97:
		// 		platform_instruction_log(opcode, "SUB A, A");
		// 		sub_A_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0x98:
		// 		platform_instruction_log(opcode, "SBC A, B");
		// 		sbc_A_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0x99:
		// 		platform_instruction_log(opcode, "SBC A, C");
		// 		sbc_A_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0x9A:
		// 		platform_instruction_log(opcode, "SBC A, D");
		// 		sbc_A_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0x9B:
		// 		platform_instruction_log(opcode, "SBC A, E");
		// 		sbc_A_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0x9C:
		// 		platform_instruction_log(opcode, "SBC A, H");
		// 		sbc_A_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0x9D:
		// 		platform_instruction_log(opcode, "SBC A, L");
		// 		sbc_A_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0x9E:
		// 		platform_instruction_log(opcode, "SBC A, [HL]");
		// 		sbc_A_aHL();
		// 		break;
		// 	case 0x9F:
		// 		platform_instruction_log(opcode, "SBC A, A");
		// 		sbc_A_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xA0:
		// 		platform_instruction_log(opcode, "AND A, B");
		// 		and_A_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xA1:
		// 		platform_instruction_log(opcode, "AND A, C");
		// 		and_A_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xA2:
		// 		platform_instruction_log(opcode, "AND A, D");
		// 		and_A_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xA3:
		// 		platform_instruction_log(opcode, "AND A, E");
		// 		and_A_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xA4:
		// 		platform_instruction_log(opcode, "AND A, H");
		// 		and_A_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xA5:
		// 		platform_instruction_log(opcode, "AND A, L");
		// 		and_A_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xA6:
		// 		platform_instruction_log(opcode, "AND A, [HL]");
		// 		and_A_aHL();
		// 		break;
		// 	case 0xA7:
		// 		platform_instruction_log(opcode, "AND A, A");
		// 		and_A_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xA8:
		// 		platform_instruction_log(opcode, "XOR A, B");
		// 		xor_A_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xA9:
		// 		platform_instruction_log(opcode, "XOR A, C");
		// 		xor_A_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xAA:
		// 		platform_instruction_log(opcode, "XOR A, D");
		// 		xor_A_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xAB:
		// 		platform_instruction_log(opcode, "XOR A, E");
		// 		xor_A_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xAC:
		// 		platform_instruction_log(opcode, "XOR A, H");
		// 		xor_A_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xAD:
		// 		platform_instruction_log(opcode, "XOR A, L");
		// 		xor_A_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xAE:
		// 		platform_instruction_log(opcode, "XOR A, [HL]");
		// 		xor_A_aHL();
		// 		break;
		// 	case 0xAF:
		// 		platform_instruction_log(opcode, "XOR A, A");
		// 		xor_A_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xB0:
		// 		platform_instruction_log(opcode, "OR A, B");
		// 		or_A_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xB1:
		// 		platform_instruction_log(opcode, "OR A, C");
		// 		or_A_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xB2:
		// 		platform_instruction_log(opcode, "OR A, D");
		// 		or_A_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xB3:
		// 		platform_instruction_log(opcode, "OR A, E");
		// 		or_A_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xB4:
		// 		platform_instruction_log(opcode, "OR A, H");
		// 		or_A_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xB5:
		// 		platform_instruction_log(opcode, "OR A, L");
		// 		or_A_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xB6:
		// 		platform_instruction_log(opcode, "OR A, [HL]");
		// 		or_A_aHL();
		// 		break;
		// 	case 0xB7:
		// 		platform_instruction_log(opcode, "OR A, A");
		// 		or_A_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xB8:
		// 		platform_instruction_log(opcode, "CP A, B");
		// 		cp_A_r8(gb->cpu.regs[BC].high);
		// 		break;
		// 	case 0xB9:
		// 		platform_instruction_log(opcode, "CP A, C");
		// 		cp_A_r8(gb->cpu.regs[BC].low);
		// 		break;
		// 	case 0xBA:
		// 		platform_instruction_log(opcode, "CP A, D");
		// 		cp_A_r8(gb->cpu.regs[DE].high);
		// 		break;
		// 	case 0xBB:
		// 		platform_instruction_log(opcode, "CP A, E");
		// 		cp_A_r8(gb->cpu.regs[DE].low);
		// 		break;
		// 	case 0xBC:
		// 		platform_instruction_log(opcode, "CP A, H");
		// 		cp_A_r8(gb->cpu.regs[HL].high);
		// 		break;
		// 	case 0xBD:
		// 		platform_instruction_log(opcode, "CP A, L");
		// 		cp_A_r8(gb->cpu.regs[HL].low);
		// 		break;
		// 	case 0xBE:
		// 		platform_instruction_log(opcode, "CP A, [HL]");
		// 		cp_A_aHL();
		// 		break;
		// 	case 0xBF:
		// 		platform_instruction_log(opcode, "CP A, A");
		// 		cp_A_r8(gb->cpu.regs[AF].high);
		// 		break;
		// 	case 0xC0:
		// 		platform_instruction_log(opcode, "RET NZ");
		// 		ret_cc(Z, false);
		// 		break;
		// 	case 0xC1:
		// 		platform_instruction_log(opcode, "POP BC");
		// 		pop_r16(&gb->cpu.regs[BC].full);
		// 		break;
		// 	case 0xC2: {
		// 		platform_instruction_log(opcode, "JP NZ, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		jp_cc_n16(Z, false, n16);
		// 		break;
		// 	}
		// 	case 0xC3: {
		// 		platform_instruction_log(opcode, "JP n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		jp_n16(n16);
		// 		break;
		// 	}
		// 	case 0xC4: {
		// 		platform_instruction_log(opcode, "CALL NZ, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		call_cc_n16(Z, false, n16);
		// 		break;
		// 	}
		// 	case 0xC5:
		// 		platform_instruction_log(opcode, "PUSH BC");
		// 		push_r16(gb->cpu.regs[BC].full);
		// 		break;
		// 	case 0xC6: {
		// 		platform_instruction_log(opcode, "ADD A, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		add_A_n8(n8);
		// 		break;
		// 	}
		// 	case 0xC7:
		// 		platform_instruction_log(opcode, "RST $00");
		// 		rst(0x00);
		// 		break;
		// 	case 0xC8:
		// 		platform_instruction_log(opcode, "RET Z");
		// 		ret_cc(Z, true);
		// 		break;
		// 	case 0xC9:
		// 		platform_instruction_log(opcode, "RET");
		// 		ret();
		// 		break;
		// 	case 0xCA: {
		// 		platform_instruction_log(opcode, "JP Z, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		jp_cc_n16(Z, true, n16);
		// 		break;
		// 	}
		// 	case 0xCB:
		// 		platform_instruction_log(opcode, "PREFIX TOGGLE");
		// 		gb->cpu.prefix = true;
		// 		gb->cpu.regs[PC].full += 1;
		// 		break;
		// 	case 0xCC: {
		// 		platform_instruction_log(opcode, "CALL Z, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		call_cc_n16(Z, true, n16);
		// 		break;
		// 	}
		// 	case 0xCD: {
		// 		platform_instruction_log(opcode, "CALL n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		call_n16(n16);
		// 		break;
		// 	}
		// 	case 0xCE: {
		// 		platform_instruction_log(opcode, "ADC A, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		adc_A_n8(n8);
		// 		break;
		// 	}
		// 	case 0xCF:
		// 		platform_instruction_log(opcode, "RST $08");
		// 		rst(0x08);
		// 		break;
		// 	case 0xD0:
		// 		platform_instruction_log(opcode, "RET NC");
		// 		ret_cc(C, false);
		// 		break;
		// 	case 0xD1:
		// 		platform_instruction_log(opcode, "POP DE");
		// 		pop_r16(&gb->cpu.regs[DE].full);
		// 		break;
		// 	case 0xD2: {
		// 		platform_instruction_log(opcode, "JP NC, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		jp_cc_n16(C, false, n16);
		// 		break;
		// 	}
		// 	case 0xD4: {
		// 		platform_instruction_log(opcode, "CALL NC, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		call_cc_n16(C, false, n16);
		// 		break;
		// 	}
		// 	case 0xD5:
		// 		platform_instruction_log(opcode, "PUSH DE");
		// 		push_r16(gb->cpu.regs[DE].full);
		// 		break;
		// 	case 0xD6: {
		// 		platform_instruction_log(opcode, "SUB A, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		sub_A_n8(n8);
		// 		break;
		// 	}
		// 	case 0xD7:
		// 		platform_instruction_log(opcode, "RST $10");
		// 		rst(0x10);
		// 		break;
		// 	case 0xD8:
		// 		platform_instruction_log(opcode, "RET C");
		// 		ret_cc(C, true);
		// 		break;
		// 	case 0xD9:
		// 		platform_instruction_log(opcode, "RETI");
		// 		reti();
		// 		break;
		// 	case 0xDA: {
		// 		platform_instruction_log(opcode, "JP C, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		jp_cc_n16(C, true, n16);
		// 		break;
		// 	}
		// 	case 0xDC: {
		// 		platform_instruction_log(opcode, "CALL C, n16");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		call_cc_n16(C, true, n16);
		// 		break;
		// 	}
		// 	case 0xDE: {
		// 		platform_instruction_log(opcode, "SBC A, n8");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		sbc_A_n8(n8);
		// 		break;
		// 	}
		// 	case 0xDF:
		// 		platform_instruction_log(opcode, "RST $18");
		// 		rst(0x18);
		// 		break;
		// 	case 0xE0: {
		// 		platform_instruction_log(opcode, "LDH [n16], A");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ldh_addr16_A(0xFF00 + n8);
		// 		break;
		// 	}
		// 	case 0xE1:
		// 		platform_instruction_log(opcode, "POP HL");
		// 		pop_r16(&gb->cpu.regs[HL].full);
		// 		break;
		// 	case 0xE2:
		// 		platform_instruction_log(opcode, "LDH [C], A");
		// 		ldh_aC_A();
		// 		break;
		// 	case 0xE5:
		// 		platform_instruction_log(opcode, "PUSH HL");
		// 		push_r16(gb->cpu.regs[HL].full);
		// 		break;
		// 	case 0xE6: {
		// 		platform_instruction_log(opcode, "AND A, n8");
		// 		i8 n8 = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		and_A_n8(n8);
		// 	} break;
		// 	case 0xE7:
		// 		platform_instruction_log(opcode, "RST $20");
		// 		rst(0x20);
		// 		break;
		// 	case 0xE8: {
		// 		platform_instruction_log(opcode, "ADD SP, n8");
		// 		i8 n8 = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		add_SP_n8(n8);
		// 	} break;
		// 	case 0xE9:
		// 		platform_instruction_log(opcode, "JP HL");
		// 		jp_aHL();
		// 		break;
		// 	case 0xEA: {
		// 		platform_instruction_log(opcode, "LD [n16], A");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		ld_addr16_A(n16);
		// 		break;
		// 	}
		// 	case 0xEE: {
		// 		platform_instruction_log(opcode, "XOR A, n8");
		// 		i8 n8 = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		xor_A_n8(n8);
		// 	} break;
		// 	case 0xEF:
		// 		platform_instruction_log(opcode, "RST $28");
		// 		rst(0x28);
		// 		break;
		// 	case 0xF0: {
		// 		platform_instruction_log(opcode, "LDH A, [n16]");
		// 		u8 n8 = read8(gb->cpu.regs[PC].full + 1);
		// 		ldh_A_addr16(0xFF00 + n8);
		// 		break;
		// 	}
		// 	case 0xF1:
		// 		platform_instruction_log(opcode, "POP AF");
		// 		pop_AF();
		// 		break;
		// 	case 0xF2:
		// 		platform_instruction_log(opcode, "LDH A, [C]");
		// 		ldh_A_aC();
		// 		break;
		// 	case 0xF3:
		// 		platform_instruction_log(opcode, "DI");
		// 		di();
		// 		break;
		// 	case 0xF5:
		// 		platform_instruction_log(opcode, "PUSH AF");
		// 		push_AF();
		// 		break;
		// 	case 0xF6: {
		// 		platform_instruction_log(opcode, "OR A, n8");
		// 		i8 n8 = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		or_A_n8(n8);
		// 	} break;
		// 	case 0xF7:
		// 		platform_instruction_log(opcode, "RST $30");
		// 		rst(0x30);
		// 		break;
		// 	case 0xF8: {
		// 		platform_instruction_log(opcode, "LD HL, SP+n8");
		// 		i8 n8 = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		ld_HL_SPe8(n8);
		// 	} break;
		// 	case 0xF9:
		// 		platform_instruction_log(opcode, "LD SP, HL");
		// 		ld_SP_HL();
		// 		break;
		// 	case 0xFA: {
		// 		platform_instruction_log(opcode, "LD A, [n16]");
		// 		u16 n16 = read16(gb->cpu.regs[PC].full + 1);
		// 		ld_A_addr16(n16);
		// 		break;
		// 	}
		// 	case 0xFB:
		// 		platform_instruction_log(opcode, "EI");
		// 		ei();
		// 		break;
		// 	case 0xFE: {
		// 		platform_instruction_log(opcode, "CP A, n8");
		// 		i8 n8 = (i8)read8(gb->cpu.regs[PC].full + 1);
		// 		cp_A_n8(n8);
		// 	} break;
		// 	case 0xFF:
		// 		platform_instruction_log(opcode, "RST $38");
		// 		rst(0x38);
		// 		break;
		default:
			if (debug) platform_instruction_log(gb, opcode, "INVALID OPCODE");
			gb->cpu.regs[PC].full += 1;
			gb->cpu.cycle += 1;
			break;
		}
	}
}
