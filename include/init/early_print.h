/* Functions to print, assert, or panic the kernel. */

#pragma once

#include <stdint.h>

/* Prints a single character via SBI DBCN or legacy console putchar. */
void early_putchar(char c);

/* Prints a null-terminated string. Safe for strings anywhere in memory. */
void early_puts(const char* s);

/* Prints a 64-bit integer formatted as hexadecimal (0x0123456789abcdef). */
void early_print_hex(uint64_t val);

/* Prints a 64-bit unsigned integer formatted as decimal. */
void early_print_dec(uint64_t val);

/* Prints a kernel binary string and attempts to shutdown the system.
   If the SBI does not support SRST, this function will spin. */
[[noreturn]] void early_panic(const char* s);

/* Early assertion macro. Panics with msg if cond is false. */
#define early_assert(cond, msg) \
	do {                        \
		if (!(cond)) {          \
			early_panic(msg);   \
		}                       \
	} while (0)
