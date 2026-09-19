#include "init/boot_alloc.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "arch/pt.h"
#include "init/early_print.h"
#include "limine/boot.h"
#include "limine/limine.h"
#include "string.h"

struct free_page {
	struct free_page* next_paddr;
};

static struct free_page* head = nullptr;

void boot_alloc_init(void) {
	struct limine_memmap_response* memmap = limine_get_memmap();
	if (!memmap || !memmap->entries || memmap->entry_count == 0) {
		early_panic("Boot Alloc: Failed to retrieve Limine memory map\n");
	}

	early_puts("[Boot Alloc] Initializing early boot memory allocator...\n");
	early_puts("[Boot Alloc] Memory Map Entries:\n");

	for (size_t i = 0; i < memmap->entry_count; i++) {
		struct limine_memmap_entry* entry = memmap->entries[i];

		/* Only add LIMINE_MEMMAP_USABLE entries to the initial free page pool.
		   These regions are guaranteed page-aligned, non-overlapping,
		   and mapped into HHDM. */
		if (entry->type == LIMINE_MEMMAP_USABLE) {
			for (uint64_t offset = 0; offset < entry->length;
				 offset += PAGE_SIZE) {
				uintptr_t paddr = entry->base + offset;
				boot_free_page(limine_hhdm_ptov(paddr));
			}
		}
	}

	early_puts("[Boot Alloc] Initialization complete.\n");
}

void* boot_alloc_page(void) {
	if (!head) {
		return 0;
	}

	struct free_page* page = head;
	head = page->next_paddr;

	return page;
}

void* boot_zalloc_page(void) {
	void* vaddr = (void*) boot_alloc_page();
	if (vaddr) {
		memset(vaddr, 0, (size_t)PAGE_SIZE);
	}
	return vaddr;
}

void boot_free_page(void* vaddr) {
	if (!vaddr || !page_is_aligned(vaddr)) {
		early_panic("Boot Alloc: Bad free\n");
	}

	/* Store next physical pointer directly within free page at its virtual
	 * address. */
	struct free_page* page = (void*)vaddr;
	page->next_paddr = head;
	head = page;
}
