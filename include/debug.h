/* Functions to print, assert, or panic the kernel. */

#pragma once

#include <stdint.h>

/* Prints a single character via SBI DBCN or legacy console putchar. */
void debug_putchar(char c);

/* Prints a null-terminated string. Safe for strings anywhere in memory. */
void debug_puts(const char* s);

/* Prints a 64-bit integer formatted as hexadecimal (0x0123456789abcdef). */
void debug_print_hex(uint64_t val);

/* Prints a 64-bit unsigned integer formatted as decimal. */
void debug_print_dec(uint64_t val);

/* Prints a string for early boot messages, while in the initial bootloader map.
   Only works if SBI supports DBCN extension. sbi_init() must be called.
   s must be a string within the kernel binary.
   len should be value of strlen(s). */
void debug_print_kstr(const char* s, unsigned long len);

/* Prints a kernel binary string and attempts to shutdown the system.
   If the SBI does not support SRST, this function will spin. */
[[noreturn]] void early_panic(const char* s);
