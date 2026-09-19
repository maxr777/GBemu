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

#define CPU_TEST

#include "../gameboy/gameboy_inc.h"

// clang-format off
#include "../platform/desktop.c"
#include "../misc/utils.c"
#include "../gameboy/gameboy_inc.c"
#include "../external/cJSON/cJSON.c"
#include "cpu_tests.c"
// clang-format on

int main(void) {
	bool color = isatty(fileno(stderr));

	u64 start = now_ns();
	test_cpu();
	u64 end = now_ns();

	if (color) fprintf(stderr, "\033[1;36m");
	fprintf(stderr, "All CPU tests passed in %.3fms\n", (end - start) / 1000000.0);
	if (color) fprintf(stderr, "\033[0m");

	return 0;
}
