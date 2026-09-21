#pragma once

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
	bool ram_enable;
	u8 first_rom_bank_reg;
	u8 second_rom_bank_reg;
	u8 ram_bank_number;
	bool banking_mode_is_advanced; // 0 = simple, 1 = advanced
} MBC1State;

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
	u8 oam[160];
	u8 io_registers[128];
	u8 hram[127];
	u8 display[160][144];
} Memory;

typedef struct {
	u64 div_elapsed;
	u64 tima_elapsed;
} TimerCounters;

typedef struct {
	CPU cpu;
	ROM rom;
	Memory memory;
	MBC1State mbc1;
	TimerCounters timer;
} Gameboy;

// ==================== DECLARATIONS ====================

static void gameboy_initialize(const char *filepath, Gameboy *gb);
static void gameboy_step(Gameboy *gb);

// ==================== CPU ====================

static bool get_flag(const CPU *cpu, const int flag);
static void set_flag(CPU *cpu, const int flag, const bool value);
static void opcode_execute(const u8 opcode, Gameboy *gb);

// LOADS
static void ld_r8_r8(Gameboy *gb, u8 *dest, const u8 src);
static void ld_r8_n8(Gameboy *gb, u8 *dest, const u8 val);
static void ld_r16_n16(Gameboy *gb, u16 *dest, const u16 val);
static void ld_aHL_r8(Gameboy *gb, const u8 src);
static void ld_aHL_n8(Gameboy *gb, const u8 val);
static void ld_r8_aHL(Gameboy *gb, u8 *dest);
static void ld_a16_A(Gameboy *gb, const u16 addr);
static void ld_addr16_A(Gameboy *gb, const u16 addr);
static void ldh_addr16_A(Gameboy *gb, const u16 addr);
static void ldh_aC_A(Gameboy *gb);
static void ld_A_a16(Gameboy *gb, const u16 addr);
static void ld_A_addr16(Gameboy *gb, const u16 addr);
static void ldh_A_addr16(Gameboy *gb, const u16 addr);
static void ldh_A_aC(Gameboy *gb);
static void ld_aHLi_A(Gameboy *gb);
static void ld_aHLd_A(Gameboy *gb);
static void ld_A_aHLi(Gameboy *gb);
static void ld_A_aHLd(Gameboy *gb);

// 8-BIT ARITHMETIC
static void add_A_r8(Gameboy *gb, const u8 src);
static void add_A_aHL(Gameboy *gb);
static void add_A_n8(Gameboy *gb, const u8 val);
static void adc_A_r8(Gameboy *gb, const u8 src);
static void adc_A_aHL(Gameboy *gb);
static void adc_A_n8(Gameboy *gb, const u8 val);
static void sub_A_r8(Gameboy *gb, const u8 src);
static void sub_A_aHL(Gameboy *gb);
static void sub_A_n8(Gameboy *gb, const u8 val);
static void sbc_A_r8(Gameboy *gb, const u8 src);
static void sbc_A_aHL(Gameboy *gb);
static void sbc_A_n8(Gameboy *gb, const u8 val);
static void dec_r8(Gameboy *gb, u8 *dest);
static void dec_aHL(Gameboy *gb);
static void inc_r8(Gameboy *gb, u8 *dest);
static void inc_aHL(Gameboy *gb);
static void cp_A_r8(Gameboy *gb, const u8 src);
static void cp_A_aHL(Gameboy *gb);
static void cp_A_n8(Gameboy *gb, const u8 val);

// 16-BIT ARITHMETIC
static void add_HL_r16(Gameboy *gb, const u16 src);
static void dec_r16(Gameboy *gb, u16 *dest);
static void inc_r16(Gameboy *gb, u16 *dest);

// BITWISE LOGIC
static void and_A_r8(Gameboy *gb, const u8 src);
static void and_A_aHL(Gameboy *gb);
static void and_A_n8(Gameboy *gb, const u8 val);
static void or_A_r8(Gameboy *gb, const u8 src);
static void or_A_aHL(Gameboy *gb);
static void or_A_n8(Gameboy *gb, const u8 val);
static void cpl(Gameboy *gb);
static void xor_A_r8(Gameboy *gb, const u8 src);
static void xor_A_aHL(Gameboy *gb);
static void xor_A_n8(Gameboy *gb, const u8 val);

// BIT FLAGS
static void bit_u3_r8(Gameboy *gb, const int bit_num, const u8 src);
static void bit_u3_aHL(Gameboy *gb, const int bit_num);
static void res_u3_r8(Gameboy *gb, const int bit_num, u8 *src);
static void res_u3_aHL(Gameboy *gb, const int bit_num);
static void set_u3_r8(Gameboy *gb, const int bit_num, u8 *src);
static void set_u3_aHL(Gameboy *gb, const int bit_num);

// BIT SHIFTS
static void rla(Gameboy *gb);
static void rlca(Gameboy *gb);
static void rra(Gameboy *gb);
static void rrca(Gameboy *gb);
static void rlc_r8(Gameboy *gb, u8 *src);
static void rlc_aHL(Gameboy *gb);
static void rrc_r8(Gameboy *gb, u8 *src);
static void rrc_aHL(Gameboy *gb);
static void rl_r8(Gameboy *gb, u8 *src);
static void rl_aHL(Gameboy *gb);
static void rr_r8(Gameboy *gb, u8 *src);
static void rr_aHL(Gameboy *gb);
static void sla_r8(Gameboy *gb, u8 *src);
static void sla_aHL(Gameboy *gb);
static void sra_r8(Gameboy *gb, u8 *src);
static void sra_aHL(Gameboy *gb);
static void swap_r8(Gameboy *gb, u8 *src);
static void swap_aHL(Gameboy *gb);
static void srl_r8(Gameboy *gb, u8 *src);
static void srl_aHL(Gameboy *gb);

// JUMPS
static void call_n16(Gameboy *gb, const u16 addr);
static void call_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const u16 addr);
static void jp_n16(Gameboy *gb, const u16 addr);
static void jp_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const u16 addr);
static void jp_aHL(Gameboy *gb);
static void jr_n16(Gameboy *gb, const i8 offset);
static void jr_cc_n16(Gameboy *gb, const int flag, const bool flag_state, const i8 offset);
static void rst(Gameboy *gb, const u8 vec);
static void ret(Gameboy *gb);
static void ret_cc(Gameboy *gb, const int flag, const bool flag_state);
static void reti(Gameboy *gb);

// CARRY FLAG INSTRUCTIONS
static void ccf(Gameboy *gb);
static void scf(Gameboy *gb);

// STACK INSTRUCTIONS
static void pop_r16(Gameboy *gb, u16 *src);
static void pop_AF(Gameboy *gb);
static void push_r16(Gameboy *gb, const u16 src);
static void push_AF(Gameboy *gb);
static void ld_addr16_SP(Gameboy *gb, const u16 addr);
static void ld_HL_SPe8(Gameboy *gb, const i8 val);
static void ld_SP_HL(Gameboy *gb);
static void add_SP_n8(Gameboy *gb, const i8 val);

// INTERRUPTS
static void di(Gameboy *gb);
static void ei(Gameboy *gb);
static void halt(Gameboy *gb);

// MISC
static void daa(Gameboy *gb);
static void nop(Gameboy *gb);
static void stop_n8(Gameboy *gb, const u8 val);

// ==================== MEMORY ====================

static void write16(Gameboy *gb, const u16 addr, const u16 val);
static void write8(Gameboy *gb, const u16 addr, const u8 val);
static void rom_write(Gameboy *gb, const u16 addr, const u8 val);
static void mbc1_write(Gameboy *gb, const u16 addr, const u8 val);

static u16 read16(const Gameboy *gb, const u16 addr);
static u8 read8(const Gameboy *gb, const u16 addr);
static u8 rom_read(const Gameboy *gb, const u16 addr);
static u8 mbc1_read(const Gameboy *gb, const u16 addr);

// ==================== TIMERS ====================

static int tac_cycles_get(const u8 tac);
static bool tac_enable_get(const u8 tac);
static void timer_advance(Gameboy *gb, const u64 cycles_elapsed);
