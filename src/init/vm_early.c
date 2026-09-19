#include "init/vm_early.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "arch/layout.h"
#include "arch/pt.h"
#include "init/boot_alloc.h"
#include "init/early_print.h"
#include "limine/boot.h"
#include "limine/limine.h"

static page_table_t kernel_root_table = nullptr;

/* Maps a contiguous virtual-to-physical range using the largest possible
   superpages (e.g. Petapages at Level 4, Terapages at Level 3, Gigapages at Level 2,
   Megapages at Level 1, 4 KiB at Level 0).
   va, pa, and size must be page-aligned (4096 bytes).
   Works dynamically across Sv39, Sv48, and Sv57 paging modes. */
void vm_early_map_range(page_table_t root_table, void* va, uintptr_t pa,
						size_t size, uint64_t flags) {
	early_assert(root_table != nullptr, "vm_early_map_range: null root table\n");
	early_assert(page_is_aligned(va), "vm_early_map_range: va not page aligned\n");
	early_assert((pa & PAGE_MASK) == 0, "vm_early_map_range: pa not page aligned\n");
	early_assert((size & PAGE_MASK) == 0, "vm_early_map_range: size not page aligned\n");

	if (size == 0) {
		return;
	}

	uint64_t leaf_flags = flags | PTE_V | PTE_A | PTE_G;
	if (flags & PTE_W) {
		leaf_flags |= PTE_D;
	}

	uintptr_t curr_va = (uintptr_t)va;
	uintptr_t curr_pa = pa;
	size_t remaining = size;

	while (remaining > 0) {
		page_table_t table = root_table;

		for (int level = VM_ROOT_LEVEL; level >= 0; level--) {
			size_t page_sz = vm_level_page_size(level);
			size_t vpn = vm_vpn((void*)curr_va, level);

			bool can_map_leaf = (level == 0) ||
								((remaining >= page_sz) &&
								 ((curr_va & (page_sz - 1)) == 0) &&
								 ((curr_pa & (page_sz - 1)) == 0) &&
								 !vm_pte_is_table(table[vpn]));

			if (can_map_leaf) {
				if (table[vpn] & PTE_V) {
					if (vm_pte_is_leaf(table[vpn]) &&
						vm_pte_to_paddr(table[vpn]) == curr_pa) {
						table[vpn] = vm_paddr_to_pte(curr_pa, leaf_flags);
					} else {
						early_panic("vm_early_map_range: conflicting mapping\n");
					}
				} else {
					table[vpn] = vm_paddr_to_pte(curr_pa, leaf_flags);
				}

				curr_va += page_sz;
				curr_pa += page_sz;
				remaining -= page_sz;
				break;
			}

			/* Need to descend to next level table */
			if (!(table[vpn] & PTE_V)) {
				void* new_tbl = boot_zalloc_page();
				if (!new_tbl) {
					early_panic("vm_early_map_range: failed to allocate page table\n");
				}
				uintptr_t new_tbl_pa = limine_hhdm_vtop(new_tbl);
				table[vpn] = vm_paddr_to_pte(new_tbl_pa, PTE_V);
			} else if (vm_pte_is_leaf(table[vpn])) {
				early_panic("vm_early_map_range: cannot descend into existing leaf PTE\n");
			}

			table = (page_table_t)limine_hhdm_ptov(vm_pte_to_paddr(table[vpn]));
		}
	}
}

static void map_kernel_section(page_table_t root, void* start, void* end,
							   uint64_t flags) {
	early_assert(page_is_aligned(start),
				 "Kernel section start not page aligned\n");
	early_assert(page_is_aligned(end), "Kernel section end not page aligned\n");

	uintptr_t va_start = (uintptr_t)start;
	uintptr_t va_end = (uintptr_t)end;

	if (va_end > va_start) {
		size_t size = va_end - va_start;
		uintptr_t pa = (uintptr_t)limine_exe_vtop(start);
		vm_early_map_range(root, start, pa, size, flags);
	}
}

/* Initializes the kernel page tables */
void vm_early_init(void) {
	early_puts("[VM Early] Initializing early page tables...\n");

	struct limine_memmap_response* memmap = limine_get_memmap();
	early_assert(memmap && memmap->entries && memmap->entry_count > 0,
				 "VM Early: Limine memory map unavailable\n");

	kernel_root_table = (page_table_t)boot_zalloc_page();
	early_assert(kernel_root_table != nullptr,
				 "VM Early: Failed to allocate root page table\n");

	/* Map physical memory into Direct Map at VM_DIRECT_MAP_BASE, excluding reserved areas.
	   Entries are sorted by base address. We merge contiguous non-reserved
	   intervals and clip them strictly outside reserved areas. */
	uintptr_t cur_start = 0;
	uintptr_t cur_end = 0;
	uintptr_t last_reserved_end = 0;
	bool has_range = false;

	for (size_t i = 0; i < memmap->entry_count; i++) {
		struct limine_memmap_entry* entry = memmap->entries[i];

		if (entry->type == LIMINE_MEMMAP_RESERVED ||
			entry->type == LIMINE_MEMMAP_BAD_MEMORY) {
			uintptr_t res_start = (uintptr_t)(entry->base & ~PAGE_MASK);
			uintptr_t res_end =
				(uintptr_t)((entry->base + entry->length + PAGE_MASK) &
							~PAGE_MASK);

			if (res_end > last_reserved_end) {
				last_reserved_end = res_end;
			}

			if (has_range) {
				if (cur_end > res_start) {
					cur_end = res_start;
				}
				if (cur_end > cur_start) {
					vm_early_map_range(kernel_root_table,
									   (void*)(VM_DIRECT_MAP_BASE + cur_start),
									   cur_start, cur_end - cur_start, PTE_RW);
				}
				has_range = false;
			}
			continue;
		}

		uintptr_t entry_start = (uintptr_t)(entry->base & ~PAGE_MASK);
		uintptr_t entry_end =
			(uintptr_t)((entry->base + entry->length + PAGE_MASK) & ~PAGE_MASK);

		if (entry_start < last_reserved_end) {
			entry_start = last_reserved_end;
		}

		if (entry_start >= entry_end) {
			continue;
		}

		early_assert(entry_end <= VM_DIRECT_MAP_SIZE,
					 "VM Early: Memory space is too large\n");

		if (!has_range) {
			cur_start = entry_start;
			cur_end = entry_end;
			has_range = true;
		} else {
			if (entry_start <= cur_end) {
				if (entry_end > cur_end) {
					cur_end = entry_end;
				}
			} else {
				vm_early_map_range(kernel_root_table,
								   (void*)(VM_DIRECT_MAP_BASE + cur_start),
								   cur_start, cur_end - cur_start, PTE_RW);
				cur_start = entry_start;
				cur_end = entry_end;
			}
		}
	}

	if (has_range && cur_end > cur_start) {
		vm_early_map_range(kernel_root_table,
						   (void*)(VM_DIRECT_MAP_BASE + cur_start),
						   cur_start, cur_end - cur_start, PTE_RW);
	}

	/* Map kernel ELF sections with appropriate permissions */
	map_kernel_section(kernel_root_table, __text_start, __text_end, PTE_RX);
	map_kernel_section(kernel_root_table, __rodata_start, __rodata_end, PTE_R);
	map_kernel_section(kernel_root_table, __data_start, __data_end, PTE_RW);
	map_kernel_section(kernel_root_table, __bss_start, __bss_end, PTE_RW);

	early_puts("[VM Early] Kernel page tables initialized.\n");
}
