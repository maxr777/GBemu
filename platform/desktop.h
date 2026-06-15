#ifndef DESKTOP_H
#define DESKTOP_H

#include "../gameboy/gameboy.h"

void platform_game_load(const char *filepath, ROM *rom);
void platform_screen_draw(u16 *framebuffer);
void platform_play_sound();
void platform_instruction_log(Gameboy *gb, const u8 opcode, const char *instr);
void platform_error_log(const char *msg);

#endif
