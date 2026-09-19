#include "init/early_print.h"

#include <stddef.h>
#include <stdint.h>

#include "sbi.h"
#include "string.h"

void early_putchar(char c) {
	if (sbi_has_dbcn()) {
		sbi_debug_console_write_byte((uint8_t)c);
	} else {
		sbi_console_putchar((int)c);
	}
}

void early_puts(const char* s) {
	if (!s) return;
	while (*s) {
		early_putchar(*s++);
	}
}

void early_print_hex(uint64_t val) {
	constexpr char hex_digits[] = "0123456789abcdef";
	early_puts("0x");
	for (int i = 60; i >= 0; i -= 4) {
		early_putchar(hex_digits[(val >> i) & 0xf]);
	}
}

void early_print_dec(uint64_t val) {
	if (val == 0) {
		early_putchar('0');
		return;
	}
	constexpr size_t DEC_BUF_SIZE = 21;
	char buf[DEC_BUF_SIZE];
	int i = 0;
	while (val > 0) {
		buf[i++] = (char)('0' + (val % 10));
		val /= 10;
	}
	while (i > 0) {
		early_putchar(buf[--i]);
	}
}

[[noreturn]] void early_panic(const char* s) {
	if (s) {
		early_puts(s);
	}

	/* Attempt system shutdown using SRST extension if available */
	if (sbi_has_srst()) {
		sbi_system_reset(SBI_SRST_RESET_TYPE_SHUTDOWN,
						 SBI_SRST_RESET_REASON_SYS_FAILURE);
	}

	/* Fallback to legacy shutdown if SRST not supported or failed */
	sbi_shutdown();

	/* Spin if system shutdown is not supported */
	while (1) {
		__asm__ volatile("wfi");
	}
}
