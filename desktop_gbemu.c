#include "constants.h"
#include "raylib.h"
#include <stdbool.h>
#include <string.h>

bool debug = false;

int main(int argc, char *argv[]) {
	// if (argc < 2) {
	// 	fprintf(stderr, "No game loaded\n");
	// 	return 1;
	// }

	for (int i = 2; i < argc; ++i) {
		if (strcmp(argv[i], "-d") == 0)
			debug = true;
	}

	SetTraceLogLevel(LOG_ERROR);

	InitWindow(960, 540, "GBemu");
	ToggleBorderlessWindowed();

	// This has to be called after PlatformInitWindow()
	// because it requires an active OpenGL context
	Texture2D test_background = LoadTexture("test.png");

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

		DrawTexture(test_background, 0, 0, WHITE);
		DrawFPS(0.0f, 0.0f);

		EndDrawing();

		// ==============================================

		const double elapsed = GetTime() - frame_start;
		if (elapsed < TARGET_FRAMETIME)
			WaitTime(TARGET_FRAMETIME - elapsed);
	}
}
