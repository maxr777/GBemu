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
} Register_names;

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
	CPU cpu;
	ROM rom;
} Gameboy;

#endif
