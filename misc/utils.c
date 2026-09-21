#include "utils.h"

static void *gb_memset(void *ptr, u8 val, size_t count) {
	u8 *p = ptr;
	while (count--) *p++ = val;
	return ptr;
}
