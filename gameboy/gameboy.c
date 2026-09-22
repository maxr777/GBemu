#include "gameboy.h"
#include "../misc/types.h"
#include "../misc/utils.h"
#include "../platform/platform.h"
#include <stdbool.h>

// Bootix bootrom: https://github.com/Hacktix/Bootix
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

static int tac_cycles_get(const u8 tac) {
	switch (tac & 0x03) {
	case 0x00:
		return TAC_00_CYCLES;
	case 0x01:
		return TAC_01_CYCLES;
	case 0x02:
		return TAC_10_CYCLES;
	case 0x03:
		return TAC_11_CYCLES;
	}

	return 0;
}

static bool tac_enable_get(const u8 tac) {
	return tac & 0x04;
}

static void gameboy_initialize(const char *filepath, Gameboy *gb) {
	// Deterministic baseline: real WRAM/HRAM and wave RAM are undefined on power-up.
	gb_memset(gb, 0, sizeof(*gb));

	platform_game_load(filepath, &gb->rom);

	// DMG power-on state, BEFORE the boot ROM runs at PC = 0x0000.
	// The boot ROM establishes SP, CPU registers, LCDC, BGP, and sound settings.
	// Do not copy the post-boot (PC = 0x0100) values here.
	// https://gbdev.io/pandocs/Power_Up_Sequence.html
	gb->rom.boot_rom = BOOT_ROM;
	gb->rom.boot_rom_enabled = true;
	gb->rom.current_rom_bank = 1;
	gb->mbc1.first_rom_bank_reg = 1;
	// RAM bank 0, RAM disabled, and simple MBC1 banking mode remain zero.

	u8 *io = gb->memory.io_registers;
	io[JOYPAD_INPUT - IO_REGS_ADDR] = 0xCF;	  // No buttons pressed; both groups selected.
	io[SERIAL_CONTROL - IO_REGS_ADDR] = 0x7E; // Transfer stopped, external clock (DMG).
	io[TAC_ADDR - IO_REGS_ADDR] = 0xF8;	  // Timer disabled, clock select 00.
	io[IF_ADDR - IO_REGS_ADDR] = 0xE0;	  // No pending interrupts.
	io[LCD_STATUS - IO_REGS_ADDR] = 0x80;	  // LCD off; bit 7 reads as one.
	// DIV, TIMA, TMA, and LCDC start at zero in this implementation.

	// Sound is powered off. These are read-as-one bits, not enabled features.
	// TODO: Enforce read masks in read8() when implementing each device so
	// subsequent writes cannot clear them. Initialization alone is insufficient.
	io[CH1_SWEEP - IO_REGS_ADDR] = 0x80;
	io[CH1_TIMER - IO_REGS_ADDR] = 0x3F;
	io[CH1_PERIOD_LOW - IO_REGS_ADDR] = 0xFF;
	io[CH1_PERIOD_HIGH - IO_REGS_ADDR] = 0xBF;
	io[CH2_TIMER - IO_REGS_ADDR] = 0x3F;
	io[CH2_PERIOD_LOW - IO_REGS_ADDR] = 0xFF;
	io[CH2_PERIOD_HIGH - IO_REGS_ADDR] = 0xBF;
	io[CH3_DAC_ENABLE - IO_REGS_ADDR] = 0x7F;
	io[CH3_LEN_TIMER - IO_REGS_ADDR] = 0xFF;
	io[CH3_OUTPUT_LVL - IO_REGS_ADDR] = 0x9F;
	io[CH3_PERIOD_LOW - IO_REGS_ADDR] = 0xFF;
	io[CH3_PERIOD_HIGH - IO_REGS_ADDR] = 0xBF;
	io[CH4_LEN_TIMER - IO_REGS_ADDR] = 0xFF;
	io[CH4_CONTROL - IO_REGS_ADDR] = 0xBF;
	io[AUDIO_CONTROL - IO_REGS_ADDR] = 0x70;

	// Undefined on real DMG hardware; use a common, deterministic startup value.
	io[0xFF46 - IO_REGS_ADDR] = 0xFF; // OAM DMA register (does not start a transfer).
	io[OBJ_PALETTE0_DATA - IO_REGS_ADDR] = 0xFF;
	io[OBJ_PALETTE1_DATA - IO_REGS_ADDR] = 0xFF;
}

static void opcode_execute(const u8 opcode, Gameboy *gb) {
	if (gb->cpu.prefix) {
		// ==================== PREFIX ====================
		switch (opcode) {
		case 0x00:
			rlc_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x01:
			rlc_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x02:
			rlc_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x03:
			rlc_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x04:
			rlc_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x05:
			rlc_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x06:
			rlc_aHL(gb);
			break;
		case 0x07:
			rlc_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x08:
			rrc_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x09:
			rrc_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x0A:
			rrc_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x0B:
			rrc_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x0C:
			rrc_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x0D:
			rrc_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x0E:
			rrc_aHL(gb);
			break;
		case 0x0F:
			rrc_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x10:
			rl_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x11:
			rl_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x12:
			rl_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x13:
			rl_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x14:
			rl_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x15:
			rl_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x16:
			rl_aHL(gb);
			break;
		case 0x17:
			rl_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x18:
			rr_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x19:
			rr_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x1A:
			rr_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x1B:
			rr_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x1C:
			rr_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x1D:
			rr_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x1E:
			rr_aHL(gb);
			break;
		case 0x1F:
			rr_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x20:
			sla_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x21:
			sla_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x22:
			sla_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x23:
			sla_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x24:
			sla_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x25:
			sla_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x26:
			sla_aHL(gb);
			break;
		case 0x27:
			sla_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x28:
			sra_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x29:
			sra_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x2A:
			sra_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x2B:
			sra_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x2C:
			sra_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x2D:
			sra_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x2E:
			sra_aHL(gb);
			break;
		case 0x2F:
			sra_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x30:
			swap_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x31:
			swap_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x32:
			swap_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x33:
			swap_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x34:
			swap_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x35:
			swap_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x36:
			swap_aHL(gb);
			break;
		case 0x37:
			swap_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x38:
			srl_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x39:
			srl_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x3A:
			srl_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x3B:
			srl_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x3C:
			srl_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x3D:
			srl_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x3E:
			srl_aHL(gb);
			break;
		case 0x3F:
			srl_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x40:
			bit_u3_r8(gb, 0, gb->cpu.regs[BC].high);
			break;
		case 0x41:
			bit_u3_r8(gb, 0, gb->cpu.regs[BC].low);
			break;
		case 0x42:
			bit_u3_r8(gb, 0, gb->cpu.regs[DE].high);
			break;
		case 0x43:
			bit_u3_r8(gb, 0, gb->cpu.regs[DE].low);
			break;
		case 0x44:
			bit_u3_r8(gb, 0, gb->cpu.regs[HL].high);
			break;
		case 0x45:
			bit_u3_r8(gb, 0, gb->cpu.regs[HL].low);
			break;
		case 0x46:
			bit_u3_aHL(gb, 0);
			break;
		case 0x47:
			bit_u3_r8(gb, 0, gb->cpu.regs[AF].high);
			break;
		case 0x48:
			bit_u3_r8(gb, 1, gb->cpu.regs[BC].high);
			break;
		case 0x49:
			bit_u3_r8(gb, 1, gb->cpu.regs[BC].low);
			break;
		case 0x4A:
			bit_u3_r8(gb, 1, gb->cpu.regs[DE].high);
			break;
		case 0x4B:
			bit_u3_r8(gb, 1, gb->cpu.regs[DE].low);
			break;
		case 0x4C:
			bit_u3_r8(gb, 1, gb->cpu.regs[HL].high);
			break;
		case 0x4D:
			bit_u3_r8(gb, 1, gb->cpu.regs[HL].low);
			break;
		case 0x4E:
			bit_u3_aHL(gb, 1);
			break;
		case 0x4F:
			bit_u3_r8(gb, 1, gb->cpu.regs[AF].high);
			break;
		case 0x50:
			bit_u3_r8(gb, 2, gb->cpu.regs[BC].high);
			break;
		case 0x51:
			bit_u3_r8(gb, 2, gb->cpu.regs[BC].low);
			break;
		case 0x52:
			bit_u3_r8(gb, 2, gb->cpu.regs[DE].high);
			break;
		case 0x53:
			bit_u3_r8(gb, 2, gb->cpu.regs[DE].low);
			break;
		case 0x54:
			bit_u3_r8(gb, 2, gb->cpu.regs[HL].high);
			break;
		case 0x55:
			bit_u3_r8(gb, 2, gb->cpu.regs[HL].low);
			break;
		case 0x56:
			bit_u3_aHL(gb, 2);
			break;
		case 0x57:
			bit_u3_r8(gb, 2, gb->cpu.regs[AF].high);
			break;
		case 0x58:
			bit_u3_r8(gb, 3, gb->cpu.regs[BC].high);
			break;
		case 0x59:
			bit_u3_r8(gb, 3, gb->cpu.regs[BC].low);
			break;
		case 0x5A:
			bit_u3_r8(gb, 3, gb->cpu.regs[DE].high);
			break;
		case 0x5B:
			bit_u3_r8(gb, 3, gb->cpu.regs[DE].low);
			break;
		case 0x5C:
			bit_u3_r8(gb, 3, gb->cpu.regs[HL].high);
			break;
		case 0x5D:
			bit_u3_r8(gb, 3, gb->cpu.regs[HL].low);
			break;
		case 0x5E:
			bit_u3_aHL(gb, 3);
			break;
		case 0x5F:
			bit_u3_r8(gb, 3, gb->cpu.regs[AF].high);
			break;
		case 0x60:
			bit_u3_r8(gb, 4, gb->cpu.regs[BC].high);
			break;
		case 0x61:
			bit_u3_r8(gb, 4, gb->cpu.regs[BC].low);
			break;
		case 0x62:
			bit_u3_r8(gb, 4, gb->cpu.regs[DE].high);
			break;
		case 0x63:
			bit_u3_r8(gb, 4, gb->cpu.regs[DE].low);
			break;
		case 0x64:
			bit_u3_r8(gb, 4, gb->cpu.regs[HL].high);
			break;
		case 0x65:
			bit_u3_r8(gb, 4, gb->cpu.regs[HL].low);
			break;
		case 0x66:
			bit_u3_aHL(gb, 4);
			break;
		case 0x67:
			bit_u3_r8(gb, 4, gb->cpu.regs[AF].high);
			break;
		case 0x68:
			bit_u3_r8(gb, 5, gb->cpu.regs[BC].high);
			break;
		case 0x69:
			bit_u3_r8(gb, 5, gb->cpu.regs[BC].low);
			break;
		case 0x6A:
			bit_u3_r8(gb, 5, gb->cpu.regs[DE].high);
			break;
		case 0x6B:
			bit_u3_r8(gb, 5, gb->cpu.regs[DE].low);
			break;
		case 0x6C:
			bit_u3_r8(gb, 5, gb->cpu.regs[HL].high);
			break;
		case 0x6D:
			bit_u3_r8(gb, 5, gb->cpu.regs[HL].low);
			break;
		case 0x6E:
			bit_u3_aHL(gb, 5);
			break;
		case 0x6F:
			bit_u3_r8(gb, 5, gb->cpu.regs[AF].high);
			break;
		case 0x70:
			bit_u3_r8(gb, 6, gb->cpu.regs[BC].high);
			break;
		case 0x71:
			bit_u3_r8(gb, 6, gb->cpu.regs[BC].low);
			break;
		case 0x72:
			bit_u3_r8(gb, 6, gb->cpu.regs[DE].high);
			break;
		case 0x73:
			bit_u3_r8(gb, 6, gb->cpu.regs[DE].low);
			break;
		case 0x74:
			bit_u3_r8(gb, 6, gb->cpu.regs[HL].high);
			break;
		case 0x75:
			bit_u3_r8(gb, 6, gb->cpu.regs[HL].low);
			break;
		case 0x76:
			bit_u3_aHL(gb, 6);
			break;
		case 0x77:
			bit_u3_r8(gb, 6, gb->cpu.regs[AF].high);
			break;
		case 0x78:
			bit_u3_r8(gb, 7, gb->cpu.regs[BC].high);
			break;
		case 0x79:
			bit_u3_r8(gb, 7, gb->cpu.regs[BC].low);
			break;
		case 0x7A:
			bit_u3_r8(gb, 7, gb->cpu.regs[DE].high);
			break;
		case 0x7B:
			bit_u3_r8(gb, 7, gb->cpu.regs[DE].low);
			break;
		case 0x7C:
			bit_u3_r8(gb, 7, gb->cpu.regs[HL].high);
			break;
		case 0x7D:
			bit_u3_r8(gb, 7, gb->cpu.regs[HL].low);
			break;
		case 0x7E:
			bit_u3_aHL(gb, 7);
			break;
		case 0x7F:
			bit_u3_r8(gb, 7, gb->cpu.regs[AF].high);
			break;
		case 0x80:
			res_u3_r8(gb, 0, &gb->cpu.regs[BC].high);
			break;
		case 0x81:
			res_u3_r8(gb, 0, &gb->cpu.regs[BC].low);
			break;
		case 0x82:
			res_u3_r8(gb, 0, &gb->cpu.regs[DE].high);
			break;
		case 0x83:
			res_u3_r8(gb, 0, &gb->cpu.regs[DE].low);
			break;
		case 0x84:
			res_u3_r8(gb, 0, &gb->cpu.regs[HL].high);
			break;
		case 0x85:
			res_u3_r8(gb, 0, &gb->cpu.regs[HL].low);
			break;
		case 0x86:
			res_u3_aHL(gb, 0);
			break;
		case 0x87:
			res_u3_r8(gb, 0, &gb->cpu.regs[AF].high);
			break;
		case 0x88:
			res_u3_r8(gb, 1, &gb->cpu.regs[BC].high);
			break;
		case 0x89:
			res_u3_r8(gb, 1, &gb->cpu.regs[BC].low);
			break;
		case 0x8A:
			res_u3_r8(gb, 1, &gb->cpu.regs[DE].high);
			break;
		case 0x8B:
			res_u3_r8(gb, 1, &gb->cpu.regs[DE].low);
			break;
		case 0x8C:
			res_u3_r8(gb, 1, &gb->cpu.regs[HL].high);
			break;
		case 0x8D:
			res_u3_r8(gb, 1, &gb->cpu.regs[HL].low);
			break;
		case 0x8E:
			res_u3_aHL(gb, 1);
			break;
		case 0x8F:
			res_u3_r8(gb, 1, &gb->cpu.regs[AF].high);
			break;
		case 0x90:
			res_u3_r8(gb, 2, &gb->cpu.regs[BC].high);
			break;
		case 0x91:
			res_u3_r8(gb, 2, &gb->cpu.regs[BC].low);
			break;
		case 0x92:
			res_u3_r8(gb, 2, &gb->cpu.regs[DE].high);
			break;
		case 0x93:
			res_u3_r8(gb, 2, &gb->cpu.regs[DE].low);
			break;
		case 0x94:
			res_u3_r8(gb, 2, &gb->cpu.regs[HL].high);
			break;
		case 0x95:
			res_u3_r8(gb, 2, &gb->cpu.regs[HL].low);
			break;
		case 0x96:
			res_u3_aHL(gb, 2);
			break;
		case 0x97:
			res_u3_r8(gb, 2, &gb->cpu.regs[AF].high);
			break;
		case 0x98:
			res_u3_r8(gb, 3, &gb->cpu.regs[BC].high);
			break;
		case 0x99:
			res_u3_r8(gb, 3, &gb->cpu.regs[BC].low);
			break;
		case 0x9A:
			res_u3_r8(gb, 3, &gb->cpu.regs[DE].high);
			break;
		case 0x9B:
			res_u3_r8(gb, 3, &gb->cpu.regs[DE].low);
			break;
		case 0x9C:
			res_u3_r8(gb, 3, &gb->cpu.regs[HL].high);
			break;
		case 0x9D:
			res_u3_r8(gb, 3, &gb->cpu.regs[HL].low);
			break;
		case 0x9E:
			res_u3_aHL(gb, 3);
			break;
		case 0x9F:
			res_u3_r8(gb, 3, &gb->cpu.regs[AF].high);
			break;
		case 0xA0:
			res_u3_r8(gb, 4, &gb->cpu.regs[BC].high);
			break;
		case 0xA1:
			res_u3_r8(gb, 4, &gb->cpu.regs[BC].low);
			break;
		case 0xA2:
			res_u3_r8(gb, 4, &gb->cpu.regs[DE].high);
			break;
		case 0xA3:
			res_u3_r8(gb, 4, &gb->cpu.regs[DE].low);
			break;
		case 0xA4:
			res_u3_r8(gb, 4, &gb->cpu.regs[HL].high);
			break;
		case 0xA5:
			res_u3_r8(gb, 4, &gb->cpu.regs[HL].low);
			break;
		case 0xA6:
			res_u3_aHL(gb, 4);
			break;
		case 0xA7:
			res_u3_r8(gb, 4, &gb->cpu.regs[AF].high);
			break;
		case 0xA8:
			res_u3_r8(gb, 5, &gb->cpu.regs[BC].high);
			break;
		case 0xA9:
			res_u3_r8(gb, 5, &gb->cpu.regs[BC].low);
			break;
		case 0xAA:
			res_u3_r8(gb, 5, &gb->cpu.regs[DE].high);
			break;
		case 0xAB:
			res_u3_r8(gb, 5, &gb->cpu.regs[DE].low);
			break;
		case 0xAC:
			res_u3_r8(gb, 5, &gb->cpu.regs[HL].high);
			break;
		case 0xAD:
			res_u3_r8(gb, 5, &gb->cpu.regs[HL].low);
			break;
		case 0xAE:
			res_u3_aHL(gb, 5);
			break;
		case 0xAF:
			res_u3_r8(gb, 5, &gb->cpu.regs[AF].high);
			break;
		case 0xB0:
			res_u3_r8(gb, 6, &gb->cpu.regs[BC].high);
			break;
		case 0xB1:
			res_u3_r8(gb, 6, &gb->cpu.regs[BC].low);
			break;
		case 0xB2:
			res_u3_r8(gb, 6, &gb->cpu.regs[DE].high);
			break;
		case 0xB3:
			res_u3_r8(gb, 6, &gb->cpu.regs[DE].low);
			break;
		case 0xB4:
			res_u3_r8(gb, 6, &gb->cpu.regs[HL].high);
			break;
		case 0xB5:
			res_u3_r8(gb, 6, &gb->cpu.regs[HL].low);
			break;
		case 0xB6:
			res_u3_aHL(gb, 6);
			break;
		case 0xB7:
			res_u3_r8(gb, 6, &gb->cpu.regs[AF].high);
			break;
		case 0xB8:
			res_u3_r8(gb, 7, &gb->cpu.regs[BC].high);
			break;
		case 0xB9:
			res_u3_r8(gb, 7, &gb->cpu.regs[BC].low);
			break;
		case 0xBA:
			res_u3_r8(gb, 7, &gb->cpu.regs[DE].high);
			break;
		case 0xBB:
			res_u3_r8(gb, 7, &gb->cpu.regs[DE].low);
			break;
		case 0xBC:
			res_u3_r8(gb, 7, &gb->cpu.regs[HL].high);
			break;
		case 0xBD:
			res_u3_r8(gb, 7, &gb->cpu.regs[HL].low);
			break;
		case 0xBE:
			res_u3_aHL(gb, 7);
			break;
		case 0xBF:
			res_u3_r8(gb, 7, &gb->cpu.regs[AF].high);
			break;
		case 0xC0:
			set_u3_r8(gb, 0, &gb->cpu.regs[BC].high);
			break;
		case 0xC1:
			set_u3_r8(gb, 0, &gb->cpu.regs[BC].low);
			break;
		case 0xC2:
			set_u3_r8(gb, 0, &gb->cpu.regs[DE].high);
			break;
		case 0xC3:
			set_u3_r8(gb, 0, &gb->cpu.regs[DE].low);
			break;
		case 0xC4:
			set_u3_r8(gb, 0, &gb->cpu.regs[HL].high);
			break;
		case 0xC5:
			set_u3_r8(gb, 0, &gb->cpu.regs[HL].low);
			break;
		case 0xC6:
			set_u3_aHL(gb, 0);
			break;
		case 0xC7:
			set_u3_r8(gb, 0, &gb->cpu.regs[AF].high);
			break;
		case 0xC8:
			set_u3_r8(gb, 1, &gb->cpu.regs[BC].high);
			break;
		case 0xC9:
			set_u3_r8(gb, 1, &gb->cpu.regs[BC].low);
			break;
		case 0xCA:
			set_u3_r8(gb, 1, &gb->cpu.regs[DE].high);
			break;
		case 0xCB:
			set_u3_r8(gb, 1, &gb->cpu.regs[DE].low);
			break;
		case 0xCC:
			set_u3_r8(gb, 1, &gb->cpu.regs[HL].high);
			break;
		case 0xCD:
			set_u3_r8(gb, 1, &gb->cpu.regs[HL].low);
			break;
		case 0xCE:
			set_u3_aHL(gb, 1);
			break;
		case 0xCF:
			set_u3_r8(gb, 1, &gb->cpu.regs[AF].high);
			break;
		case 0xD0:
			set_u3_r8(gb, 2, &gb->cpu.regs[BC].high);
			break;
		case 0xD1:
			set_u3_r8(gb, 2, &gb->cpu.regs[BC].low);
			break;
		case 0xD2:
			set_u3_r8(gb, 2, &gb->cpu.regs[DE].high);
			break;
		case 0xD3:
			set_u3_r8(gb, 2, &gb->cpu.regs[DE].low);
			break;
		case 0xD4:
			set_u3_r8(gb, 2, &gb->cpu.regs[HL].high);
			break;
		case 0xD5:
			set_u3_r8(gb, 2, &gb->cpu.regs[HL].low);
			break;
		case 0xD6:
			set_u3_aHL(gb, 2);
			break;
		case 0xD7:
			set_u3_r8(gb, 2, &gb->cpu.regs[AF].high);
			break;
		case 0xD8:
			set_u3_r8(gb, 3, &gb->cpu.regs[BC].high);
			break;
		case 0xD9:
			set_u3_r8(gb, 3, &gb->cpu.regs[BC].low);
			break;
		case 0xDA:
			set_u3_r8(gb, 3, &gb->cpu.regs[DE].high);
			break;
		case 0xDB:
			set_u3_r8(gb, 3, &gb->cpu.regs[DE].low);
			break;
		case 0xDC:
			set_u3_r8(gb, 3, &gb->cpu.regs[HL].high);
			break;
		case 0xDD:
			set_u3_r8(gb, 3, &gb->cpu.regs[HL].low);
			break;
		case 0xDE:
			set_u3_aHL(gb, 3);
			break;
		case 0xDF:
			set_u3_r8(gb, 3, &gb->cpu.regs[AF].high);
			break;
		case 0xE0:
			set_u3_r8(gb, 4, &gb->cpu.regs[BC].high);
			break;
		case 0xE1:
			set_u3_r8(gb, 4, &gb->cpu.regs[BC].low);
			break;
		case 0xE2:
			set_u3_r8(gb, 4, &gb->cpu.regs[DE].high);
			break;
		case 0xE3:
			set_u3_r8(gb, 4, &gb->cpu.regs[DE].low);
			break;
		case 0xE4:
			set_u3_r8(gb, 4, &gb->cpu.regs[HL].high);
			break;
		case 0xE5:
			set_u3_r8(gb, 4, &gb->cpu.regs[HL].low);
			break;
		case 0xE6:
			set_u3_aHL(gb, 4);
			break;
		case 0xE7:
			set_u3_r8(gb, 4, &gb->cpu.regs[AF].high);
			break;
		case 0xE8:
			set_u3_r8(gb, 5, &gb->cpu.regs[BC].high);
			break;
		case 0xE9:
			set_u3_r8(gb, 5, &gb->cpu.regs[BC].low);
			break;
		case 0xEA:
			set_u3_r8(gb, 5, &gb->cpu.regs[DE].high);
			break;
		case 0xEB:
			set_u3_r8(gb, 5, &gb->cpu.regs[DE].low);
			break;
		case 0xEC:
			set_u3_r8(gb, 5, &gb->cpu.regs[HL].high);
			break;
		case 0xED:
			set_u3_r8(gb, 5, &gb->cpu.regs[HL].low);
			break;
		case 0xEE:
			set_u3_aHL(gb, 5);
			break;
		case 0xEF:
			set_u3_r8(gb, 5, &gb->cpu.regs[AF].high);
			break;
		case 0xF0:
			set_u3_r8(gb, 6, &gb->cpu.regs[BC].high);
			break;
		case 0xF1:
			set_u3_r8(gb, 6, &gb->cpu.regs[BC].low);
			break;
		case 0xF2:
			set_u3_r8(gb, 6, &gb->cpu.regs[DE].high);
			break;
		case 0xF3:
			set_u3_r8(gb, 6, &gb->cpu.regs[DE].low);
			break;
		case 0xF4:
			set_u3_r8(gb, 6, &gb->cpu.regs[HL].high);
			break;
		case 0xF5:
			set_u3_r8(gb, 6, &gb->cpu.regs[HL].low);
			break;
		case 0xF6:
			set_u3_aHL(gb, 6);
			break;
		case 0xF7:
			set_u3_r8(gb, 6, &gb->cpu.regs[AF].high);
			break;
		case 0xF8:
			set_u3_r8(gb, 7, &gb->cpu.regs[BC].high);
			break;
		case 0xF9:
			set_u3_r8(gb, 7, &gb->cpu.regs[BC].low);
			break;
		case 0xFA:
			set_u3_r8(gb, 7, &gb->cpu.regs[DE].high);
			break;
		case 0xFB:
			set_u3_r8(gb, 7, &gb->cpu.regs[DE].low);
			break;
		case 0xFC:
			set_u3_r8(gb, 7, &gb->cpu.regs[HL].high);
			break;
		case 0xFD:
			set_u3_r8(gb, 7, &gb->cpu.regs[HL].low);
			break;
		case 0xFE:
			set_u3_aHL(gb, 7);
			break;
		case 0xFF:
			set_u3_r8(gb, 7, &gb->cpu.regs[AF].high);
			break;

		default:
			gb->cpu.regs[PC].full += 1;
			gb->cpu.cycle += 1;
			break;
		}
		gb->cpu.prefix = false;
	} else {
		// ==================== NON-PREFIX ====================
		switch (opcode) {
		case 0x00:
			nop(gb);
			break;
		case 0x01: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_r16_n16(gb, &gb->cpu.regs[BC].full, n16);
		} break;
		case 0x02:
			ld_a16_A(gb, gb->cpu.regs[BC].full);
			break;
		case 0x03:
			inc_r16(gb, &gb->cpu.regs[BC].full);
			break;
		case 0x04:
			inc_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x05:
			dec_r8(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x06: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[BC].high, n8);
		} break;
		case 0x07:
			rlca(gb);
			break;
		case 0x08: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_addr16_SP(gb, n16);
		} break;
		case 0x09:
			add_HL_r16(gb, gb->cpu.regs[BC].full);
			break;
		case 0x0A:
			ld_A_a16(gb, gb->cpu.regs[BC].full);
			break;
		case 0x0B:
			dec_r16(gb, &gb->cpu.regs[BC].full);
			break;
		case 0x0C:
			inc_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x0D:
			dec_r8(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x0E: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[BC].low, n8);
		} break;
		case 0x0F:
			rrca(gb);
			break;
		case 0x10: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			stop_n8(gb, n8);
		} break;
		case 0x11: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_r16_n16(gb, &gb->cpu.regs[DE].full, n16);
		} break;
		case 0x12:
			ld_a16_A(gb, gb->cpu.regs[DE].full);
			break;
		case 0x13:
			inc_r16(gb, &gb->cpu.regs[DE].full);
			break;
		case 0x14:
			inc_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x15:
			dec_r8(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x16: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[DE].high, n8);
		} break;
		case 0x17:
			rla(gb);
			break;
		case 0x18: {
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_n16(gb, offset);
			break;
		}
		case 0x19:
			add_HL_r16(gb, gb->cpu.regs[DE].full);
			break;
		case 0x1A:
			ld_A_a16(gb, gb->cpu.regs[DE].full);
			break;
		case 0x1B:
			dec_r16(gb, &gb->cpu.regs[DE].full);
			break;
		case 0x1C:
			inc_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x1D:
			dec_r8(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x1E: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[DE].low, n8);
		} break;
		case 0x1F:
			rra(gb);
			break;
		case 0x20: {
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_cc_n16(gb, Z, false, offset);
			break;
		}
		case 0x21: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_r16_n16(gb, &gb->cpu.regs[HL].full, n16);
		} break;
		case 0x22:
			ld_aHLi_A(gb);
			break;
		case 0x23:
			inc_r16(gb, &gb->cpu.regs[HL].full);
			break;
		case 0x24:
			inc_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x25:
			dec_r8(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x26: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[HL].high, n8);
		} break;
		case 0x27:
			daa(gb);
			break;
		case 0x28: {
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_cc_n16(gb, Z, true, offset);
			break;
		}
		case 0x29:
			add_HL_r16(gb, gb->cpu.regs[HL].full);
			break;
		case 0x2A:
			ld_A_aHLi(gb);
			break;
		case 0x2B:
			dec_r16(gb, &gb->cpu.regs[HL].full);
			break;
		case 0x2C:
			inc_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x2D:
			dec_r8(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x2E: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[HL].low, n8);
		} break;
		case 0x2F:
			cpl(gb);
			break;
		case 0x30: {
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_cc_n16(gb, C, false, offset);
			break;
		}
		case 0x31: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_r16_n16(gb, &gb->cpu.regs[SP].full, n16);
		} break;
		case 0x32:
			ld_aHLd_A(gb);
			break;
		case 0x33:
			inc_r16(gb, &gb->cpu.regs[SP].full);
			break;
		case 0x34:
			inc_aHL(gb);
			break;
		case 0x35:
			dec_aHL(gb);
			break;
		case 0x36: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_aHL_n8(gb, n8);
		} break;
		case 0x37:
			scf(gb);
			break;
		case 0x38: {
			i8 offset = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			jr_cc_n16(gb, C, true, offset);
			break;
		}
		case 0x39:
			add_HL_r16(gb, gb->cpu.regs[SP].full);
			break;
		case 0x3A:
			ld_A_aHLd(gb);
			break;
		case 0x3B:
			dec_r16(gb, &gb->cpu.regs[SP].full);
			break;
		case 0x3C:
			inc_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x3D:
			dec_r8(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x3E: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ld_r8_n8(gb, &gb->cpu.regs[AF].high, n8);
		} break;
		case 0x3F:
			ccf(gb);
			break;
		case 0x40:
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[BC].high);
			break;
		case 0x41:
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[BC].low);
			break;
		case 0x42:
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[DE].high);
			break;
		case 0x43:
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[DE].low);
			break;
		case 0x44:
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[HL].high);
			break;
		case 0x45:
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[HL].low);
			break;
		case 0x46:
			ld_r8_aHL(gb, &gb->cpu.regs[BC].high);
			break;
		case 0x47:
			ld_r8_r8(gb, &gb->cpu.regs[BC].high, gb->cpu.regs[AF].high);
			break;
		case 0x48:
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[BC].high);
			break;
		case 0x49:
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[BC].low);
			break;
		case 0x4A:
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[DE].high);
			break;
		case 0x4B:
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[DE].low);
			break;
		case 0x4C:
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[HL].high);
			break;
		case 0x4D:
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[HL].low);
			break;
		case 0x4E:
			ld_r8_aHL(gb, &gb->cpu.regs[BC].low);
			break;
		case 0x4F:
			ld_r8_r8(gb, &gb->cpu.regs[BC].low, gb->cpu.regs[AF].high);
			break;
		case 0x50:
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[BC].high);
			break;
		case 0x51:
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[BC].low);
			break;
		case 0x52:
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[DE].high);
			break;
		case 0x53:
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[DE].low);
			break;
		case 0x54:
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[HL].high);
			break;
		case 0x55:
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[HL].low);
			break;
		case 0x56:
			ld_r8_aHL(gb, &gb->cpu.regs[DE].high);
			break;
		case 0x57:
			ld_r8_r8(gb, &gb->cpu.regs[DE].high, gb->cpu.regs[AF].high);
			break;
		case 0x58:
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[BC].high);
			break;
		case 0x59:
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[BC].low);
			break;
		case 0x5A:
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[DE].high);
			break;
		case 0x5B:
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[DE].low);
			break;
		case 0x5C:
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[HL].high);
			break;
		case 0x5D:
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[HL].low);
			break;
		case 0x5E:
			ld_r8_aHL(gb, &gb->cpu.regs[DE].low);
			break;
		case 0x5F:
			ld_r8_r8(gb, &gb->cpu.regs[DE].low, gb->cpu.regs[AF].high);
			break;
		case 0x60:
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[BC].high);
			break;
		case 0x61:
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[BC].low);
			break;
		case 0x62:
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[DE].high);
			break;
		case 0x63:
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[DE].low);
			break;
		case 0x64:
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[HL].high);
			break;
		case 0x65:
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[HL].low);
			break;
		case 0x66:
			ld_r8_aHL(gb, &gb->cpu.regs[HL].high);
			break;
		case 0x67:
			ld_r8_r8(gb, &gb->cpu.regs[HL].high, gb->cpu.regs[AF].high);
			break;
		case 0x68:
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[BC].high);
			break;
		case 0x69:
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[BC].low);
			break;
		case 0x6A:
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[DE].high);
			break;
		case 0x6B:
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[DE].low);
			break;
		case 0x6C:
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[HL].high);
			break;
		case 0x6D:
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[HL].low);
			break;
		case 0x6E:
			ld_r8_aHL(gb, &gb->cpu.regs[HL].low);
			break;
		case 0x6F:
			ld_r8_r8(gb, &gb->cpu.regs[HL].low, gb->cpu.regs[AF].high);
			break;
		case 0x70:
			ld_aHL_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x71:
			ld_aHL_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x72:
			ld_aHL_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x73:
			ld_aHL_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x74:
			ld_aHL_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x75:
			ld_aHL_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x76:
			halt(gb);
			break;
		case 0x77:
			ld_aHL_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0x78:
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[BC].high);
			break;
		case 0x79:
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[BC].low);
			break;
		case 0x7A:
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[DE].high);
			break;
		case 0x7B:
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[DE].low);
			break;
		case 0x7C:
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[HL].high);
			break;
		case 0x7D:
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[HL].low);
			break;
		case 0x7E:
			ld_r8_aHL(gb, &gb->cpu.regs[AF].high);
			break;
		case 0x7F:
			ld_r8_r8(gb, &gb->cpu.regs[AF].high, gb->cpu.regs[AF].high);
			break;
		case 0x80:
			add_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x81:
			add_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x82:
			add_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x83:
			add_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x84:
			add_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x85:
			add_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x86:
			add_A_aHL(gb);
			break;
		case 0x87:
			add_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0x88:
			adc_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x89:
			adc_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x8A:
			adc_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x8B:
			adc_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x8C:
			adc_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x8D:
			adc_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x8E:
			adc_A_aHL(gb);
			break;
		case 0x8F:
			adc_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0x90:
			sub_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x91:
			sub_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x92:
			sub_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x93:
			sub_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x94:
			sub_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x95:
			sub_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x96:
			sub_A_aHL(gb);
			break;
		case 0x97:
			sub_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0x98:
			sbc_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0x99:
			sbc_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0x9A:
			sbc_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0x9B:
			sbc_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0x9C:
			sbc_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0x9D:
			sbc_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0x9E:
			sbc_A_aHL(gb);
			break;
		case 0x9F:
			sbc_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xA0:
			and_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0xA1:
			and_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0xA2:
			and_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0xA3:
			and_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0xA4:
			and_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0xA5:
			and_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0xA6:
			and_A_aHL(gb);
			break;
		case 0xA7:
			and_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xA8:
			xor_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0xA9:
			xor_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0xAA:
			xor_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0xAB:
			xor_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0xAC:
			xor_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0xAD:
			xor_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0xAE:
			xor_A_aHL(gb);
			break;
		case 0xAF:
			xor_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xB0:
			or_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0xB1:
			or_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0xB2:
			or_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0xB3:
			or_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0xB4:
			or_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0xB5:
			or_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0xB6:
			or_A_aHL(gb);
			break;
		case 0xB7:
			or_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xB8:
			cp_A_r8(gb, gb->cpu.regs[BC].high);
			break;
		case 0xB9:
			cp_A_r8(gb, gb->cpu.regs[BC].low);
			break;
		case 0xBA:
			cp_A_r8(gb, gb->cpu.regs[DE].high);
			break;
		case 0xBB:
			cp_A_r8(gb, gb->cpu.regs[DE].low);
			break;
		case 0xBC:
			cp_A_r8(gb, gb->cpu.regs[HL].high);
			break;
		case 0xBD:
			cp_A_r8(gb, gb->cpu.regs[HL].low);
			break;
		case 0xBE:
			cp_A_aHL(gb);
			break;
		case 0xBF:
			cp_A_r8(gb, gb->cpu.regs[AF].high);
			break;
		case 0xC0:
			ret_cc(gb, Z, false);
			break;
		case 0xC1:
			pop_r16(gb, &gb->cpu.regs[BC].full);
			break;
		case 0xC2: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_cc_n16(gb, Z, false, n16);
			break;
		}
		case 0xC3: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_n16(gb, n16);
			break;
		}
		case 0xC4: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_cc_n16(gb, Z, false, n16);
			break;
		}
		case 0xC5:
			push_r16(gb, gb->cpu.regs[BC].full);
			break;
		case 0xC6: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			add_A_n8(gb, n8);
			break;
		}
		case 0xC7:
			rst(gb, 0x00);
			break;
		case 0xC8:
			ret_cc(gb, Z, true);
			break;
		case 0xC9:
			ret(gb);
			break;
		case 0xCA: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_cc_n16(gb, Z, true, n16);
			break;
		}
		case 0xCB:
			gb->cpu.prefix = true;
			gb->cpu.regs[PC].full += 1;
			break;
		case 0xCC: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_cc_n16(gb, Z, true, n16);
			break;
		}
		case 0xCD: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_n16(gb, n16);
			break;
		}
		case 0xCE: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			adc_A_n8(gb, n8);
			break;
		}
		case 0xCF:
			rst(gb, 0x08);
			break;
		case 0xD0:
			ret_cc(gb, C, false);
			break;
		case 0xD1:
			pop_r16(gb, &gb->cpu.regs[DE].full);
			break;
		case 0xD2: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_cc_n16(gb, C, false, n16);
			break;
		}
		case 0xD4: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_cc_n16(gb, C, false, n16);
			break;
		}
		case 0xD5:
			push_r16(gb, gb->cpu.regs[DE].full);
			break;
		case 0xD6: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			sub_A_n8(gb, n8);
			break;
		}
		case 0xD7:
			rst(gb, 0x10);
			break;
		case 0xD8:
			ret_cc(gb, C, true);
			break;
		case 0xD9:
			reti(gb);
			break;
		case 0xDA: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			jp_cc_n16(gb, C, true, n16);
			break;
		}
		case 0xDC: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			call_cc_n16(gb, C, true, n16);
			break;
		}
		case 0xDE: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			sbc_A_n8(gb, n8);
			break;
		}
		case 0xDF:
			rst(gb, 0x18);
			break;
		case 0xE0: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ldh_addr16_A(gb, 0xFF00 + n8);
			break;
		}
		case 0xE1:
			pop_r16(gb, &gb->cpu.regs[HL].full);
			break;
		case 0xE2:
			ldh_aC_A(gb);
			break;
		case 0xE5:
			push_r16(gb, gb->cpu.regs[HL].full);
			break;
		case 0xE6: {
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			and_A_n8(gb, n8);
		} break;
		case 0xE7:
			rst(gb, 0x20);
			break;
		case 0xE8: {
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			add_SP_n8(gb, n8);
		} break;
		case 0xE9:
			jp_aHL(gb);
			break;
		case 0xEA: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_addr16_A(gb, n16);
			break;
		}
		case 0xEE: {
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			xor_A_n8(gb, n8);
		} break;
		case 0xEF:
			rst(gb, 0x28);
			break;
		case 0xF0: {
			u8 n8 = read8(gb, gb->cpu.regs[PC].full + 1);
			ldh_A_addr16(gb, 0xFF00 + n8);
			break;
		}
		case 0xF1:
			pop_AF(gb);
			break;
		case 0xF2:
			ldh_A_aC(gb);
			break;
		case 0xF3:
			di(gb);
			break;
		case 0xF5:
			push_AF(gb);
			break;
		case 0xF6: {
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			or_A_n8(gb, n8);
		} break;
		case 0xF7:
			rst(gb, 0x30);
			break;
		case 0xF8: {
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			ld_HL_SPe8(gb, n8);
		} break;
		case 0xF9:
			ld_SP_HL(gb);
			break;
		case 0xFA: {
			u16 n16 = read16(gb, gb->cpu.regs[PC].full + 1);
			ld_A_addr16(gb, n16);
			break;
		}
		case 0xFB:
			ei(gb);
			break;
		case 0xFE: {
			i8 n8 = (i8)read8(gb, gb->cpu.regs[PC].full + 1);
			cp_A_n8(gb, n8);
		} break;
		case 0xFF:
			rst(gb, 0x38);
			break;
		default:
			gb->cpu.regs[PC].full += 1;
			gb->cpu.cycle += 1;
			break;
		}
	}
}

static void gameboy_step(Gameboy *gb) {
	u64 cycle_pre = gb->cpu.cycle;
	bool ime_pending_old = gb->cpu.ime_pending;
	// Used for HALT handling; See interrupt_handle() for explanation on the ANDs
	bool int_pending = gb->memory.ie & gb->memory.io_registers[IF_ADDR - IO_REGS_ADDR] & 0x1F;

	// HALT handling, look at halt() for more details
	if (gb->cpu.halted && !int_pending) {
		/*
		 * If the IME flag is not set, and no interrupts are pending (halt = true, halt_bug = false):
		 * As soon as an interrupt becomes pending, the CPU resumes execution. This is like
		 * the above (note: refering to the else case), except that the handler is not called.
		 */
		++gb->cpu.cycle;
	} else {
		/*
		 * If the IME flag is set (halt = true, halt_bug = false):
		 * The CPU enters low-power mode until after an interrupt is about to be serviced.
		 * The handler is executed normally, and the CPU resumes execution after the HALT when that returns.
		 */
		gb->cpu.halted = false;
		if (!interrupt_handle(gb)) {
			u8 opcode = read8(gb, gb->cpu.regs[PC].full);
			/*
			 * If the IME flag is not set, and some interrupt is pending (halt = false, halt_bug = true):
			 * The CPU continues execution after the HALT, but the byte after it is
			 * read twice in a row (PC is not incremented, due to a hardware bug).
			 */
			if (gb->cpu.halt_bug) {
				gb->cpu.halt_bug = false;
				--gb->cpu.regs[PC].full;
			}

			opcode_execute(opcode, gb);

			/*
			 * "The effect of ei is delayed by one instruction. This means that
			 * ei followed immediately by di does not allow any interrupts between them."
			 *
			 * INSTRUCTIONS     PENDING_OLD     PENDING_NEW     RESULT
			 * EI; DI           true            false           IME stays off
			 * EI; EI           true            true            First EI takes effect
			 * EI; NOP          true            true            EI takes effect
			 *
			 * NOTE: EI; EI does not queue another interrupt, only 1 interrupt runs
			 */
			if (ime_pending_old && gb->cpu.ime_pending) {
				gb->cpu.ime = true;
				gb->cpu.ime_pending = false;
			}
		}
	}

	u64 cycles_elapsed = gb->cpu.cycle - cycle_pre;
	timer_advance(gb, cycles_elapsed);
}

static bool interrupt_handle(Gameboy *gb) {
	// - IF says which interrupts are requested, IE which are enabled
	// e.g. VBLANK and TIMER enabled, SERIAL and TIMER requested -> only TIMER interrupt is executed
	// - 0x1F because only bits 4-0 are used in both IF and IE; bits 7-5 are unused
	const u8 gb_if = read8(gb, IF_ADDR);
	const u8 gb_ef = read8(gb, IE_ADDR);
	u8 pending = gb_if & gb_ef & 0x1F;

	if (!pending || !gb->cpu.ime) return false;

	// In order: VBLANK, STAT, TIMER, SERIAL, JOYPAD
	static const u16 sources[] = {0x0040, 0x0048, 0x0050, 0x0058, 0x0060};

	int interrupt = 0;
	// - Get the source[] index of first pending interrupt to execute
	// sources[] is in order of importance, so first hit is the one to execute
	// - Even though it increments each time it's fine because only the most
	// important bit is what matters for the comparison
	while ((pending & (1u << interrupt)) == 0) ++interrupt;

	gb->cpu.ime_pending = false;
	gb->cpu.ime = false;

	// Clear the IF bit of executed interrupt
	write8(gb, IF_ADDR, gb_if & ~(1u << interrupt));

	// Store the PC on stack and jump to the interrupt's source address
	gb->cpu.regs[SP].full -= 2;
	write16(gb, gb->cpu.regs[SP].full, gb->cpu.regs[PC].full);
	gb->cpu.regs[PC].full = sources[interrupt];

	gb->cpu.cycle += 5;

	return true;
}

static void timer_advance(Gameboy *gb, const u64 cycles_elapsed) {
	u8 *div = &gb->memory.io_registers[DIV_ADDR - IO_REGS_ADDR];
	u64 *div_timer = &gb->timer.div_elapsed;

	*div_timer += cycles_elapsed;
	if (*div_timer >= CYCLES_PER_DIV) {
		++(*div);
		*div_timer %= cycles_elapsed;
	}

	u8 *tima = &gb->memory.io_registers[TIMA_ADDR - IO_REGS_ADDR];
	u8 *tma = &gb->memory.io_registers[TMA_ADDR - IO_REGS_ADDR];
	u8 *tac = &gb->memory.io_registers[TAC_ADDR - IO_REGS_ADDR];
	u64 *tima_timer = &gb->timer.tima_elapsed;

	int tac_increment = 0;
	// Bits 0 and 1 of tac set the frequency at which TIMA is incremented
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

	// Bit 2 of tac decides whether TIMA is incremented or not
	if (*tac & 0x04) {
		*tima_timer += cycles_elapsed;
		if (*tima_timer >= tac_increment) {
			if (*tima == 0xFF) {
				*tima = *tma;
				// TODO: request an interrupt
			} else {
				++(*tima);
			}
			*tima_timer %= cycles_elapsed;
		}
	}
}
