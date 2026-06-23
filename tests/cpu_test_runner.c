/*
 * Since the CPU tests require a simplified memory layout, I have to use
 * a separate test runner for them, since I just went with a simple #ifdef
 * for the memory read and write functions.
 */

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

#define CPU_TEST

// clang-format off
#include "../platform/desktop.c"
#include "../gameboy/memory.c"
#include "../gameboy/cpu.c"
#include "../gameboy/gameboy.c"
#include "../external/cJSON/cJSON.c"
#include "cpu_tests.c"
// clang-format on

int main(void) {

	u64 start = now_ns();
	test_cpu();
	u64 end = now_ns();

	fprintf(stderr, "All CPU tests passed in %.3fms\n", (end - start) / 1000000.0);

	return 0;
}
