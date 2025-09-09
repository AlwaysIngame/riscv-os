#include "debug.h"

#include <stdint.h>

#include "limine/features.h"
#include "sbi.h"
#include "string.h"

void debug_print_kstr(const char *s, unsigned long len) {
	if (!sbi_capabilities.dbcn) return;

	unsigned long addr = (uintptr_t)LIMINE_EXE_VTOP(s);
	sbi_debug_console_write(len, addr & ((1ul << 32) - 1), addr >> 32);
}

noreturn void early_panic(const char *s) {
	debug_print_kstr(s, strlen(s));

	if (sbi_capabilities.srst) {
		sbi_system_reset(SBI_SRST_TYPE_SHUTDOWN, SBI_SRST_REASON_NO_REASON);
	}

	while (1);
}