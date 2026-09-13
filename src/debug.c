#include "debug.h"

#include <stddef.h>
#include <stdint.h>

#include "limine/boot.h"
#include "sbi.h"
#include "string.h"

void debug_putchar(char c) {
	if (sbi_has_dbcn()) {
		sbi_debug_console_write_byte((uint8_t)c);
	} else {
		sbi_console_putchar((int)c);
	}
}

void debug_puts(const char* s) {
	if (!s) return;
	while (*s) {
		debug_putchar(*s++);
	}
}

void debug_print_hex(uint64_t val) {
	constexpr char hex_digits[] = "0123456789abcdef";
	debug_puts("0x");
	for (int i = 60; i >= 0; i -= 4) {
		debug_putchar(hex_digits[(val >> i) & 0xf]);
	}
}

void debug_print_dec(uint64_t val) {
	if (val == 0) {
		debug_putchar('0');
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
		debug_putchar(buf[--i]);
	}
}

void debug_print_kstr(const char* s, unsigned long len) {
	if (!s || len == 0) return;

	if (sbi_has_dbcn()) {
		void* phys = limine_exe_vtop(s);
		unsigned long written = 0;
		while (written < len) {
			struct sbiret ret = sbi_debug_console_write(
				len - written, (unsigned long)phys + written, 0);
			if (ret.error == SBI_SUCCESS && ret.uvalue > 0) {
				written += ret.uvalue;
			} else if (ret.error == SBI_SUCCESS && ret.uvalue == 0) {
				sbi_debug_console_write_byte((uint8_t)s[written]);
				written++;
			} else {
				/* Write failed, fall back to write_byte for remaining */
				while (written < len) {
					sbi_debug_console_write_byte((uint8_t)s[written]);
					written++;
				}
				break;
			}
		}
		return;
	}

	/* Fall back to legacy console putchar if DBCN is not supported */
	for (unsigned long i = 0; i < len; i++) {
		sbi_console_putchar((int)s[i]);
	}
}

[[noreturn]] void early_panic(const char* s) {
	if (s) {
		debug_print_kstr(s, strlen(s));
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
