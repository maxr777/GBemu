#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "../misc/types.h"
#include "constants.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
	AF,
	BC,
	DE,
	HL,
	SP, // always accessed as 16-bits
	PC, // always accessed as 16-bits
	REGISTER_COUNT
} RegisterName;

typedef enum {
	Z, // zero flag
	N, // subtract flag
	H, // half carry flag
	C, // carry flag
	FLAG_COUNT
} Flags;

// BC = Full 16 bit register
// B = High 8 bits
// C = Low 8 bits
typedef union {
	u16 full;
	struct {
		u8 low;
		u8 high;
	};
} Register;

typedef struct {
	Register regs[REGISTER_COUNT];
	u64 cycle;
	bool prefix;
	bool ime;
	int ime_enable_counter;
} CPU;

typedef struct {
	u8 entry_point[4];
	u8 nintendo_logo[48];
	char title[16]; // already includes manufacturer code
	u8 cgb_flag;
	char new_license_code[2];
	u8 sgb_flag;
	u8 cartridge_type;
	u8 rom_size;
	u8 ram_size;
	u8 destination_code;
	u8 old_licensee_code;
	u8 mask_rom_version_number;
	u8 header_checksum;
	u16 global_checksum;
} CartridgeHeader;

typedef struct {
	u8 *game_rom;
	size_t game_size;
	CartridgeHeader cartridge_header;
	bool boot_rom_enabled;
	u8 *boot_rom;
	u8 current_rom_bank;
	int max_rom_banks;
	u8 current_ram_bank;
	// multiplied by 4 because of possible RAM banking, if no
	// RAM banking then we simply can't access anything past 8192
	u8 external_ram[EXTERN_RAM_SIZE * 4];
} ROM;

typedef struct {
#ifdef CPU_TEST
	u8 test_memory[0xFFFF + 1];
#endif
	u8 ram[8192]; // TODO: size change
	u8 vram[VRAM_SIZE];
	u8 io_registers[128];
	u8 oam[160];
	u8 hram[127];
	bool display[160][144];
} Memory;

typedef struct {
	CPU cpu;
	ROM rom;
	Memory memory;
} Gameboy;

// ==================== DECLARATIONS ====================

Gameboy gameboy_initialize(const char *filepath);

// ==================== CPU ====================

bool get_flag(const CPU *cpu, const int flag);
void set_flag(CPU *cpu, const int flag, const bool value);
void opcode_execute(const u8 opcode, Gameboy *gb, const bool debug);

// LOADS
void ld_r8_r8(Gameboy *gb, u8 *dest, const u8 src);
void ld_r8_n8(Gameboy *gb, u8 *dest, const u8 val);
void ld_r16_n16(Gameboy *gb, u16 *dest, const u16 val);
void ld_aHL_r8(Gameboy *gb, const u8 src);
void ld_aHL_n8(Gameboy *gb, const u8 val);
void ld_r8_aHL(Gameboy *gb, u8 *dest);
void ld_a16_A(Gameboy *gb, const u16 addr);
void ld_addr16_A(Gameboy *gb, const u16 addr);
void ldh_addr16_A(Gameboy *gb, const u16 addr);
void ldh_aC_A(Gameboy *gb);
void ld_A_a16(Gameboy *gb, const u16 addr);
void ld_A_addr16(Gameboy *gb, const u16 addr);
void ldh_A_addr16(Gameboy *gb, const u16 addr);
void ldh_A_aC(Gameboy *gb);
void ld_aHLi_A(Gameboy *gb);
void ld_aHLd_A(Gameboy *gb);
void ld_A_aHLi(Gameboy *gb);
void ld_A_aHLd(Gameboy *gb);

// 8-BIT ARITHMETIC
void add_A_r8(Gameboy *gb, const u8 src);
void add_A_aHL(Gameboy *gb);
void add_A_n8(Gameboy *gb, const u8 val);
void adc_A_r8(Gameboy *gb, const u8 src);
void adc_A_aHL(Gameboy *gb);
void adc_A_n8(Gameboy *gb, const u8 val);
void sub_A_r8(Gameboy *gb, const u8 src);
void sub_A_aHL(Gameboy *gb);
void sub_A_n8(Gameboy *gb, const u8 val);
void sbc_A_r8(Gameboy *gb, const u8 src);
void sbc_A_aHL(Gameboy *gb);
void sbc_A_n8(Gameboy *gb, const u8 val);
void dec_r8(Gameboy *gb, u8 *dest);
void dec_aHL(Gameboy *gb);
void inc_r8(Gameboy *gb, u8 *dest);
void inc_aHL(Gameboy *gb);
void cp_A_r8(Gameboy *gb, const u8 src);
void cp_A_aHL(Gameboy *gb);
void cp_A_n8(Gameboy *gb, const u8 val);

// 16-BIT ARITHMETIC
void add_HL_r16(Gameboy *gb, const u16 src);
void dec_r16(Gameboy *gb, u16 *dest);
void inc_r16(Gameboy *gb, u16 *dest);

// BITWISE LOGIC
void and_A_r8(Gameboy *gb, const u8 src);
void and_A_aHL(Gameboy *gb);
void and_A_n8(Gameboy *gb, const u8 val);
void or_A_r8(Gameboy *gb, const u8 src);
void or_A_aHL(Gameboy *gb);
void or_A_n8(Gameboy *gb, const u8 val);
void cpl(Gameboy *gb);
void xor_A_r8(Gameboy *gb, const u8 src);
void xor_A_aHL(Gameboy *gb);
void xor_A_n8(Gameboy *gb, const u8 val);

// BIT FLAGS
void bit_u3_r8(Gameboy *gb, const int bit_num, const u8 src);
void bit_u3_aHL(Gameboy *gb, const int bit_num);
void res_u3_r8(Gameboy *gb, const int bit_num, u8 *src);
void res_u3_aHL(Gameboy *gb, const int bit_num);
void set_u3_r8(Gameboy *gb, const int bit_num, u8 *src);
void set_u3_aHL(Gameboy *gb, const int bit_num);

// BIT SHIFTS
void rla(Gameboy *gb);
void rlca(Gameboy *gb);
void rra(Gameboy *gb);
void rrca(Gameboy *gb);
void rlc_r8(Gameboy *gb, u8 *src);
void rlc_aHL(Gameboy *gb);
void rrc_r8(Gameboy *gb, u8 *src);
void rrc_aHL(Gameboy *gb);
void rl_r8(Gameboy *gb, u8 *src);
void rl_aHL(Gameboy *gb);
void rr_r8(Gameboy *gb, u8 *src);
void rr_aHL(Gameboy *gb);
void sla_r8(Gameboy *gb, u8 *src);
void sla_aHL(Gameboy *gb);
void sra_r8(Gameboy *gb, u8 *src);
void sra_aHL(Gameboy *gb);
void swap_r8(Gameboy *gb, u8 *src);
void swap_aHL(Gameboy *gb);
void srl_r8(Gameboy *gb, u8 *src);
void srl_aHL(Gameboy *gb);

// JUMPS
void call_n16(Gameboy *gb, const u16 addr);
void call_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const u16 addr);
void jp_n16(Gameboy *gb, const u16 addr);
void jp_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const u16 addr);
void jp_aHL(Gameboy *gb);
void jr_n16(Gameboy *gb, const i8 offset);
void jr_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const i8 offset);
void rst(Gameboy *gb, const u8 vec);
void ret(Gameboy *gb);
void ret_cc(Gameboy *gb, const int flag, const bool flag_state);
void reti(Gameboy *gb);

// CARRY FLAG INSTRUCTIONS
void ccf(Gameboy *gb);
void scf(Gameboy *gb);

// STACK INSTRUCTIONS
void pop_r16(Gameboy *gb, u16 *src);
void pop_AF(Gameboy *gb);
void push_r16(Gameboy *gb, const u16 src);
void push_AF(Gameboy *gb);
void ld_addr16_SP(Gameboy *gb, const u16 addr);
void ld_HL_SPe8(Gameboy *gb, const i8 val);
void ld_SP_HL(Gameboy *gb);
void add_SP_n8(Gameboy *gb, const i8 val);

// INTERRUPTS
void di(Gameboy *gb);
void ei(Gameboy *gb);
void halt(Gameboy *gb);

// MISC
void daa(Gameboy *gb);
void nop(Gameboy *gb);
void stop_n8(Gameboy *gb, const u8 val);

// ==================== MEMORY ====================

void write16(Memory *mem, const u16 addr, const u16 val);
void write8(Memory *mem, const u16 addr, const u8 val);

u16 read16(const Memory *mem, const u16 addr);
u8 read8(const Memory *mem, const u16 addr);

#endif
