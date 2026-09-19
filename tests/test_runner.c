#ifdef _WIN32
#include <windows.h>
#else
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#endif

#include "../misc/types.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

static u64 now_ns(void) {
#ifdef _WIN32
	static LARGE_INTEGER frequency;
	static int initialized = 0;

	LARGE_INTEGER counter;

	if (!initialized) {
		QueryPerformanceFrequency(&frequency);
		initialized = 1;
	}

	QueryPerformanceCounter(&counter);

	return (u64)((counter.QuadPart * 1000000000ULL) / frequency.QuadPart);
#else
	struct timespec ts = {0};
	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (u64)ts.tv_sec * 1000000000ULL + (u64)ts.tv_nsec;
#endif
}

// clang-format off
#include "../platform/desktop.c"
#include "../gameboy/memory.c"
#include "game_load_test.c"
#include "memory_map_test.c"
// clang-format on

int main(void) {
	bool color = isatty(fileno(stderr));

	if (color) fprintf(stderr, "\033[1;33mNOTE:\033[0m ");
	else fprintf(stderr, "NOTE: ");

	fprintf(stderr, "The prints below are out of sync. The test suite either hits an assert (and fails) "
			"or passes and prints the total time it took to run the tests\n");

	u64 start = now_ns();

	test_game_load();
	test_game_load_missing();
	test_game_load_too_small();
	test_memory_read();
	test_memory_write();

	u64 end = now_ns();

	if (color) fprintf(stderr, "\033[1;36m");
	fprintf(stderr, "All (non-cpu instr) tests passed in %.3fms\n", (end - start) / 1000000.0);
	if (color) fprintf(stderr, "\033[0m");

	return 0;
}
