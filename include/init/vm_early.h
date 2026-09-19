#pragma once

#include <stddef.h>
#include <stdint.h>

#include "arch/pt.h"

struct limine_mp_info;

/* Maps a contiguous virtual-to-physical range using the largest possible
   superpages (e.g. Gigapages at Level 2, Megapages at Level 1, 4 KiB at Level
   0). va, pa, and size must be page-aligned (4096 bytes). */
void vm_early_map_range(page_table_t root_table, void* va, uintptr_t pa,
						size_t size, uint64_t flags);

/* Initializes the kernel page tables */
void vm_early_init(void);
