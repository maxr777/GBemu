#include "../external/cJSON/cJSON.h"
#include "../gameboy/gameboy.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char name[10];
	u8 a;
	u8 b;
	u8 c;
	u8 d;
	u8 e;
	u8 f;
	u8 h;
	u8 l;
	u16 pc;
	u16 sp;
	// Size as in entries, not byte size
	int mem_size;
	u16 *mem_addr;
	u8 *mem_val;
} TestState;

void test_state_print(TestState *test) {
	printf("a: 0x%02x\nb: 0x%02x\nc: 0x%02x\nd: 0x%02x\ne: 0x%02x\nf: 0x%02x\nh: 0x%02x\nl: 0x%02x\npc: 0x%02x\nsp: 0x%02x\n",
	       test->a, test->b, test->c, test->d, test->e, test->f, test->h, test->l, test->pc, test->sp);

	for (int i = 0; i < test->mem_size; ++i) {
		printf("address: 0x%02x\nvalue: 0x%02x\n", test->mem_addr[i], test->mem_val[i]);
	}
}

void test_cpu() {
	Gameboy gb = {};

	for (int i = 0x00; i <= 0x01 /*0xFF*/; ++i) {
		printf("Test: Opcode %02X\n", i);
		fflush(stdout);

		char buf[20];
		snprintf(buf, sizeof(buf), "tests/CPU/%02x.json", i);

		FILE *f = fopen(buf, "rb");
		if (f == 0) {
			fprintf(stderr, "%s - ", buf);
			perror("fopen() failed");
			continue;
		}
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		char *test = malloc(fsize + 1);
		if (test == 0) {
			perror("malloc() failed:");
			abort();
		}
		fread(test, 1, fsize, f);
		fclose(f);

		test[fsize] = '\0';

		cJSON *json = cJSON_Parse(test);
		free(test);
		if (json == 0) {
			const char *error_ptr = cJSON_GetErrorPtr();
			if (error_ptr != NULL) {
				fprintf(stderr, "Error: JSON parse failed before: %s\n", error_ptr);
			}
			abort();
		}

		int test_num = cJSON_GetArraySize(json);

		for (int j = 0; j < test_num; ++j) {
			const cJSON *item = cJSON_GetArrayItem(json, j);
			if (item == 0) {
				fprintf(stderr, "cJSON_GetArrayItem() failed\n");
				abort();
			}

			cJSON *name = cJSON_GetObjectItem(item, "name");

			TestState test_initial = {};

			{
				// printf("initial:\n");
				cJSON *initial = cJSON_GetObjectItem(item, "initial");

				cJSON *a = cJSON_GetObjectItem(initial, "a");
				test_initial.a = (u8)cJSON_GetNumberValue(a);

				cJSON *b = cJSON_GetObjectItem(initial, "b");
				test_initial.b = (u8)cJSON_GetNumberValue(b);

				cJSON *c = cJSON_GetObjectItem(initial, "c");
				test_initial.c = (u8)cJSON_GetNumberValue(c);

				cJSON *d = cJSON_GetObjectItem(initial, "d");
				test_initial.d = (u8)cJSON_GetNumberValue(d);

				cJSON *e = cJSON_GetObjectItem(initial, "e");
				test_initial.e = (u8)cJSON_GetNumberValue(e);

				cJSON *f = cJSON_GetObjectItem(initial, "f");
				test_initial.f = (u8)cJSON_GetNumberValue(f);

				cJSON *h = cJSON_GetObjectItem(initial, "h");
				test_initial.h = (u8)cJSON_GetNumberValue(h);

				cJSON *l = cJSON_GetObjectItem(initial, "l");
				test_initial.l = (u8)cJSON_GetNumberValue(l);

				cJSON *pc = cJSON_GetObjectItem(initial, "pc");
				test_initial.pc = (u16)cJSON_GetNumberValue(pc);

				cJSON *sp = cJSON_GetObjectItem(initial, "sp");
				test_initial.sp = (u16)cJSON_GetNumberValue(sp);

				cJSON *ram = cJSON_GetObjectItem(initial, "ram");
				test_initial.mem_size = cJSON_GetArraySize(ram);
				test_initial.mem_addr = malloc(sizeof(u16) * test_initial.mem_size);
				test_initial.mem_val = malloc(sizeof(u8) * test_initial.mem_size);

				for (int k = 0; k < test_initial.mem_size; ++k) {
					cJSON *ram_entry = cJSON_GetArrayItem(ram, k);

					cJSON *ram_address = cJSON_GetArrayItem(ram_entry, 0);
					test_initial.mem_addr[k] = (u16)cJSON_GetNumberValue(ram_address);

					cJSON *ram_value = cJSON_GetArrayItem(ram_entry, 1);
					test_initial.mem_val[k] = (u16)cJSON_GetNumberValue(ram_value);
				}

				// test_state_print(&test_initial);
			}

			gb.cpu.regs[AF].high = test_initial.a;
			gb.cpu.regs[AF].low = test_initial.f;
			gb.cpu.regs[BC].high = test_initial.b;
			gb.cpu.regs[BC].low = test_initial.c;
			gb.cpu.regs[DE].high = test_initial.d;
			gb.cpu.regs[DE].low = test_initial.e;
			gb.cpu.regs[HL].high = test_initial.h;
			gb.cpu.regs[HL].low = test_initial.l;
			// These CPU tests provide the opcode separately and set PC as if the
			// opcode fetch already advanced it. opcode_execute() expects PC to
			// still point at the opcode, because each instruction advances PC by
			// its full length - that's why I use PC - 1 here
			gb.cpu.regs[PC].full = test_initial.pc - 1;
			gb.cpu.regs[SP].full = test_initial.sp;

			for (int k = 0; k < test_initial.mem_size; ++k) {
				// test_mem[test_initial.mem_addr[k]] =
				//     test_initial.mem_val[k];
				write8(&gb.memory, test_initial.mem_addr[k], test_initial.mem_val[k]);
			}

			opcode_execute(i, &gb, false);

			TestState test_expected = {};

			{
				// printf("final:\n");
				cJSON *final = cJSON_GetObjectItem(item, "final");

				cJSON *a = cJSON_GetObjectItem(final, "a");
				test_expected.a = (u8)cJSON_GetNumberValue(a);

				cJSON *b = cJSON_GetObjectItem(final, "b");
				test_expected.b = (u8)cJSON_GetNumberValue(b);

				cJSON *c = cJSON_GetObjectItem(final, "c");
				test_expected.c = (u8)cJSON_GetNumberValue(c);

				cJSON *d = cJSON_GetObjectItem(final, "d");
				test_expected.d = (u8)cJSON_GetNumberValue(d);

				cJSON *e = cJSON_GetObjectItem(final, "e");
				test_expected.e = (u8)cJSON_GetNumberValue(e);

				cJSON *f = cJSON_GetObjectItem(final, "f");
				test_expected.f = (u8)cJSON_GetNumberValue(f);

				cJSON *h = cJSON_GetObjectItem(final, "h");
				test_expected.h = (u8)cJSON_GetNumberValue(h);

				cJSON *l = cJSON_GetObjectItem(final, "l");
				test_expected.l = (u8)cJSON_GetNumberValue(l);

				cJSON *pc = cJSON_GetObjectItem(final, "pc");
				test_expected.pc = (u16)cJSON_GetNumberValue(pc);

				cJSON *sp = cJSON_GetObjectItem(final, "sp");
				test_expected.sp = (u16)cJSON_GetNumberValue(sp);

				cJSON *ram = cJSON_GetObjectItem(final, "ram");
				test_expected.mem_size = cJSON_GetArraySize(ram);

				test_expected.mem_addr = malloc(sizeof(u16) * test_expected.mem_size);
				test_expected.mem_val = malloc(sizeof(u8) * test_expected.mem_size);

				for (int k = 0; k < test_expected.mem_size; ++k) {
					cJSON *ram_entry = cJSON_GetArrayItem(ram, k);

					cJSON *ram_address = cJSON_GetArrayItem(ram_entry, 0);
					test_expected.mem_addr[k] = (u16)cJSON_GetNumberValue(ram_address);

					cJSON *ram_value = cJSON_GetArrayItem(ram_entry, 1);
					test_expected.mem_val[k] = (u16)cJSON_GetNumberValue(ram_value);
				}

				// printf("\n***\n\n");
				// test_state_print(&test_expected);
				// printf("\n=============================================\n\n");
			}

			bool pass = true;

			if (gb.cpu.regs[AF].high != test_expected.a) pass = false;
			if (gb.cpu.regs[AF].low != test_expected.f) pass = false;
			if (gb.cpu.regs[BC].high != test_expected.b) pass = false;
			if (gb.cpu.regs[BC].low != test_expected.c) pass = false;
			if (gb.cpu.regs[DE].high != test_expected.d) pass = false;
			if (gb.cpu.regs[DE].low != test_expected.e) pass = false;
			if (gb.cpu.regs[HL].high != test_expected.h) pass = false;
			if (gb.cpu.regs[HL].low != test_expected.l) pass = false;
			// These CPU tests provide the opcode separately and set PC as if the
			// opcode fetch already advanced it. opcode_execute() expects PC to
			// still point at the opcode, because each instruction advances PC by
			// its full length - that's why I use PC - 1 here
			if (gb.cpu.regs[PC].full != test_expected.pc - 1) pass = false;
			if (gb.cpu.regs[SP].full != test_expected.sp) pass = false;

			for (int k = 0; k < test_expected.mem_size; ++k) {
				// if (test_mem[test_expected.mem_addr[k]] !=
				//     test_expected.mem_val[k]) pass = false;
				if (read8(&gb.memory, test_expected.mem_addr[k]) !=
				    test_expected.mem_val[k]) pass = false;
			}

			if (!pass) {
				char *name_str = cJSON_GetStringValue(name);
				printf("FAILED TEST: %s\n", name_str);

				// This is just so we can use the test_state_print function
				TestState temp = {0};
				temp.a = gb.cpu.regs[AF].high;
				temp.f = gb.cpu.regs[AF].low;
				temp.b = gb.cpu.regs[BC].high;
				temp.c = gb.cpu.regs[BC].low;
				temp.d = gb.cpu.regs[DE].high;
				temp.e = gb.cpu.regs[DE].low;
				temp.h = gb.cpu.regs[HL].high;
				temp.l = gb.cpu.regs[HL].low;
				temp.pc = gb.cpu.regs[PC].full;
				temp.sp = gb.cpu.regs[SP].full;

				temp.mem_size = test_expected.mem_size;
				temp.mem_addr = malloc(sizeof(u16) * temp.mem_size);
				temp.mem_val = malloc(sizeof(u8) * temp.mem_size);
				for (int k = 0; k < temp.mem_size; ++k) {
					temp.mem_addr[k] = test_expected.mem_addr[k];
					// temp.mem_val[k] = test_mem[temp.mem_addr[k]];
					temp.mem_val[k] = read8(&gb.memory, temp.mem_addr[k]);
				}

				printf("CURRENT VALUES:\n");
				test_state_print(&temp);

				printf("\n***\n\n");

				printf("EXPECTED:\n");
				test_state_print(&test_expected);

				abort();
			}

			free(test_initial.mem_addr);
			free(test_initial.mem_val);
			free(test_expected.mem_addr);
			free(test_expected.mem_val);
		}
		cJSON_Delete(json);
	}
}
