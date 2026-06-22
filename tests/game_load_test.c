#include "../gameboy/gameboy.h"
#include "../platform/desktop.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void check_game_load(const char *filepath, const ROM *expected) {
	ROM rom = {};
	platform_game_load(filepath, &rom);

	assert(rom.game_size == expected->game_size);
	assert(rom.max_rom_banks == expected->max_rom_banks);

	assert(!memcmp(rom.cartridge_header.entry_point, expected->cartridge_header.entry_point, sizeof(rom.cartridge_header.entry_point)));
	assert(!memcmp(rom.cartridge_header.nintendo_logo, expected->cartridge_header.nintendo_logo, sizeof(rom.cartridge_header.nintendo_logo)));
	assert(!memcmp((const u8 *)rom.cartridge_header.title, (const u8 *)expected->cartridge_header.title, sizeof(rom.cartridge_header.title)));

	assert(rom.cartridge_header.cgb_flag == expected->cartridge_header.cgb_flag);
	assert(rom.cartridge_header.new_license_code[0] == expected->cartridge_header.new_license_code[0]);
	assert(rom.cartridge_header.new_license_code[1] == expected->cartridge_header.new_license_code[1]);
	assert(rom.cartridge_header.sgb_flag == expected->cartridge_header.sgb_flag);
	assert(rom.cartridge_header.cartridge_type == expected->cartridge_header.cartridge_type);
	assert(rom.cartridge_header.rom_size == expected->cartridge_header.rom_size);
	assert(rom.cartridge_header.ram_size == expected->cartridge_header.ram_size);
	assert(rom.cartridge_header.destination_code == expected->cartridge_header.destination_code);
	assert(rom.cartridge_header.old_licensee_code == expected->cartridge_header.old_licensee_code);
	assert(rom.cartridge_header.mask_rom_version_number == expected->cartridge_header.mask_rom_version_number);
	assert(rom.cartridge_header.header_checksum == expected->cartridge_header.header_checksum);
	assert(rom.cartridge_header.global_checksum == expected->cartridge_header.global_checksum);

	assert(rom.boot_rom_enabled == expected->boot_rom_enabled);
	assert(rom.boot_rom == expected->boot_rom);
	assert(rom.game_rom != NULL);

	assert(rom.game_rom[0x0101] == 0xC3);
	assert(rom.game_rom[0x0134] == 'H');
	assert(rom.game_rom[0x013F] == '\0');
	assert(rom.game_rom[0x014F] == 0x77);
	assert(rom.game_rom[rom.game_size - 1] == 0x00);

	free(rom.game_rom);
}

// static void check_game_load_failure(const char *filepath, int expected_ret) {
// 	ROM rom = {};
// 	int ret_code = platform_game_load(filepath, &rom);
//
// 	assert(ret_code == expected_ret);
// 	assert(rom.game_rom == NULL);
// }

static void test_game_load(void) {
	puts("Test: Load a correct ROM");
	fflush(stdout);

	ROM expected = {};

	expected.game_size = 16384;
	expected.max_rom_banks = 2;

	expected.cartridge_header.entry_point[0] = 0x00;
	expected.cartridge_header.entry_point[1] = 0xC3;
	expected.cartridge_header.entry_point[2] = 0x50;
	expected.cartridge_header.entry_point[3] = 0x01;

	for (size_t i = 0; i < sizeof(expected.cartridge_header.nintendo_logo); ++i)
		expected.cartridge_header.nintendo_logo[i] = (u8)i;

	// \0\0\0\0\0 because the whole field is 11 bytes
	memcpy(expected.cartridge_header.title, "HEADER_TEST\0\0\0\0\0", sizeof(expected.cartridge_header.title));
	expected.cartridge_header.cgb_flag = 0x00;
	expected.cartridge_header.new_license_code[0] = '4';
	expected.cartridge_header.new_license_code[1] = 'Z';
	expected.cartridge_header.sgb_flag = 0x00;
	expected.cartridge_header.cartridge_type = 0x01;
	expected.cartridge_header.rom_size = 0x00;
	expected.cartridge_header.ram_size = 0x00;
	expected.cartridge_header.destination_code = 0x01;
	expected.cartridge_header.old_licensee_code = 0xA5;
	expected.cartridge_header.mask_rom_version_number = 0x03;
	expected.cartridge_header.header_checksum = 0x67;
	expected.cartridge_header.global_checksum = 0x770B;

	check_game_load("tests/ROMs/cartridge_header_test.gb", &expected);
}

static void test_game_load_missing(void) {
	puts("Test: Load a ROM that's missing");
	fflush(stdout);

	ROM rom = {};
	platform_game_load("tests/does_not_exist.gb", &rom);

	assert(rom.game_rom == NULL);
	assert(rom.game_size == 0);
	assert(rom.boot_rom == NULL);
}

static void test_game_load_too_small(void) {
	puts("Test: Load a ROM that's too small");
	fflush(stdout);

	const char *filepath = "tests/tmp_small_rom.gb";
	FILE *file = fopen(filepath, "wb");

	assert(file);
	for (int i = 0; i < 0x014F; ++i)
		fputc(0x00, file);
	fclose(file);

	ROM rom = {};
	platform_game_load(filepath, &rom);

	assert(rom.game_rom == NULL);
	assert(rom.game_size < 0x0150);
	assert(rom.boot_rom == NULL);

	assert(remove(filepath) == 0);
}
