#include "../external/raylib/raylib.h"
#include "../gameboy/constants.h"
#include "../gameboy/gameboy.h"
#include "../misc/types.h"
#include "desktop.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off: unity build include order is intentional
#include "desktop.c"
#include "../gameboy/cpu.c"
#include "../gameboy/memory.c"
#include "../gameboy/gameboy.c"
// clang-format on

bool debug = true;

int
main(int argc, char *argv[]) {
	// if (argc < 2) {
	// 	fprintf(stderr, "No game loaded\n");
	// 	return 1;
	// }

	for (int i = 2; i < argc; ++i) {
		if (strcmp(argv[i], "-d") == 0)
			debug = true;
	}

	// Gameboy gb = gameboy_initialize(argv[1]);

	SetTraceLogLevel(LOG_ERROR);

	InitWindow(960, 540, "GBemu");
	ToggleBorderlessWindowed();

	// This has to be called after InitWindow()
	// because it requires an active OpenGL context
	Texture2D test_background = LoadTexture("tests/test.png");
	// Gameboy gb = {};

	bool running = true;
	while (!WindowShouldClose() && running) {
		if (IsKeyPressed(KEY_Q))
			running = false;
		if (IsKeyPressed(KEY_F11))
			ToggleBorderlessWindowed();

		const double frame_start = GetTime();

		// ==============================================

		BeginDrawing();
		ClearBackground(BLACK);

		// main_loop(&gameboy);

		DrawTexture(test_background, 0, 0, WHITE);
		DrawFPS(0.0f, 0.0f);

		EndDrawing();

		// ==============================================

		const double elapsed = GetTime() - frame_start;
		if (elapsed < TARGET_FRAMETIME)
			WaitTime(TARGET_FRAMETIME - elapsed);
	}
}
