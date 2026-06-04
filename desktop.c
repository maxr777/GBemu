#include "gameboy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void platform_instruction_log(Gameboy *gb, const u8 opcode, const char *instr) {
	printf("Cycle: %lu\tPC: 0x%04X\tOpcode: 0x%02X\t%-12s\tAF: %04X\t\tBC: %04X\tDE: %04X\tHL: %04X\tSP: %04X\tFlags: %c%c%c%c\n",
	       gb->cpu.cycle, gb->cpu.regs[PC].full, opcode, instr,
	       gb->cpu.regs[AF].full, gb->cpu.regs[BC].full, gb->cpu.regs[DE].full, gb->cpu.regs[HL].full, gb->cpu.regs[SP].full,
	       (gb->cpu.regs[AF].low & 0x80) ? 'Z' : '-',
	       (gb->cpu.regs[AF].low & 0x40) ? 'N' : '-',
	       (gb->cpu.regs[AF].low & 0x20) ? 'H' : '-',
	       (gb->cpu.regs[AF].low & 0x10) ? 'C' : '-');
}

void platform_game_load(const char *filepath, ROM *rom) {
	memset(rom, 0, sizeof(*rom));

	FILE *game_file = fopen(filepath, "rb");
	if (!game_file) {
		perror("fopen");
		return;
	}

	// the cartridge header itself goes to 0x014F (inclusive),
	// so if the rom is smaller than 0x0150 it's not a correct ROM
	fseek(game_file, 0, SEEK_END);
	rom->game_size = ftell(game_file);
	if (rom->game_size < 0) {
		perror("ftell");
		fclose(game_file);
		return;
	}

	rewind(game_file);
	if (rom->game_size < 0x0150) {
		fprintf(stderr, "The ROM is too small\n");
		fclose(game_file);
		return;
	}

	rom->game_rom = malloc(rom->game_size);
	if (!rom->game_rom) {
		perror("malloc");
		fclose(game_file);
		return;
	}

	if (fread(rom->game_rom, sizeof(u8), rom->game_size, game_file) != rom->game_size) {
		perror("fread");
		fclose(game_file);
		return;
	}

	fclose(game_file);

	// for exact locations: https://gbdev.io/pandocs/The_Cartridge_Header.html
	memcpy(rom->cartridge_header.entry_point, &rom->game_rom[0x0100], sizeof(rom->cartridge_header.entry_point));
	memcpy(rom->cartridge_header.nintendo_logo, &rom->game_rom[0x0104], sizeof(rom->cartridge_header.nintendo_logo));
	memcpy(rom->cartridge_header.title, &rom->game_rom[0x0134], sizeof(rom->cartridge_header.title));
	memcpy(&rom->cartridge_header.cgb_flag, &rom->game_rom[0x0143], sizeof(rom->cartridge_header.cgb_flag));
	memcpy(&rom->cartridge_header.new_license_code, &rom->game_rom[0x0144], sizeof(rom->cartridge_header.new_license_code));
	memcpy(&rom->cartridge_header.sgb_flag, &rom->game_rom[0x0146], sizeof(rom->cartridge_header.sgb_flag));
	memcpy(&rom->cartridge_header.cartridge_type, &rom->game_rom[0x0147], sizeof(rom->cartridge_header.cartridge_type));
	memcpy(&rom->cartridge_header.rom_size, &rom->game_rom[0x0148], sizeof(rom->cartridge_header.rom_size));
	memcpy(&rom->cartridge_header.ram_size, &rom->game_rom[0x0149], sizeof(rom->cartridge_header.ram_size));
	memcpy(&rom->cartridge_header.destination_code, &rom->game_rom[0x014A], sizeof(rom->cartridge_header.destination_code));
	memcpy(&rom->cartridge_header.old_licensee_code, &rom->game_rom[0x014B], sizeof(rom->cartridge_header.old_licensee_code));
	memcpy(&rom->cartridge_header.mask_rom_version_number, &rom->game_rom[0x014C], sizeof(rom->cartridge_header.mask_rom_version_number));
	memcpy(&rom->cartridge_header.header_checksum, &rom->game_rom[0x014D], sizeof(rom->cartridge_header.header_checksum));
	memcpy(&rom->cartridge_header.global_checksum, &rom->game_rom[0x014E], sizeof(rom->cartridge_header.global_checksum));

	switch (rom->cartridge_header.rom_size) {
	case 0x00:
		rom->max_rom_banks = 2;
		break;
	case 0x01:
		rom->max_rom_banks = 4;
		break;
	case 0x02:
		rom->max_rom_banks = 8;
		break;
	case 0x03:
		rom->max_rom_banks = 16;
		break;
	case 0x04:
		rom->max_rom_banks = 32;
		break;
	case 0x05:
		rom->max_rom_banks = 64;
		break;
	case 0x06:
		rom->max_rom_banks = 128;
		break;
	case 0x07:
		rom->max_rom_banks = 256;
		break;
	case 0x08:
		rom->max_rom_banks = 512;
		break;
	}

	// BOOT_ROM is defined in gameboy.c
	rom->boot_rom = BOOT_ROM;
}
