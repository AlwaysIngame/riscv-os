/* Functions to print, assert, or panic the kernel. */

#pragma once

#include <stdnoreturn.h>

/* Prints a string for early boot messages, while in the initial bootloader map.
   Only works if SBI supports DBCN extension. sbi_init() must be called.
   s must be a string within the kernel binary.
   len should be value of strlen(s). */
void debug_print_kstr(const char *s, unsigned long len);

/* Prints a kernel binary string and attempts to shutdown the system.
   If the SBI does not support SRST, this function will spin. */
noreturn void early_panic(const char *s);