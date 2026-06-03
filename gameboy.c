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

// void process_opcode(const u8 opcode, GameboyState *state) {
// 	if (state->cpu.prefix) {
// 		// ==================== PREFIX ====================
// 		switch (opcode) {
// 		case 0x00:
// 			PlatformLogInstruction(byte, "RLC B");
// 			rlc_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x01:
// 			PlatformLogInstruction(byte, "RLC C");
// 			rlc_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x02:
// 			PlatformLogInstruction(byte, "RLC D");
// 			rlc_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x03:
// 			PlatformLogInstruction(byte, "RLC E");
// 			rlc_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x04:
// 			PlatformLogInstruction(byte, "RLC H");
// 			rlc_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x05:
// 			PlatformLogInstruction(byte, "RLC L");
// 			rlc_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x06:
// 			PlatformLogInstruction(byte, "RLC [HL]");
// 			rlc_aHL();
// 			break;
// 		case 0x07:
// 			PlatformLogInstruction(byte, "RLC A");
// 			rlc_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x08:
// 			PlatformLogInstruction(byte, "RRC B");
// 			rrc_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x09:
// 			PlatformLogInstruction(byte, "RRC C");
// 			rrc_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x0A:
// 			PlatformLogInstruction(byte, "RRC D");
// 			rrc_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x0B:
// 			PlatformLogInstruction(byte, "RRC E");
// 			rrc_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x0C:
// 			PlatformLogInstruction(byte, "RRC H");
// 			rrc_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x0D:
// 			PlatformLogInstruction(byte, "RRC L");
// 			rrc_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x0E:
// 			PlatformLogInstruction(byte, "RRC [HL]");
// 			rrc_aHL();
// 			break;
// 		case 0x0F:
// 			PlatformLogInstruction(byte, "RRC A");
// 			rrc_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x10:
// 			PlatformLogInstruction(byte, "RL B");
// 			rl_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x11:
// 			PlatformLogInstruction(byte, "RL C");
// 			rl_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x12:
// 			PlatformLogInstruction(byte, "RL D");
// 			rl_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x13:
// 			PlatformLogInstruction(byte, "RL E");
// 			rl_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x14:
// 			PlatformLogInstruction(byte, "RL H");
// 			rl_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x15:
// 			PlatformLogInstruction(byte, "RL L");
// 			rl_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x16:
// 			PlatformLogInstruction(byte, "RL [HL]");
// 			rl_aHL();
// 			break;
// 		case 0x17:
// 			PlatformLogInstruction(byte, "RL A");
// 			rl_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x18:
// 			PlatformLogInstruction(byte, "RR B");
// 			rr_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x19:
// 			PlatformLogInstruction(byte, "RR C");
// 			rr_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x1A:
// 			PlatformLogInstruction(byte, "RR D");
// 			rr_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x1B:
// 			PlatformLogInstruction(byte, "RR E");
// 			rr_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x1C:
// 			PlatformLogInstruction(byte, "RR H");
// 			rr_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x1D:
// 			PlatformLogInstruction(byte, "RR L");
// 			rr_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x1E:
// 			PlatformLogInstruction(byte, "RR [HL]");
// 			rr_aHL();
// 			break;
// 		case 0x1F:
// 			PlatformLogInstruction(byte, "RR A");
// 			rr_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x20:
// 			PlatformLogInstruction(byte, "SLA B");
// 			sla_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x21:
// 			PlatformLogInstruction(byte, "SLA C");
// 			sla_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x22:
// 			PlatformLogInstruction(byte, "SLA D");
// 			sla_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x23:
// 			PlatformLogInstruction(byte, "SLA E");
// 			sla_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x24:
// 			PlatformLogInstruction(byte, "SLA H");
// 			sla_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x25:
// 			PlatformLogInstruction(byte, "SLA L");
// 			sla_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x26:
// 			PlatformLogInstruction(byte, "SLA [HL]");
// 			sla_aHL();
// 			break;
// 		case 0x27:
// 			PlatformLogInstruction(byte, "SLA A");
// 			sla_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x28:
// 			PlatformLogInstruction(byte, "SRA B");
// 			sra_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x29:
// 			PlatformLogInstruction(byte, "SRA C");
// 			sra_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x2A:
// 			PlatformLogInstruction(byte, "SRA D");
// 			sra_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x2B:
// 			PlatformLogInstruction(byte, "SRA E");
// 			sra_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x2C:
// 			PlatformLogInstruction(byte, "SRA H");
// 			sra_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x2D:
// 			PlatformLogInstruction(byte, "SRA L");
// 			sra_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x2E:
// 			PlatformLogInstruction(byte, "SRA [HL]");
// 			sra_aHL();
// 			break;
// 		case 0x2F:
// 			PlatformLogInstruction(byte, "SRA A");
// 			sra_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x30:
// 			PlatformLogInstruction(byte, "SWAP B");
// 			swap_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x31:
// 			PlatformLogInstruction(byte, "SWAP C");
// 			swap_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x32:
// 			PlatformLogInstruction(byte, "SWAP D");
// 			swap_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x33:
// 			PlatformLogInstruction(byte, "SWAP E");
// 			swap_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x34:
// 			PlatformLogInstruction(byte, "SWAP H");
// 			swap_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x35:
// 			PlatformLogInstruction(byte, "SWAP L");
// 			swap_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x36:
// 			PlatformLogInstruction(byte, "SWAP [HL]");
// 			swap_aHL();
// 			break;
// 		case 0x37:
// 			PlatformLogInstruction(byte, "SWAP A");
// 			swap_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x38:
// 			PlatformLogInstruction(byte, "SRL B");
// 			srl_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x39:
// 			PlatformLogInstruction(byte, "SRL C");
// 			srl_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x3A:
// 			PlatformLogInstruction(byte, "SRL D");
// 			srl_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x3B:
// 			PlatformLogInstruction(byte, "SRL E");
// 			srl_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x3C:
// 			PlatformLogInstruction(byte, "SRL H");
// 			srl_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x3D:
// 			PlatformLogInstruction(byte, "SRL L");
// 			srl_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x3E:
// 			PlatformLogInstruction(byte, "SRL [HL]");
// 			srl_aHL();
// 			break;
// 		case 0x3F:
// 			PlatformLogInstruction(byte, "SRL A");
// 			srl_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x40:
// 			PlatformLogInstruction(byte, "BIT 0, B");
// 			bit_u3_r8(0, state->cpu.regs[BC].high);
// 			break;
// 		case 0x41:
// 			PlatformLogInstruction(byte, "BIT 0, C");
// 			bit_u3_r8(0, state->cpu.regs[BC].low);
// 			break;
// 		case 0x42:
// 			PlatformLogInstruction(byte, "BIT 0, D");
// 			bit_u3_r8(0, state->cpu.regs[DE].high);
// 			break;
// 		case 0x43:
// 			PlatformLogInstruction(byte, "BIT 0, E");
// 			bit_u3_r8(0, state->cpu.regs[DE].low);
// 			break;
// 		case 0x44:
// 			PlatformLogInstruction(byte, "BIT 0, H");
// 			bit_u3_r8(0, state->cpu.regs[HL].high);
// 			break;
// 		case 0x45:
// 			PlatformLogInstruction(byte, "BIT 0, L");
// 			bit_u3_r8(0, state->cpu.regs[HL].low);
// 			break;
// 		case 0x46:
// 			PlatformLogInstruction(byte, "BIT 0, [HL]");
// 			bit_u3_aHL(0);
// 			break;
// 		case 0x47:
// 			PlatformLogInstruction(byte, "BIT 0, A");
// 			bit_u3_r8(0, state->cpu.regs[AF].high);
// 			break;
// 		case 0x48:
// 			PlatformLogInstruction(byte, "BIT 1, B");
// 			bit_u3_r8(1, state->cpu.regs[BC].high);
// 			break;
// 		case 0x49:
// 			PlatformLogInstruction(byte, "BIT 1, C");
// 			bit_u3_r8(1, state->cpu.regs[BC].low);
// 			break;
// 		case 0x4A:
// 			PlatformLogInstruction(byte, "BIT 1, D");
// 			bit_u3_r8(1, state->cpu.regs[DE].high);
// 			break;
// 		case 0x4B:
// 			PlatformLogInstruction(byte, "BIT 1, E");
// 			bit_u3_r8(1, state->cpu.regs[DE].low);
// 			break;
// 		case 0x4C:
// 			PlatformLogInstruction(byte, "BIT 1, H");
// 			bit_u3_r8(1, state->cpu.regs[HL].high);
// 			break;
// 		case 0x4D:
// 			PlatformLogInstruction(byte, "BIT 1, L");
// 			bit_u3_r8(1, state->cpu.regs[HL].low);
// 			break;
// 		case 0x4E:
// 			PlatformLogInstruction(byte, "BIT 1, [HL]");
// 			bit_u3_aHL(1);
// 			break;
// 		case 0x4F:
// 			PlatformLogInstruction(byte, "BIT 1, A");
// 			bit_u3_r8(1, state->cpu.regs[AF].high);
// 			break;
// 		case 0x50:
// 			PlatformLogInstruction(byte, "BIT 2, B");
// 			bit_u3_r8(2, state->cpu.regs[BC].high);
// 			break;
// 		case 0x51:
// 			PlatformLogInstruction(byte, "BIT 2, C");
// 			bit_u3_r8(2, state->cpu.regs[BC].low);
// 			break;
// 		case 0x52:
// 			PlatformLogInstruction(byte, "BIT 2, D");
// 			bit_u3_r8(2, state->cpu.regs[DE].high);
// 			break;
// 		case 0x53:
// 			PlatformLogInstruction(byte, "BIT 2, E");
// 			bit_u3_r8(2, state->cpu.regs[DE].low);
// 			break;
// 		case 0x54:
// 			PlatformLogInstruction(byte, "BIT 2, H");
// 			bit_u3_r8(2, state->cpu.regs[HL].high);
// 			break;
// 		case 0x55:
// 			PlatformLogInstruction(byte, "BIT 2, L");
// 			bit_u3_r8(2, state->cpu.regs[HL].low);
// 			break;
// 		case 0x56:
// 			PlatformLogInstruction(byte, "BIT 2, [HL]");
// 			bit_u3_aHL(2);
// 			break;
// 		case 0x57:
// 			PlatformLogInstruction(byte, "BIT 2, A");
// 			bit_u3_r8(2, state->cpu.regs[AF].high);
// 			break;
// 		case 0x58:
// 			PlatformLogInstruction(byte, "BIT 3, B");
// 			bit_u3_r8(3, state->cpu.regs[BC].high);
// 			break;
// 		case 0x59:
// 			PlatformLogInstruction(byte, "BIT 3, C");
// 			bit_u3_r8(3, state->cpu.regs[BC].low);
// 			break;
// 		case 0x5A:
// 			PlatformLogInstruction(byte, "BIT 3, D");
// 			bit_u3_r8(3, state->cpu.regs[DE].high);
// 			break;
// 		case 0x5B:
// 			PlatformLogInstruction(byte, "BIT 3, E");
// 			bit_u3_r8(3, state->cpu.regs[DE].low);
// 			break;
// 		case 0x5C:
// 			PlatformLogInstruction(byte, "BIT 3, H");
// 			bit_u3_r8(3, state->cpu.regs[HL].high);
// 			break;
// 		case 0x5D:
// 			PlatformLogInstruction(byte, "BIT 3, L");
// 			bit_u3_r8(3, state->cpu.regs[HL].low);
// 			break;
// 		case 0x5E:
// 			PlatformLogInstruction(byte, "BIT 3, [HL]");
// 			bit_u3_aHL(3);
// 			break;
// 		case 0x5F:
// 			PlatformLogInstruction(byte, "BIT 3, A");
// 			bit_u3_r8(3, state->cpu.regs[AF].high);
// 			break;
// 		case 0x60:
// 			PlatformLogInstruction(byte, "BIT 4, B");
// 			bit_u3_r8(4, state->cpu.regs[BC].high);
// 			break;
// 		case 0x61:
// 			PlatformLogInstruction(byte, "BIT 4, C");
// 			bit_u3_r8(4, state->cpu.regs[BC].low);
// 			break;
// 		case 0x62:
// 			PlatformLogInstruction(byte, "BIT 4, D");
// 			bit_u3_r8(4, state->cpu.regs[DE].high);
// 			break;
// 		case 0x63:
// 			PlatformLogInstruction(byte, "BIT 4, E");
// 			bit_u3_r8(4, state->cpu.regs[DE].low);
// 			break;
// 		case 0x64:
// 			PlatformLogInstruction(byte, "BIT 4, H");
// 			bit_u3_r8(4, state->cpu.regs[HL].high);
// 			break;
// 		case 0x65:
// 			PlatformLogInstruction(byte, "BIT 4, L");
// 			bit_u3_r8(4, state->cpu.regs[HL].low);
// 			break;
// 		case 0x66:
// 			PlatformLogInstruction(byte, "BIT 4, [HL]");
// 			bit_u3_aHL(4);
// 			break;
// 		case 0x67:
// 			PlatformLogInstruction(byte, "BIT 4, A");
// 			bit_u3_r8(4, state->cpu.regs[AF].high);
// 			break;
// 		case 0x68:
// 			PlatformLogInstruction(byte, "BIT 5, B");
// 			bit_u3_r8(5, state->cpu.regs[BC].high);
// 			break;
// 		case 0x69:
// 			PlatformLogInstruction(byte, "BIT 5, C");
// 			bit_u3_r8(5, state->cpu.regs[BC].low);
// 			break;
// 		case 0x6A:
// 			PlatformLogInstruction(byte, "BIT 5, D");
// 			bit_u3_r8(5, state->cpu.regs[DE].high);
// 			break;
// 		case 0x6B:
// 			PlatformLogInstruction(byte, "BIT 5, E");
// 			bit_u3_r8(5, state->cpu.regs[DE].low);
// 			break;
// 		case 0x6C:
// 			PlatformLogInstruction(byte, "BIT 5, H");
// 			bit_u3_r8(5, state->cpu.regs[HL].high);
// 			break;
// 		case 0x6D:
// 			PlatformLogInstruction(byte, "BIT 5, L");
// 			bit_u3_r8(5, state->cpu.regs[HL].low);
// 			break;
// 		case 0x6E:
// 			PlatformLogInstruction(byte, "BIT 5, [HL]");
// 			bit_u3_aHL(5);
// 			break;
// 		case 0x6F:
// 			PlatformLogInstruction(byte, "BIT 5, A");
// 			bit_u3_r8(5, state->cpu.regs[AF].high);
// 			break;
// 		case 0x70:
// 			PlatformLogInstruction(byte, "BIT 6, B");
// 			bit_u3_r8(6, state->cpu.regs[BC].high);
// 			break;
// 		case 0x71:
// 			PlatformLogInstruction(byte, "BIT 6, C");
// 			bit_u3_r8(6, state->cpu.regs[BC].low);
// 			break;
// 		case 0x72:
// 			PlatformLogInstruction(byte, "BIT 6, D");
// 			bit_u3_r8(6, state->cpu.regs[DE].high);
// 			break;
// 		case 0x73:
// 			PlatformLogInstruction(byte, "BIT 6, E");
// 			bit_u3_r8(6, state->cpu.regs[DE].low);
// 			break;
// 		case 0x74:
// 			PlatformLogInstruction(byte, "BIT 6, H");
// 			bit_u3_r8(6, state->cpu.regs[HL].high);
// 			break;
// 		case 0x75:
// 			PlatformLogInstruction(byte, "BIT 6, L");
// 			bit_u3_r8(6, state->cpu.regs[HL].low);
// 			break;
// 		case 0x76:
// 			PlatformLogInstruction(byte, "BIT 6, [HL]");
// 			bit_u3_aHL(6);
// 			break;
// 		case 0x77:
// 			PlatformLogInstruction(byte, "BIT 6, A");
// 			bit_u3_r8(6, state->cpu.regs[AF].high);
// 			break;
// 		case 0x78:
// 			PlatformLogInstruction(byte, "BIT 7, B");
// 			bit_u3_r8(7, state->cpu.regs[BC].high);
// 			break;
// 		case 0x79:
// 			PlatformLogInstruction(byte, "BIT 7, C");
// 			bit_u3_r8(7, state->cpu.regs[BC].low);
// 			break;
// 		case 0x7A:
// 			PlatformLogInstruction(byte, "BIT 7, D");
// 			bit_u3_r8(7, state->cpu.regs[DE].high);
// 			break;
// 		case 0x7B:
// 			PlatformLogInstruction(byte, "BIT 7, E");
// 			bit_u3_r8(7, state->cpu.regs[DE].low);
// 			break;
// 		case 0x7C:
// 			PlatformLogInstruction(byte, "BIT 7, H");
// 			bit_u3_r8(7, state->cpu.regs[HL].high);
// 			break;
// 		case 0x7D:
// 			PlatformLogInstruction(byte, "BIT 7, L");
// 			bit_u3_r8(7, state->cpu.regs[HL].low);
// 			break;
// 		case 0x7E:
// 			PlatformLogInstruction(byte, "BIT 7, [HL]");
// 			bit_u3_aHL(7);
// 			break;
// 		case 0x7F:
// 			PlatformLogInstruction(byte, "BIT 7, A");
// 			bit_u3_r8(7, state->cpu.regs[AF].high);
// 			break;
// 		case 0x80:
// 			PlatformLogInstruction(byte, "RES 0, B");
// 			res_u3_r8(0, &state->cpu.regs[BC].high);
// 			break;
// 		case 0x81:
// 			PlatformLogInstruction(byte, "RES 0, C");
// 			res_u3_r8(0, &state->cpu.regs[BC].low);
// 			break;
// 		case 0x82:
// 			PlatformLogInstruction(byte, "RES 0, D");
// 			res_u3_r8(0, &state->cpu.regs[DE].high);
// 			break;
// 		case 0x83:
// 			PlatformLogInstruction(byte, "RES 0, E");
// 			res_u3_r8(0, &state->cpu.regs[DE].low);
// 			break;
// 		case 0x84:
// 			PlatformLogInstruction(byte, "RES 0, H");
// 			res_u3_r8(0, &state->cpu.regs[HL].high);
// 			break;
// 		case 0x85:
// 			PlatformLogInstruction(byte, "RES 0, L");
// 			res_u3_r8(0, &state->cpu.regs[HL].low);
// 			break;
// 		case 0x86:
// 			PlatformLogInstruction(byte, "RES 0, [HL]");
// 			res_u3_aHL(0);
// 			break;
// 		case 0x87:
// 			PlatformLogInstruction(byte, "RES 0, A");
// 			res_u3_r8(0, &state->cpu.regs[AF].high);
// 			break;
// 		case 0x88:
// 			PlatformLogInstruction(byte, "RES 1, B");
// 			res_u3_r8(1, &state->cpu.regs[BC].high);
// 			break;
// 		case 0x89:
// 			PlatformLogInstruction(byte, "RES 1, C");
// 			res_u3_r8(1, &state->cpu.regs[BC].low);
// 			break;
// 		case 0x8A:
// 			PlatformLogInstruction(byte, "RES 1, D");
// 			res_u3_r8(1, &state->cpu.regs[DE].high);
// 			break;
// 		case 0x8B:
// 			PlatformLogInstruction(byte, "RES 1, E");
// 			res_u3_r8(1, &state->cpu.regs[DE].low);
// 			break;
// 		case 0x8C:
// 			PlatformLogInstruction(byte, "RES 1, H");
// 			res_u3_r8(1, &state->cpu.regs[HL].high);
// 			break;
// 		case 0x8D:
// 			PlatformLogInstruction(byte, "RES 1, L");
// 			res_u3_r8(1, &state->cpu.regs[HL].low);
// 			break;
// 		case 0x8E:
// 			PlatformLogInstruction(byte, "RES 1, [HL]");
// 			res_u3_aHL(1);
// 			break;
// 		case 0x8F:
// 			PlatformLogInstruction(byte, "RES 1, A");
// 			res_u3_r8(1, &state->cpu.regs[AF].high);
// 			break;
// 		case 0x90:
// 			PlatformLogInstruction(byte, "RES 2, B");
// 			res_u3_r8(2, &state->cpu.regs[BC].high);
// 			break;
// 		case 0x91:
// 			PlatformLogInstruction(byte, "RES 2, C");
// 			res_u3_r8(2, &state->cpu.regs[BC].low);
// 			break;
// 		case 0x92:
// 			PlatformLogInstruction(byte, "RES 2, D");
// 			res_u3_r8(2, &state->cpu.regs[DE].high);
// 			break;
// 		case 0x93:
// 			PlatformLogInstruction(byte, "RES 2, E");
// 			res_u3_r8(2, &state->cpu.regs[DE].low);
// 			break;
// 		case 0x94:
// 			PlatformLogInstruction(byte, "RES 2, H");
// 			res_u3_r8(2, &state->cpu.regs[HL].high);
// 			break;
// 		case 0x95:
// 			PlatformLogInstruction(byte, "RES 2, L");
// 			res_u3_r8(2, &state->cpu.regs[HL].low);
// 			break;
// 		case 0x96:
// 			PlatformLogInstruction(byte, "RES 2, [HL]");
// 			res_u3_aHL(2);
// 			break;
// 		case 0x97:
// 			PlatformLogInstruction(byte, "RES 2, A");
// 			res_u3_r8(2, &state->cpu.regs[AF].high);
// 			break;
// 		case 0x98:
// 			PlatformLogInstruction(byte, "RES 3, B");
// 			res_u3_r8(3, &state->cpu.regs[BC].high);
// 			break;
// 		case 0x99:
// 			PlatformLogInstruction(byte, "RES 3, C");
// 			res_u3_r8(3, &state->cpu.regs[BC].low);
// 			break;
// 		case 0x9A:
// 			PlatformLogInstruction(byte, "RES 3, D");
// 			res_u3_r8(3, &state->cpu.regs[DE].high);
// 			break;
// 		case 0x9B:
// 			PlatformLogInstruction(byte, "RES 3, E");
// 			res_u3_r8(3, &state->cpu.regs[DE].low);
// 			break;
// 		case 0x9C:
// 			PlatformLogInstruction(byte, "RES 3, H");
// 			res_u3_r8(3, &state->cpu.regs[HL].high);
// 			break;
// 		case 0x9D:
// 			PlatformLogInstruction(byte, "RES 3, L");
// 			res_u3_r8(3, &state->cpu.regs[HL].low);
// 			break;
// 		case 0x9E:
// 			PlatformLogInstruction(byte, "RES 3, [HL]");
// 			res_u3_aHL(3);
// 			break;
// 		case 0x9F:
// 			PlatformLogInstruction(byte, "RES 3, A");
// 			res_u3_r8(3, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xA0:
// 			PlatformLogInstruction(byte, "RES 4, B");
// 			res_u3_r8(4, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xA1:
// 			PlatformLogInstruction(byte, "RES 4, C");
// 			res_u3_r8(4, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xA2:
// 			PlatformLogInstruction(byte, "RES 4, D");
// 			res_u3_r8(4, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xA3:
// 			PlatformLogInstruction(byte, "RES 4, E");
// 			res_u3_r8(4, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xA4:
// 			PlatformLogInstruction(byte, "RES 4, H");
// 			res_u3_r8(4, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xA5:
// 			PlatformLogInstruction(byte, "RES 4, L");
// 			res_u3_r8(4, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xA6:
// 			PlatformLogInstruction(byte, "RES 4, [HL]");
// 			res_u3_aHL(4);
// 			break;
// 		case 0xA7:
// 			PlatformLogInstruction(byte, "RES 4, A");
// 			res_u3_r8(4, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xA8:
// 			PlatformLogInstruction(byte, "RES 5, B");
// 			res_u3_r8(5, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xA9:
// 			PlatformLogInstruction(byte, "RES 5, C");
// 			res_u3_r8(5, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xAA:
// 			PlatformLogInstruction(byte, "RES 5, D");
// 			res_u3_r8(5, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xAB:
// 			PlatformLogInstruction(byte, "RES 5, E");
// 			res_u3_r8(5, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xAC:
// 			PlatformLogInstruction(byte, "RES 5, H");
// 			res_u3_r8(5, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xAD:
// 			PlatformLogInstruction(byte, "RES 5, L");
// 			res_u3_r8(5, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xAE:
// 			PlatformLogInstruction(byte, "RES 5, [HL]");
// 			res_u3_aHL(5);
// 			break;
// 		case 0xAF:
// 			PlatformLogInstruction(byte, "RES 5, A");
// 			res_u3_r8(5, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xB0:
// 			PlatformLogInstruction(byte, "RES 6, B");
// 			res_u3_r8(6, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xB1:
// 			PlatformLogInstruction(byte, "RES 6, C");
// 			res_u3_r8(6, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xB2:
// 			PlatformLogInstruction(byte, "RES 6, D");
// 			res_u3_r8(6, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xB3:
// 			PlatformLogInstruction(byte, "RES 6, E");
// 			res_u3_r8(6, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xB4:
// 			PlatformLogInstruction(byte, "RES 6, H");
// 			res_u3_r8(6, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xB5:
// 			PlatformLogInstruction(byte, "RES 6, L");
// 			res_u3_r8(6, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xB6:
// 			PlatformLogInstruction(byte, "RES 6, [HL]");
// 			res_u3_aHL(6);
// 			break;
// 		case 0xB7:
// 			PlatformLogInstruction(byte, "RES 6, A");
// 			res_u3_r8(6, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xB8:
// 			PlatformLogInstruction(byte, "RES 7, B");
// 			res_u3_r8(7, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xB9:
// 			PlatformLogInstruction(byte, "RES 7, C");
// 			res_u3_r8(7, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xBA:
// 			PlatformLogInstruction(byte, "RES 7, D");
// 			res_u3_r8(7, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xBB:
// 			PlatformLogInstruction(byte, "RES 7, E");
// 			res_u3_r8(7, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xBC:
// 			PlatformLogInstruction(byte, "RES 7, H");
// 			res_u3_r8(7, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xBD:
// 			PlatformLogInstruction(byte, "RES 7, L");
// 			res_u3_r8(7, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xBE:
// 			PlatformLogInstruction(byte, "RES 7, [HL]");
// 			res_u3_aHL(7);
// 			break;
// 		case 0xBF:
// 			PlatformLogInstruction(byte, "RES 7, A");
// 			res_u3_r8(7, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xC0:
// 			PlatformLogInstruction(byte, "SET 0, B");
// 			set_u3_r8(0, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xC1:
// 			PlatformLogInstruction(byte, "SET 0, C");
// 			set_u3_r8(0, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xC2:
// 			PlatformLogInstruction(byte, "SET 0, D");
// 			set_u3_r8(0, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xC3:
// 			PlatformLogInstruction(byte, "SET 0, E");
// 			set_u3_r8(0, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xC4:
// 			PlatformLogInstruction(byte, "SET 0, H");
// 			set_u3_r8(0, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xC5:
// 			PlatformLogInstruction(byte, "SET 0, L");
// 			set_u3_r8(0, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xC6:
// 			PlatformLogInstruction(byte, "SET 0, [HL]");
// 			set_u3_aHL(0);
// 			break;
// 		case 0xC7:
// 			PlatformLogInstruction(byte, "SET 0, A");
// 			set_u3_r8(0, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xC8:
// 			PlatformLogInstruction(byte, "SET 1, B");
// 			set_u3_r8(1, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xC9:
// 			PlatformLogInstruction(byte, "SET 1, C");
// 			set_u3_r8(1, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xCA:
// 			PlatformLogInstruction(byte, "SET 1, D");
// 			set_u3_r8(1, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xCB:
// 			PlatformLogInstruction(byte, "SET 1, E");
// 			set_u3_r8(1, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xCC:
// 			PlatformLogInstruction(byte, "SET 1, H");
// 			set_u3_r8(1, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xCD:
// 			PlatformLogInstruction(byte, "SET 1, L");
// 			set_u3_r8(1, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xCE:
// 			PlatformLogInstruction(byte, "SET 1, [HL]");
// 			set_u3_aHL(1);
// 			break;
// 		case 0xCF:
// 			PlatformLogInstruction(byte, "SET 1, A");
// 			set_u3_r8(1, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xD0:
// 			PlatformLogInstruction(byte, "SET 2, B");
// 			set_u3_r8(2, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xD1:
// 			PlatformLogInstruction(byte, "SET 2, C");
// 			set_u3_r8(2, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xD2:
// 			PlatformLogInstruction(byte, "SET 2, D");
// 			set_u3_r8(2, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xD3:
// 			PlatformLogInstruction(byte, "SET 2, E");
// 			set_u3_r8(2, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xD4:
// 			PlatformLogInstruction(byte, "SET 2, H");
// 			set_u3_r8(2, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xD5:
// 			PlatformLogInstruction(byte, "SET 2, L");
// 			set_u3_r8(2, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xD6:
// 			PlatformLogInstruction(byte, "SET 2, [HL]");
// 			set_u3_aHL(2);
// 			break;
// 		case 0xD7:
// 			PlatformLogInstruction(byte, "SET 2, A");
// 			set_u3_r8(2, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xD8:
// 			PlatformLogInstruction(byte, "SET 3, B");
// 			set_u3_r8(3, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xD9:
// 			PlatformLogInstruction(byte, "SET 3, C");
// 			set_u3_r8(3, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xDA:
// 			PlatformLogInstruction(byte, "SET 3, D");
// 			set_u3_r8(3, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xDB:
// 			PlatformLogInstruction(byte, "SET 3, E");
// 			set_u3_r8(3, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xDC:
// 			PlatformLogInstruction(byte, "SET 3, H");
// 			set_u3_r8(3, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xDD:
// 			PlatformLogInstruction(byte, "SET 3, L");
// 			set_u3_r8(3, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xDE:
// 			PlatformLogInstruction(byte, "SET 3, [HL]");
// 			set_u3_aHL(3);
// 			break;
// 		case 0xDF:
// 			PlatformLogInstruction(byte, "SET 3, A");
// 			set_u3_r8(3, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xE0:
// 			PlatformLogInstruction(byte, "SET 4, B");
// 			set_u3_r8(4, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xE1:
// 			PlatformLogInstruction(byte, "SET 4, C");
// 			set_u3_r8(4, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xE2:
// 			PlatformLogInstruction(byte, "SET 4, D");
// 			set_u3_r8(4, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xE3:
// 			PlatformLogInstruction(byte, "SET 4, E");
// 			set_u3_r8(4, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xE4:
// 			PlatformLogInstruction(byte, "SET 4, H");
// 			set_u3_r8(4, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xE5:
// 			PlatformLogInstruction(byte, "SET 4, L");
// 			set_u3_r8(4, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xE6:
// 			PlatformLogInstruction(byte, "SET 4, [HL]");
// 			set_u3_aHL(4);
// 			break;
// 		case 0xE7:
// 			PlatformLogInstruction(byte, "SET 4, A");
// 			set_u3_r8(4, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xE8:
// 			PlatformLogInstruction(byte, "SET 5, B");
// 			set_u3_r8(5, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xE9:
// 			PlatformLogInstruction(byte, "SET 5, C");
// 			set_u3_r8(5, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xEA:
// 			PlatformLogInstruction(byte, "SET 5, D");
// 			set_u3_r8(5, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xEB:
// 			PlatformLogInstruction(byte, "SET 5, E");
// 			set_u3_r8(5, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xEC:
// 			PlatformLogInstruction(byte, "SET 5, H");
// 			set_u3_r8(5, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xED:
// 			PlatformLogInstruction(byte, "SET 5, L");
// 			set_u3_r8(5, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xEE:
// 			PlatformLogInstruction(byte, "SET 5, [HL]");
// 			set_u3_aHL(5);
// 			break;
// 		case 0xEF:
// 			PlatformLogInstruction(byte, "SET 5, A");
// 			set_u3_r8(5, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xF0:
// 			PlatformLogInstruction(byte, "SET 6, B");
// 			set_u3_r8(6, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xF1:
// 			PlatformLogInstruction(byte, "SET 6, C");
// 			set_u3_r8(6, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xF2:
// 			PlatformLogInstruction(byte, "SET 6, D");
// 			set_u3_r8(6, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xF3:
// 			PlatformLogInstruction(byte, "SET 6, E");
// 			set_u3_r8(6, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xF4:
// 			PlatformLogInstruction(byte, "SET 6, H");
// 			set_u3_r8(6, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xF5:
// 			PlatformLogInstruction(byte, "SET 6, L");
// 			set_u3_r8(6, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xF6:
// 			PlatformLogInstruction(byte, "SET 6, [HL]");
// 			set_u3_aHL(6);
// 			break;
// 		case 0xF7:
// 			PlatformLogInstruction(byte, "SET 6, A");
// 			set_u3_r8(6, &state->cpu.regs[AF].high);
// 			break;
// 		case 0xF8:
// 			PlatformLogInstruction(byte, "SET 7, B");
// 			set_u3_r8(7, &state->cpu.regs[BC].high);
// 			break;
// 		case 0xF9:
// 			PlatformLogInstruction(byte, "SET 7, C");
// 			set_u3_r8(7, &state->cpu.regs[BC].low);
// 			break;
// 		case 0xFA:
// 			PlatformLogInstruction(byte, "SET 7, D");
// 			set_u3_r8(7, &state->cpu.regs[DE].high);
// 			break;
// 		case 0xFB:
// 			PlatformLogInstruction(byte, "SET 7, E");
// 			set_u3_r8(7, &state->cpu.regs[DE].low);
// 			break;
// 		case 0xFC:
// 			PlatformLogInstruction(byte, "SET 7, H");
// 			set_u3_r8(7, &state->cpu.regs[HL].high);
// 			break;
// 		case 0xFD:
// 			PlatformLogInstruction(byte, "SET 7, L");
// 			set_u3_r8(7, &state->cpu.regs[HL].low);
// 			break;
// 		case 0xFE:
// 			PlatformLogInstruction(byte, "SET 7, [HL]");
// 			set_u3_aHL(7);
// 			break;
// 		case 0xFF:
// 			PlatformLogInstruction(byte, "SET 7, A");
// 			set_u3_r8(7, &state->cpu.regs[AF].high);
// 			break;
// 		}
// 		state->cpu.prefix = false;
// 	} else {
// 		// ==================== NON-PREFIX ====================
// 		switch (byte) {
// 		case 0x00:
// 			PlatformLogInstruction(byte, "NOP");
// 			nop();
// 			break;
// 		case 0x01: {
// 			PlatformLogInstruction(byte, "LD BC, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			ld_r16_n16(&state->cpu.regs[BC].full, n16);
// 		} break;
// 		case 0x02:
// 			PlatformLogInstruction(byte, "LD [BC], A");
// 			ld_a16_A(state->cpu.regs[BC].full);
// 			break;
// 		case 0x03:
// 			PlatformLogInstruction(byte, "INC BC");
// 			inc_r16(&state->cpu.regs[BC].full);
// 			break;
// 		case 0x04:
// 			PlatformLogInstruction(byte, "INC B");
// 			inc_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x05:
// 			PlatformLogInstruction(byte, "DEC B");
// 			dec_r8(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x06: {
// 			PlatformLogInstruction(byte, "LD B, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ld_r8_n8(&state->cpu.regs[BC].high, n8);
// 		} break;
// 		case 0x07:
// 			PlatformLogInstruction(byte, "RLCA");
// 			rlca();
// 			break;
// 		case 0x08: {
// 			PlatformLogInstruction(byte, "LD [n16], SP");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			ld_addr16_SP(n16);
// 		} break;
// 		case 0x09:
// 			PlatformLogInstruction(byte, "ADD HL, BC");
// 			add_HL_r16(state->cpu.regs[BC].full);
// 			break;
// 		case 0x0A:
// 			PlatformLogInstruction(byte, "LD A, [BC]");
// 			ld_A_a16(state->cpu.regs[BC].full);
// 			break;
// 		case 0x0B:
// 			PlatformLogInstruction(byte, "DEC BC");
// 			dec_r16(&state->cpu.regs[BC].full);
// 			break;
// 		case 0x0C:
// 			PlatformLogInstruction(byte, "INC C");
// 			inc_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x0D:
// 			PlatformLogInstruction(byte, "DEC C");
// 			dec_r8(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x0E: {
// 			PlatformLogInstruction(byte, "LD C, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ld_r8_n8(&state->cpu.regs[BC].low, n8);
// 		} break;
// 		case 0x0F:
// 			PlatformLogInstruction(byte, "RRCA");
// 			rrca();
// 			break;
// 		case 0x10: {
// 			PlatformLogInstruction(byte, "STOP n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			stop_n8(n8);
// 		} break;
// 		case 0x11: {
// 			PlatformLogInstruction(byte, "LD DE, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			ld_r16_n16(&state->cpu.regs[DE].full, n16);
// 		} break;
// 		case 0x12:
// 			PlatformLogInstruction(byte, "LD [DE], A");
// 			ld_a16_A(state->cpu.regs[DE].full);
// 			break;
// 		case 0x13:
// 			PlatformLogInstruction(byte, "INC DE");
// 			inc_r16(&state->cpu.regs[DE].full);
// 			break;
// 		case 0x14:
// 			PlatformLogInstruction(byte, "INC D");
// 			inc_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x15:
// 			PlatformLogInstruction(byte, "DEC D");
// 			dec_r8(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x16: {
// 			PlatformLogInstruction(byte, "LD D, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ld_r8_n8(&state->cpu.regs[DE].high, n8);
// 		} break;
// 		case 0x17:
// 			PlatformLogInstruction(byte, "RLA");
// 			rla();
// 			break;
// 		case 0x18: {
// 			PlatformLogInstruction(byte, "JR n16");
// 			i8 offset = (i8)read8(state->cpu.regs[PC].full + 1);
// 			jr_n16(offset);
// 			break;
// 		}
// 		case 0x19:
// 			PlatformLogInstruction(byte, "ADD HL, DE");
// 			add_HL_r16(state->cpu.regs[DE].full);
// 			break;
// 		case 0x1A:
// 			PlatformLogInstruction(byte, "LD A, [DE]");
// 			ld_A_a16(state->cpu.regs[DE].full);
// 			break;
// 		case 0x1B:
// 			PlatformLogInstruction(byte, "DEC DE");
// 			dec_r16(&state->cpu.regs[DE].full);
// 			break;
// 		case 0x1C:
// 			PlatformLogInstruction(byte, "INC E");
// 			inc_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x1D:
// 			PlatformLogInstruction(byte, "DEC E");
// 			dec_r8(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x1E: {
// 			PlatformLogInstruction(byte, "LD E, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ld_r8_n8(&state->cpu.regs[DE].low, n8);
// 		} break;
// 		case 0x1F:
// 			PlatformLogInstruction(byte, "RRA");
// 			rra();
// 			break;
// 		case 0x20: {
// 			PlatformLogInstruction(byte, "JR NZ, n16");
// 			i8 offset = (i8)read8(state->cpu.regs[PC].full + 1);
// 			jr_cc_n16(Z, false, offset);
// 			break;
// 		}
// 		case 0x21: {
// 			PlatformLogInstruction(byte, "LD HL, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			ld_r16_n16(&state->cpu.regs[HL].full, n16);
// 		} break;
// 		case 0x22:
// 			PlatformLogInstruction(byte, "LD [HL+], A");
// 			ld_aHLi_A();
// 			break;
// 		case 0x23:
// 			PlatformLogInstruction(byte, "INC HL");
// 			inc_r16(&state->cpu.regs[HL].full);
// 			break;
// 		case 0x24:
// 			PlatformLogInstruction(byte, "INC H");
// 			inc_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x25:
// 			PlatformLogInstruction(byte, "DEC H");
// 			dec_r8(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x26: {
// 			PlatformLogInstruction(byte, "LD H, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ld_r8_n8(&state->cpu.regs[HL].high, n8);
// 		} break;
// 		case 0x27:
// 			PlatformLogInstruction(byte, "DAA");
// 			daa();
// 			break;
// 		case 0x28: {
// 			PlatformLogInstruction(byte, "JR Z, n16");
// 			i8 offset = (i8)read8(state->cpu.regs[PC].full + 1);
// 			jr_cc_n16(Z, true, offset);
// 			break;
// 		}
// 		case 0x29:
// 			PlatformLogInstruction(byte, "ADD HL, HL");
// 			add_HL_r16(state->cpu.regs[HL].full);
// 			break;
// 		case 0x2A:
// 			PlatformLogInstruction(byte, "LD A, [HL+]");
// 			ld_A_aHLi();
// 			break;
// 		case 0x2B:
// 			PlatformLogInstruction(byte, "DEC HL");
// 			dec_r16(&state->cpu.regs[HL].full);
// 			break;
// 		case 0x2C:
// 			PlatformLogInstruction(byte, "INC L");
// 			inc_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x2D:
// 			PlatformLogInstruction(byte, "DEC L");
// 			dec_r8(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x2E: {
// 			PlatformLogInstruction(byte, "LD L, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ld_r8_n8(&state->cpu.regs[HL].low, n8);
// 		} break;
// 		case 0x2F:
// 			PlatformLogInstruction(byte, "CPL");
// 			cpl();
// 			break;
// 		case 0x30: {
// 			PlatformLogInstruction(byte, "JR NC, n16");
// 			i8 offset = (i8)read8(state->cpu.regs[PC].full + 1);
// 			jr_cc_n16(C, false, offset);
// 			break;
// 		}
// 		case 0x31: {
// 			PlatformLogInstruction(byte, "LD SP, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			ld_r16_n16(&state->cpu.regs[SP].full, n16);
// 		} break;
// 		case 0x32:
// 			PlatformLogInstruction(byte, "LD [HL-], A");
// 			ld_aHLd_A();
// 			break;
// 		case 0x33:
// 			PlatformLogInstruction(byte, "INC SP");
// 			inc_r16(&state->cpu.regs[SP].full);
// 			break;
// 		case 0x34:
// 			PlatformLogInstruction(byte, "INC [HL]");
// 			inc_aHL();
// 			break;
// 		case 0x35:
// 			PlatformLogInstruction(byte, "DEC [HL]");
// 			dec_aHL();
// 			break;
// 		case 0x36: {
// 			PlatformLogInstruction(byte, "LD [HL], n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ld_aHL_n8(n8);
// 		} break;
// 		case 0x37:
// 			PlatformLogInstruction(byte, "SCF");
// 			scf();
// 			break;
// 		case 0x38: {
// 			PlatformLogInstruction(byte, "JR C, n16");
// 			i8 offset = (i8)read8(state->cpu.regs[PC].full + 1);
// 			jr_cc_n16(C, true, offset);
// 			break;
// 		}
// 		case 0x39:
// 			PlatformLogInstruction(byte, "ADD HL, SP");
// 			add_HL_r16(state->cpu.regs[SP].full);
// 			break;
// 		case 0x3A:
// 			PlatformLogInstruction(byte, "LD A, [HL-]");
// 			ld_A_aHLd();
// 			break;
// 		case 0x3B:
// 			PlatformLogInstruction(byte, "DEC SP");
// 			dec_r16(&state->cpu.regs[SP].full);
// 			break;
// 		case 0x3C:
// 			PlatformLogInstruction(byte, "INC A");
// 			inc_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x3D:
// 			PlatformLogInstruction(byte, "DEC A");
// 			dec_r8(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x3E: {
// 			PlatformLogInstruction(byte, "LD A, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ld_r8_n8(&state->cpu.regs[AF].high, n8);
// 		} break;
// 		case 0x3F:
// 			PlatformLogInstruction(byte, "CCF");
// 			ccf();
// 			break;
// 		case 0x40:
// 			PlatformLogInstruction(byte, "LD B, B");
// 			ld_r8_r8(&state->cpu.regs[BC].high, state->cpu.regs[BC].high);
// 			if (stop_on_ld_b_b)
// 				running = false;
// 			break;
// 		case 0x41:
// 			PlatformLogInstruction(byte, "LD B, C");
// 			ld_r8_r8(&state->cpu.regs[BC].high, state->cpu.regs[BC].low);
// 			break;
// 		case 0x42:
// 			PlatformLogInstruction(byte, "LD B, D");
// 			ld_r8_r8(&state->cpu.regs[BC].high, state->cpu.regs[DE].high);
// 			break;
// 		case 0x43:
// 			PlatformLogInstruction(byte, "LD B, E");
// 			ld_r8_r8(&state->cpu.regs[BC].high, state->cpu.regs[DE].low);
// 			break;
// 		case 0x44:
// 			PlatformLogInstruction(byte, "LD B, H");
// 			ld_r8_r8(&state->cpu.regs[BC].high, state->cpu.regs[HL].high);
// 			break;
// 		case 0x45:
// 			PlatformLogInstruction(byte, "LD B, L");
// 			ld_r8_r8(&state->cpu.regs[BC].high, state->cpu.regs[HL].low);
// 			break;
// 		case 0x46:
// 			PlatformLogInstruction(byte, "LD B, [HL]");
// 			ld_r8_aHL(&state->cpu.regs[BC].high);
// 			break;
// 		case 0x47:
// 			PlatformLogInstruction(byte, "LD B, A");
// 			ld_r8_r8(&state->cpu.regs[BC].high, state->cpu.regs[AF].high);
// 			break;
// 		case 0x48:
// 			PlatformLogInstruction(byte, "LD C, B");
// 			ld_r8_r8(&state->cpu.regs[BC].low, state->cpu.regs[BC].high);
// 			break;
// 		case 0x49:
// 			PlatformLogInstruction(byte, "LD C, C");
// 			ld_r8_r8(&state->cpu.regs[BC].low, state->cpu.regs[BC].low);
// 			break;
// 		case 0x4A:
// 			PlatformLogInstruction(byte, "LD C, D");
// 			ld_r8_r8(&state->cpu.regs[BC].low, state->cpu.regs[DE].high);
// 			break;
// 		case 0x4B:
// 			PlatformLogInstruction(byte, "LD C, E");
// 			ld_r8_r8(&state->cpu.regs[BC].low, state->cpu.regs[DE].low);
// 			break;
// 		case 0x4C:
// 			PlatformLogInstruction(byte, "LD C, H");
// 			ld_r8_r8(&state->cpu.regs[BC].low, state->cpu.regs[HL].high);
// 			break;
// 		case 0x4D:
// 			PlatformLogInstruction(byte, "LD C, L");
// 			ld_r8_r8(&state->cpu.regs[BC].low, state->cpu.regs[HL].low);
// 			break;
// 		case 0x4E:
// 			PlatformLogInstruction(byte, "LD C, [HL]");
// 			ld_r8_aHL(&state->cpu.regs[BC].low);
// 			break;
// 		case 0x4F:
// 			PlatformLogInstruction(byte, "LD C, A");
// 			ld_r8_r8(&state->cpu.regs[BC].low, state->cpu.regs[AF].high);
// 			break;
// 		case 0x50:
// 			PlatformLogInstruction(byte, "LD D, B");
// 			ld_r8_r8(&state->cpu.regs[DE].high, state->cpu.regs[BC].high);
// 			break;
// 		case 0x51:
// 			PlatformLogInstruction(byte, "LD D, C");
// 			ld_r8_r8(&state->cpu.regs[DE].high, state->cpu.regs[BC].low);
// 			break;
// 		case 0x52:
// 			PlatformLogInstruction(byte, "LD D, D");
// 			ld_r8_r8(&state->cpu.regs[DE].high, state->cpu.regs[DE].high);
// 			break;
// 		case 0x53:
// 			PlatformLogInstruction(byte, "LD D, E");
// 			ld_r8_r8(&state->cpu.regs[DE].high, state->cpu.regs[DE].low);
// 			break;
// 		case 0x54:
// 			PlatformLogInstruction(byte, "LD D, H");
// 			ld_r8_r8(&state->cpu.regs[DE].high, state->cpu.regs[HL].high);
// 			break;
// 		case 0x55:
// 			PlatformLogInstruction(byte, "LD D, L");
// 			ld_r8_r8(&state->cpu.regs[DE].high, state->cpu.regs[HL].low);
// 			break;
// 		case 0x56:
// 			PlatformLogInstruction(byte, "LD D, [HL]");
// 			ld_r8_aHL(&state->cpu.regs[DE].high);
// 			break;
// 		case 0x57:
// 			PlatformLogInstruction(byte, "LD D, A");
// 			ld_r8_r8(&state->cpu.regs[DE].high, state->cpu.regs[AF].high);
// 			break;
// 		case 0x58:
// 			PlatformLogInstruction(byte, "LD E, B");
// 			ld_r8_r8(&state->cpu.regs[DE].low, state->cpu.regs[BC].high);
// 			break;
// 		case 0x59:
// 			PlatformLogInstruction(byte, "LD E, C");
// 			ld_r8_r8(&state->cpu.regs[DE].low, state->cpu.regs[BC].low);
// 			break;
// 		case 0x5A:
// 			PlatformLogInstruction(byte, "LD E, D");
// 			ld_r8_r8(&state->cpu.regs[DE].low, state->cpu.regs[DE].high);
// 			break;
// 		case 0x5B:
// 			PlatformLogInstruction(byte, "LD E, E");
// 			ld_r8_r8(&state->cpu.regs[DE].low, state->cpu.regs[DE].low);
// 			break;
// 		case 0x5C:
// 			PlatformLogInstruction(byte, "LD E, H");
// 			ld_r8_r8(&state->cpu.regs[DE].low, state->cpu.regs[HL].high);
// 			break;
// 		case 0x5D:
// 			PlatformLogInstruction(byte, "LD E, L");
// 			ld_r8_r8(&state->cpu.regs[DE].low, state->cpu.regs[HL].low);
// 			break;
// 		case 0x5E:
// 			PlatformLogInstruction(byte, "LD E, [HL]");
// 			ld_r8_aHL(&state->cpu.regs[DE].low);
// 			break;
// 		case 0x5F:
// 			PlatformLogInstruction(byte, "LD E, A");
// 			ld_r8_r8(&state->cpu.regs[DE].low, state->cpu.regs[AF].high);
// 			break;
// 		case 0x60:
// 			PlatformLogInstruction(byte, "LD H, B");
// 			ld_r8_r8(&state->cpu.regs[HL].high, state->cpu.regs[BC].high);
// 			break;
// 		case 0x61:
// 			PlatformLogInstruction(byte, "LD H, C");
// 			ld_r8_r8(&state->cpu.regs[HL].high, state->cpu.regs[BC].low);
// 			break;
// 		case 0x62:
// 			PlatformLogInstruction(byte, "LD H, D");
// 			ld_r8_r8(&state->cpu.regs[HL].high, state->cpu.regs[DE].high);
// 			break;
// 		case 0x63:
// 			PlatformLogInstruction(byte, "LD H, E");
// 			ld_r8_r8(&state->cpu.regs[HL].high, state->cpu.regs[DE].low);
// 			break;
// 		case 0x64:
// 			PlatformLogInstruction(byte, "LD H, H");
// 			ld_r8_r8(&state->cpu.regs[HL].high, state->cpu.regs[HL].high);
// 			break;
// 		case 0x65:
// 			PlatformLogInstruction(byte, "LD H, L");
// 			ld_r8_r8(&state->cpu.regs[HL].high, state->cpu.regs[HL].low);
// 			break;
// 		case 0x66:
// 			PlatformLogInstruction(byte, "LD H, [HL]");
// 			ld_r8_aHL(&state->cpu.regs[HL].high);
// 			break;
// 		case 0x67:
// 			PlatformLogInstruction(byte, "LD H, A");
// 			ld_r8_r8(&state->cpu.regs[HL].high, state->cpu.regs[AF].high);
// 			break;
// 		case 0x68:
// 			PlatformLogInstruction(byte, "LD L, B");
// 			ld_r8_r8(&state->cpu.regs[HL].low, state->cpu.regs[BC].high);
// 			break;
// 		case 0x69:
// 			PlatformLogInstruction(byte, "LD L, C");
// 			ld_r8_r8(&state->cpu.regs[HL].low, state->cpu.regs[BC].low);
// 			break;
// 		case 0x6A:
// 			PlatformLogInstruction(byte, "LD L, D");
// 			ld_r8_r8(&state->cpu.regs[HL].low, state->cpu.regs[DE].high);
// 			break;
// 		case 0x6B:
// 			PlatformLogInstruction(byte, "LD L, E");
// 			ld_r8_r8(&state->cpu.regs[HL].low, state->cpu.regs[DE].low);
// 			break;
// 		case 0x6C:
// 			PlatformLogInstruction(byte, "LD L, H");
// 			ld_r8_r8(&state->cpu.regs[HL].low, state->cpu.regs[HL].high);
// 			break;
// 		case 0x6D:
// 			PlatformLogInstruction(byte, "LD L, L");
// 			ld_r8_r8(&state->cpu.regs[HL].low, state->cpu.regs[HL].low);
// 			break;
// 		case 0x6E:
// 			PlatformLogInstruction(byte, "LD L, [HL]");
// 			ld_r8_aHL(&state->cpu.regs[HL].low);
// 			break;
// 		case 0x6F:
// 			PlatformLogInstruction(byte, "LD L, A");
// 			ld_r8_r8(&state->cpu.regs[HL].low, state->cpu.regs[AF].high);
// 			break;
// 		case 0x70:
// 			PlatformLogInstruction(byte, "LD [HL], B");
// 			ld_aHL_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0x71:
// 			PlatformLogInstruction(byte, "LD [HL], C");
// 			ld_aHL_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0x72:
// 			PlatformLogInstruction(byte, "LD [HL], D");
// 			ld_aHL_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0x73:
// 			PlatformLogInstruction(byte, "LD [HL], E");
// 			ld_aHL_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0x74:
// 			PlatformLogInstruction(byte, "LD [HL], H");
// 			ld_aHL_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0x75:
// 			PlatformLogInstruction(byte, "LD [HL], L");
// 			ld_aHL_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0x76:
// 			PlatformLogInstruction(byte, "HALT");
// 			halt();
// 			break;
// 		case 0x77:
// 			PlatformLogInstruction(byte, "LD [HL], A");
// 			ld_aHL_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0x78:
// 			PlatformLogInstruction(byte, "LD A, B");
// 			ld_r8_r8(&state->cpu.regs[AF].high, state->cpu.regs[BC].high);
// 			break;
// 		case 0x79:
// 			PlatformLogInstruction(byte, "LD A, C");
// 			ld_r8_r8(&state->cpu.regs[AF].high, state->cpu.regs[BC].low);
// 			break;
// 		case 0x7A:
// 			PlatformLogInstruction(byte, "LD A, D");
// 			ld_r8_r8(&state->cpu.regs[AF].high, state->cpu.regs[DE].high);
// 			break;
// 		case 0x7B:
// 			PlatformLogInstruction(byte, "LD A, E");
// 			ld_r8_r8(&state->cpu.regs[AF].high, state->cpu.regs[DE].low);
// 			break;
// 		case 0x7C:
// 			PlatformLogInstruction(byte, "LD A, H");
// 			ld_r8_r8(&state->cpu.regs[AF].high, state->cpu.regs[HL].high);
// 			break;
// 		case 0x7D:
// 			PlatformLogInstruction(byte, "LD A, L");
// 			ld_r8_r8(&state->cpu.regs[AF].high, state->cpu.regs[HL].low);
// 			break;
// 		case 0x7E:
// 			PlatformLogInstruction(byte, "LD A, [HL]");
// 			ld_r8_aHL(&state->cpu.regs[AF].high);
// 			break;
// 		case 0x7F:
// 			PlatformLogInstruction(byte, "LD A, A");
// 			ld_r8_r8(&state->cpu.regs[AF].high, state->cpu.regs[AF].high);
// 			break;
// 		case 0x80:
// 			PlatformLogInstruction(byte, "ADD A, B");
// 			add_A_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0x81:
// 			PlatformLogInstruction(byte, "ADD A, C");
// 			add_A_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0x82:
// 			PlatformLogInstruction(byte, "ADD A, D");
// 			add_A_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0x83:
// 			PlatformLogInstruction(byte, "ADD A, E");
// 			add_A_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0x84:
// 			PlatformLogInstruction(byte, "ADD A, H");
// 			add_A_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0x85:
// 			PlatformLogInstruction(byte, "ADD A, L");
// 			add_A_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0x86:
// 			PlatformLogInstruction(byte, "ADD A, [HL]");
// 			add_A_aHL();
// 			break;
// 		case 0x87:
// 			PlatformLogInstruction(byte, "ADD A, A");
// 			add_A_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0x88:
// 			PlatformLogInstruction(byte, "ADC A, B");
// 			adc_A_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0x89:
// 			PlatformLogInstruction(byte, "ADC A, C");
// 			adc_A_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0x8A:
// 			PlatformLogInstruction(byte, "ADC A, D");
// 			adc_A_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0x8B:
// 			PlatformLogInstruction(byte, "ADC A, E");
// 			adc_A_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0x8C:
// 			PlatformLogInstruction(byte, "ADC A, H");
// 			adc_A_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0x8D:
// 			PlatformLogInstruction(byte, "ADC A, L");
// 			adc_A_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0x8E:
// 			PlatformLogInstruction(byte, "ADC A, [HL]");
// 			adc_A_aHL();
// 			break;
// 		case 0x8F:
// 			PlatformLogInstruction(byte, "ADC A, A");
// 			adc_A_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0x90:
// 			PlatformLogInstruction(byte, "SUB A, B");
// 			sub_A_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0x91:
// 			PlatformLogInstruction(byte, "SUB A, C");
// 			sub_A_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0x92:
// 			PlatformLogInstruction(byte, "SUB A, D");
// 			sub_A_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0x93:
// 			PlatformLogInstruction(byte, "SUB A, E");
// 			sub_A_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0x94:
// 			PlatformLogInstruction(byte, "SUB A, H");
// 			sub_A_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0x95:
// 			PlatformLogInstruction(byte, "SUB A, L");
// 			sub_A_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0x96:
// 			PlatformLogInstruction(byte, "SUB A, [HL]");
// 			sub_A_aHL();
// 			break;
// 		case 0x97:
// 			PlatformLogInstruction(byte, "SUB A, A");
// 			sub_A_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0x98:
// 			PlatformLogInstruction(byte, "SBC A, B");
// 			sbc_A_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0x99:
// 			PlatformLogInstruction(byte, "SBC A, C");
// 			sbc_A_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0x9A:
// 			PlatformLogInstruction(byte, "SBC A, D");
// 			sbc_A_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0x9B:
// 			PlatformLogInstruction(byte, "SBC A, E");
// 			sbc_A_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0x9C:
// 			PlatformLogInstruction(byte, "SBC A, H");
// 			sbc_A_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0x9D:
// 			PlatformLogInstruction(byte, "SBC A, L");
// 			sbc_A_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0x9E:
// 			PlatformLogInstruction(byte, "SBC A, [HL]");
// 			sbc_A_aHL();
// 			break;
// 		case 0x9F:
// 			PlatformLogInstruction(byte, "SBC A, A");
// 			sbc_A_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0xA0:
// 			PlatformLogInstruction(byte, "AND A, B");
// 			and_A_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0xA1:
// 			PlatformLogInstruction(byte, "AND A, C");
// 			and_A_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0xA2:
// 			PlatformLogInstruction(byte, "AND A, D");
// 			and_A_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0xA3:
// 			PlatformLogInstruction(byte, "AND A, E");
// 			and_A_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0xA4:
// 			PlatformLogInstruction(byte, "AND A, H");
// 			and_A_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0xA5:
// 			PlatformLogInstruction(byte, "AND A, L");
// 			and_A_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0xA6:
// 			PlatformLogInstruction(byte, "AND A, [HL]");
// 			and_A_aHL();
// 			break;
// 		case 0xA7:
// 			PlatformLogInstruction(byte, "AND A, A");
// 			and_A_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0xA8:
// 			PlatformLogInstruction(byte, "XOR A, B");
// 			xor_A_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0xA9:
// 			PlatformLogInstruction(byte, "XOR A, C");
// 			xor_A_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0xAA:
// 			PlatformLogInstruction(byte, "XOR A, D");
// 			xor_A_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0xAB:
// 			PlatformLogInstruction(byte, "XOR A, E");
// 			xor_A_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0xAC:
// 			PlatformLogInstruction(byte, "XOR A, H");
// 			xor_A_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0xAD:
// 			PlatformLogInstruction(byte, "XOR A, L");
// 			xor_A_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0xAE:
// 			PlatformLogInstruction(byte, "XOR A, [HL]");
// 			xor_A_aHL();
// 			break;
// 		case 0xAF:
// 			PlatformLogInstruction(byte, "XOR A, A");
// 			xor_A_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0xB0:
// 			PlatformLogInstruction(byte, "OR A, B");
// 			or_A_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0xB1:
// 			PlatformLogInstruction(byte, "OR A, C");
// 			or_A_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0xB2:
// 			PlatformLogInstruction(byte, "OR A, D");
// 			or_A_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0xB3:
// 			PlatformLogInstruction(byte, "OR A, E");
// 			or_A_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0xB4:
// 			PlatformLogInstruction(byte, "OR A, H");
// 			or_A_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0xB5:
// 			PlatformLogInstruction(byte, "OR A, L");
// 			or_A_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0xB6:
// 			PlatformLogInstruction(byte, "OR A, [HL]");
// 			or_A_aHL();
// 			break;
// 		case 0xB7:
// 			PlatformLogInstruction(byte, "OR A, A");
// 			or_A_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0xB8:
// 			PlatformLogInstruction(byte, "CP A, B");
// 			cp_A_r8(state->cpu.regs[BC].high);
// 			break;
// 		case 0xB9:
// 			PlatformLogInstruction(byte, "CP A, C");
// 			cp_A_r8(state->cpu.regs[BC].low);
// 			break;
// 		case 0xBA:
// 			PlatformLogInstruction(byte, "CP A, D");
// 			cp_A_r8(state->cpu.regs[DE].high);
// 			break;
// 		case 0xBB:
// 			PlatformLogInstruction(byte, "CP A, E");
// 			cp_A_r8(state->cpu.regs[DE].low);
// 			break;
// 		case 0xBC:
// 			PlatformLogInstruction(byte, "CP A, H");
// 			cp_A_r8(state->cpu.regs[HL].high);
// 			break;
// 		case 0xBD:
// 			PlatformLogInstruction(byte, "CP A, L");
// 			cp_A_r8(state->cpu.regs[HL].low);
// 			break;
// 		case 0xBE:
// 			PlatformLogInstruction(byte, "CP A, [HL]");
// 			cp_A_aHL();
// 			break;
// 		case 0xBF:
// 			PlatformLogInstruction(byte, "CP A, A");
// 			cp_A_r8(state->cpu.regs[AF].high);
// 			break;
// 		case 0xC0:
// 			PlatformLogInstruction(byte, "RET NZ");
// 			ret_cc(Z, false);
// 			break;
// 		case 0xC1:
// 			PlatformLogInstruction(byte, "POP BC");
// 			pop_r16(&state->cpu.regs[BC].full);
// 			break;
// 		case 0xC2: {
// 			PlatformLogInstruction(byte, "JP NZ, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			jp_cc_n16(Z, false, n16);
// 			break;
// 		}
// 		case 0xC3: {
// 			PlatformLogInstruction(byte, "JP n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			jp_n16(n16);
// 			break;
// 		}
// 		case 0xC4: {
// 			PlatformLogInstruction(byte, "CALL NZ, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			call_cc_n16(Z, false, n16);
// 			break;
// 		}
// 		case 0xC5:
// 			PlatformLogInstruction(byte, "PUSH BC");
// 			push_r16(state->cpu.regs[BC].full);
// 			break;
// 		case 0xC6: {
// 			PlatformLogInstruction(byte, "ADD A, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			add_A_n8(n8);
// 			break;
// 		}
// 		case 0xC7:
// 			PlatformLogInstruction(byte, "RST $00");
// 			rst(0x00);
// 			break;
// 		case 0xC8:
// 			PlatformLogInstruction(byte, "RET Z");
// 			ret_cc(Z, true);
// 			break;
// 		case 0xC9:
// 			PlatformLogInstruction(byte, "RET");
// 			ret();
// 			break;
// 		case 0xCA: {
// 			PlatformLogInstruction(byte, "JP Z, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			jp_cc_n16(Z, true, n16);
// 			break;
// 		}
// 		case 0xCB:
// 			PlatformLogInstruction(byte, "PREFIX TOGGLE");
// 			state->cpu.prefix = true;
// 			state->cpu.regs[PC].full += 1;
// 			break;
// 		case 0xCC: {
// 			PlatformLogInstruction(byte, "CALL Z, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			call_cc_n16(Z, true, n16);
// 			break;
// 		}
// 		case 0xCD: {
// 			PlatformLogInstruction(byte, "CALL n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			call_n16(n16);
// 			break;
// 		}
// 		case 0xCE: {
// 			PlatformLogInstruction(byte, "ADC A, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			adc_A_n8(n8);
// 			break;
// 		}
// 		case 0xCF:
// 			PlatformLogInstruction(byte, "RST $08");
// 			rst(0x08);
// 			break;
// 		case 0xD0:
// 			PlatformLogInstruction(byte, "RET NC");
// 			ret_cc(C, false);
// 			break;
// 		case 0xD1:
// 			PlatformLogInstruction(byte, "POP DE");
// 			pop_r16(&state->cpu.regs[DE].full);
// 			break;
// 		case 0xD2: {
// 			PlatformLogInstruction(byte, "JP NC, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			jp_cc_n16(C, false, n16);
// 			break;
// 		}
// 		case 0xD4: {
// 			PlatformLogInstruction(byte, "CALL NC, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			call_cc_n16(C, false, n16);
// 			break;
// 		}
// 		case 0xD5:
// 			PlatformLogInstruction(byte, "PUSH DE");
// 			push_r16(state->cpu.regs[DE].full);
// 			break;
// 		case 0xD6: {
// 			PlatformLogInstruction(byte, "SUB A, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			sub_A_n8(n8);
// 			break;
// 		}
// 		case 0xD7:
// 			PlatformLogInstruction(byte, "RST $10");
// 			rst(0x10);
// 			break;
// 		case 0xD8:
// 			PlatformLogInstruction(byte, "RET C");
// 			ret_cc(C, true);
// 			break;
// 		case 0xD9:
// 			PlatformLogInstruction(byte, "RETI");
// 			reti();
// 			break;
// 		case 0xDA: {
// 			PlatformLogInstruction(byte, "JP C, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			jp_cc_n16(C, true, n16);
// 			break;
// 		}
// 		case 0xDC: {
// 			PlatformLogInstruction(byte, "CALL C, n16");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			call_cc_n16(C, true, n16);
// 			break;
// 		}
// 		case 0xDE: {
// 			PlatformLogInstruction(byte, "SBC A, n8");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			sbc_A_n8(n8);
// 			break;
// 		}
// 		case 0xDF:
// 			PlatformLogInstruction(byte, "RST $18");
// 			rst(0x18);
// 			break;
// 		case 0xE0: {
// 			PlatformLogInstruction(byte, "LDH [n16], A");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ldh_addr16_A(0xFF00 + n8);
// 			break;
// 		}
// 		case 0xE1:
// 			PlatformLogInstruction(byte, "POP HL");
// 			pop_r16(&state->cpu.regs[HL].full);
// 			break;
// 		case 0xE2:
// 			PlatformLogInstruction(byte, "LDH [C], A");
// 			ldh_aC_A();
// 			break;
// 		case 0xE5:
// 			PlatformLogInstruction(byte, "PUSH HL");
// 			push_r16(state->cpu.regs[HL].full);
// 			break;
// 		case 0xE6: {
// 			PlatformLogInstruction(byte, "AND A, n8");
// 			i8 n8 = (i8)read8(state->cpu.regs[PC].full + 1);
// 			and_A_n8(n8);
// 		} break;
// 		case 0xE7:
// 			PlatformLogInstruction(byte, "RST $20");
// 			rst(0x20);
// 			break;
// 		case 0xE8: {
// 			PlatformLogInstruction(byte, "ADD SP, n8");
// 			i8 n8 = (i8)read8(state->cpu.regs[PC].full + 1);
// 			add_SP_n8(n8);
// 		} break;
// 		case 0xE9:
// 			PlatformLogInstruction(byte, "JP HL");
// 			jp_aHL();
// 			break;
// 		case 0xEA: {
// 			PlatformLogInstruction(byte, "LD [n16], A");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			ld_addr16_A(n16);
// 			break;
// 		}
// 		case 0xEE: {
// 			PlatformLogInstruction(byte, "XOR A, n8");
// 			i8 n8 = (i8)read8(state->cpu.regs[PC].full + 1);
// 			xor_A_n8(n8);
// 		} break;
// 		case 0xEF:
// 			PlatformLogInstruction(byte, "RST $28");
// 			rst(0x28);
// 			break;
// 		case 0xF0: {
// 			PlatformLogInstruction(byte, "LDH A, [n16]");
// 			u8 n8 = read8(state->cpu.regs[PC].full + 1);
// 			ldh_A_addr16(0xFF00 + n8);
// 			break;
// 		}
// 		case 0xF1:
// 			PlatformLogInstruction(byte, "POP AF");
// 			pop_AF();
// 			break;
// 		case 0xF2:
// 			PlatformLogInstruction(byte, "LDH A, [C]");
// 			ldh_A_aC();
// 			break;
// 		case 0xF3:
// 			PlatformLogInstruction(byte, "DI");
// 			di();
// 			break;
// 		case 0xF5:
// 			PlatformLogInstruction(byte, "PUSH AF");
// 			push_AF();
// 			break;
// 		case 0xF6: {
// 			PlatformLogInstruction(byte, "OR A, n8");
// 			i8 n8 = (i8)read8(state->cpu.regs[PC].full + 1);
// 			or_A_n8(n8);
// 		} break;
// 		case 0xF7:
// 			PlatformLogInstruction(byte, "RST $30");
// 			rst(0x30);
// 			break;
// 		case 0xF8: {
// 			PlatformLogInstruction(byte, "LD HL, SP+n8");
// 			i8 n8 = (i8)read8(state->cpu.regs[PC].full + 1);
// 			ld_HL_SPe8(n8);
// 		} break;
// 		case 0xF9:
// 			PlatformLogInstruction(byte, "LD SP, HL");
// 			ld_SP_HL();
// 			break;
// 		case 0xFA: {
// 			PlatformLogInstruction(byte, "LD A, [n16]");
// 			u16 n16 = read16(state->cpu.regs[PC].full + 1);
// 			ld_A_addr16(n16);
// 			break;
// 		}
// 		case 0xFB:
// 			PlatformLogInstruction(byte, "EI");
// 			ei();
// 			break;
// 		case 0xFE: {
// 			PlatformLogInstruction(byte, "CP A, n8");
// 			i8 n8 = (i8)read8(state->cpu.regs[PC].full + 1);
// 			cp_A_n8(n8);
// 		} break;
// 		case 0xFF:
// 			PlatformLogInstruction(byte, "RST $38");
// 			rst(0x38);
// 			break;
// 		default:
// 			PlatformLogInstruction(byte, "INVALID OPCODE");
// 			state->cpu.regs[PC].full += 1;
// 			state->cpu.cycle += 1;
// 			break;
// 		}
// 	}
// }
