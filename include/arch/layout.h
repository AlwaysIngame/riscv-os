#pragma once

#include <stddef.h>
#include <stdint.h>

#include "arch/pt.h"

/* ========================================================================= */
/* Virtual Memory Layout Definitions                                         */
/* ========================================================================= */

extern char __text_start[];
extern char __text_end[];
extern char __rodata_start[];
extern char __rodata_end[];
extern char __data_start[];
extern char __data_end[];
extern char __bss_start[];
extern char __bss_end[];

/* Direct Physical Map (higher half direct map) */
constexpr uintptr_t VM_DIRECT_MAP_BASE =
	(VM_KERNEL_MODE == VM_MODE_SV39)   ? 0xffffffc000000000ULL
	: (VM_KERNEL_MODE == VM_MODE_SV48) ? 0xffff800000000000ULL
	: (VM_KERNEL_MODE == VM_MODE_SV57) ? 0xff00000000000000ULL
									   : 0ULL;

constexpr size_t VM_DIRECT_MAP_SIZE =
	128ULL * 1024 * 1024 * 1024ULL; /* 128 GiB */
constexpr uintptr_t VM_DIRECT_MAP_END = VM_DIRECT_MAP_BASE + VM_DIRECT_MAP_SIZE;

/* Kernel ELF mapping base (as defined in link.ld) */
constexpr uintptr_t VM_KERNEL_BASE = 0xffffffff80000000ULL;
