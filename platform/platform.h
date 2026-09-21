#pragma once

#include "../gameboy/gameboy.h"

void platform_game_load(const char *filepath, ROM *rom);
void platform_screen_draw(u16 *framebuffer);
void platform_play_sound();
void platform_serial_print(const char ch);
void platform_debug_log(const Gameboy *gb);
void platform_error_log(const char *msg);
