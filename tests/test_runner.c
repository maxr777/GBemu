#ifdef _WIN32
#include <windows.h>
#else
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#endif

#include "../misc/types.h"
#include <stdint.h>
#include <stdio.h>

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

	u64 start = now_ns();

	test_game_load();
	test_game_load_missing();
	test_game_load_too_small();
	test_memory_read();

	u64 end = now_ns();

	fprintf(stderr, "All game load tests passed in %.3fms\n", (end - start) / 1000000.0);

	return 0;
}
