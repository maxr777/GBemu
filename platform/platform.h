#pragma once

#include "../gameboy/gameboy.h"

static void platform_game_load(const char *filepath, ROM *rom);
static void platform_screen_draw(u16 *framebuffer);
static void platform_play_sound();
static void platform_serial_print(const char ch);
static void platform_debug_log(const Gameboy *gb);
static void platform_error_log(const char *msg);
