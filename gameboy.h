#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "types.h"
#include <stddef.h>

// =============================================

void platform_rom_load(const char *filename);
void platform_message_log(const char *msg);
void platform_screen_draw(u16 *framebuffer);

// =============================================

enum RegisterNames {
	AF,
	BC,
	DE,
	HL,
	SP, // always accessed as 16-bits
	PC, // always accessed as 16-bits
	REGISTER_COUNT
};

enum Flags {
	Z, // zero flag
	N, // subtract flag
	H, // half carry flag
	C, // carry flag
	FLAG_COUNT
};

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
	uint8_t entry_point[4];
	uint8_t nintendo_logo[48];
	char title[16]; // already includes manufacturer code
	uint8_t cgb_flag;
	uint16_t new_license_code;
	uint8_t sgb_flag;
	uint8_t cartridge_type;
	uint8_t rom_size;
	uint8_t ram_size;
	uint8_t destination_code;
	uint8_t old_licensee_code;
	uint8_t mask_rom_version_number;
	uint8_t header_checksum;
	uint16_t global_checksum;
} Cartridge_header;

typedef struct {
	uint8_t *game_rom;
	size_t game_size;
	Cartridge_header cartridge_header;
	bool boot_rom_enabled;
	const uint8_t boot_rom[256];
	uint8_t current_rom_bank;
	int max_rom_banks;
	uint8_t current_ram_bank;
	// multiplied by 4 because of possible RAM banking, if no
	// RAM banking then we simply can't access anything past 8192
	uint8_t external_ram[EXTERN_RAM_SIZE * 4];
} ROM;

typedef struct {
	CPU cpu;
	ROM rom;
} Gameboy;

#endif
