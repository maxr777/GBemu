#ifdef _WIN32
#include <windows.h>
#else
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#endif

#include "../types.h"
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

#include "game_load_test.c"

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);

	u64 start = now_ns();

	puts("Test: correct game load");
	test_game_load();
	puts("Test: game missing when loading");
	test_game_load_missing();
	puts("Test: game too small to load");
	test_game_load_too_small();

	u64 end = now_ns();

#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif

	printf("tests passed in %.3fms\n", (end - start) / 1000000.0);

	return 0;
}
