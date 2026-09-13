#include "mm/pmm.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "riscv.h"
#include "debug.h"
#include "limine/boot.h"
#include "limine/limine.h"
#include "string.h"

struct free_page {
	struct free_page* next;
};

static struct free_page* pmm_free_list = nullptr;
static size_t pmm_total_pages = 0;
static size_t pmm_free_pages = 0;
static size_t pmm_used_pages = 0;
static bool pmm_bootloader_reclaimed = false;

static const char* memmap_type_to_string(uint64_t type) {
	switch (type) {
		case LIMINE_MEMMAP_USABLE:
			return "USABLE";
		case LIMINE_MEMMAP_RESERVED:
			return "RESERVED";
		case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
			return "ACPI_RECLAIMABLE";
		case LIMINE_MEMMAP_ACPI_NVS:
			return "ACPI_NVS";
		case LIMINE_MEMMAP_BAD_MEMORY:
			return "BAD_MEMORY";
		case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
			return "BOOTLOADER_RECLAIMABLE";
		case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
			return "EXECUTABLE_AND_MODULES";
		case LIMINE_MEMMAP_FRAMEBUFFER:
			return "FRAMEBUFFER";
		case LIMINE_MEMMAP_RESERVED_MAPPED:
			return "RESERVED_MAPPED";
		default:
			return "UNKNOWN";
	}
}

void pmm_init(void) {
	struct limine_memmap_response* memmap = limine_get_memmap();
	if (!memmap || !memmap->entries || memmap->entry_count == 0) {
		early_panic("PMM: Failed to retrieve Limine memory map\n");
	}

	debug_puts("[PMM] Initializing physical memory manager...\n");
	debug_puts("[PMM] Memory Map Entries:\n");

	for (size_t i = 0; i < memmap->entry_count; i++) {
		struct limine_memmap_entry* entry = memmap->entries[i];
		debug_puts("  [");
		debug_print_dec(i);
		debug_puts("] base: ");
		debug_print_hex(entry->base);
		debug_puts(" - length: ");
		debug_print_hex(entry->length);
		debug_puts(" (");
		debug_puts(memmap_type_to_string(entry->type));
		debug_puts(")\n");

		/* Only add LIMINE_MEMMAP_USABLE entries to the initial free page pool.
		   These regions are guaranteed page-aligned, non-overlapping,
		   and mapped into HHDM. */
		if (entry->type == LIMINE_MEMMAP_USABLE) {
			for (uint64_t offset = 0; offset < entry->length;
				 offset += PAGE_SIZE) {
				uintptr_t paddr = (uintptr_t)(entry->base + offset);
				pmm_free_page(paddr);
				pmm_total_pages++;
			}
		}
	}

	debug_puts("[PMM] Initialization complete.\n");
	debug_puts("[PMM] Total Usable Pages: ");
	debug_print_dec(pmm_total_pages);
	debug_puts(" (");
	debug_print_dec((pmm_total_pages * PAGE_SIZE) / (1024 * 1024));
	debug_puts(" MiB)\n");
	debug_puts("[PMM] Free Pages: ");
	debug_print_dec(pmm_free_pages);
	debug_puts("\n");
}

uintptr_t pmm_alloc_page(void) {
	if (!pmm_free_list) {
		return 0;
	}

	struct free_page* page = pmm_free_list;
	pmm_free_list = page->next;
	pmm_free_pages--;
	pmm_used_pages++;

	return limine_hhdm_vtop(page);
}

uintptr_t pmm_zalloc_page(void) {
	uintptr_t paddr = pmm_alloc_page();
	if (paddr != 0) {
		void* vaddr = limine_hhdm_ptov(paddr);
		memset(vaddr, 0, (size_t)PAGE_SIZE);
	}
	return paddr;
}

void pmm_free_page(uintptr_t paddr) {
	if (!paddr || !page_is_aligned(paddr)) {
		return;
	}

	/* Store next pointer directly within free page at its HHDM virtual address.
	   Safe because usable and bootloader-reclaimable memory is mapped in HHDM.
	 */
	struct free_page* page = (struct free_page*)limine_hhdm_ptov(paddr);
	page->next = pmm_free_list;
	pmm_free_list = page;

	pmm_free_pages++;
	if (pmm_used_pages > 0) {
		pmm_used_pages--;
	}
}

void* pmm_alloc_page_virt(void) {
	uintptr_t paddr = pmm_alloc_page();
	return paddr ? limine_hhdm_ptov(paddr) : nullptr;
}

void* pmm_zalloc_page_virt(void) {
	uintptr_t paddr = pmm_zalloc_page();
	return paddr ? limine_hhdm_ptov(paddr) : nullptr;
}

void pmm_free_page_virt(void* vaddr) {
	if (!vaddr) {
		return;
	}
	pmm_free_page(limine_hhdm_vtop(vaddr));
}

void pmm_reclaim_bootloader(void) {
	if (pmm_bootloader_reclaimed) {
		return;
	}

	struct limine_memmap_response* memmap = limine_get_memmap();
	if (!memmap) {
		return;
	}

	size_t reclaimed_count = 0;
	for (size_t i = 0; i < memmap->entry_count; i++) {
		struct limine_memmap_entry* entry = memmap->entries[i];
		if (entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
			for (uint64_t offset = 0; offset < entry->length;
				 offset += PAGE_SIZE) {
				uintptr_t paddr = (uintptr_t)(entry->base + offset);
				pmm_free_page(paddr);
				pmm_total_pages++;
				reclaimed_count++;
			}
		}
	}

	pmm_bootloader_reclaimed = true;
	debug_puts("[PMM] Reclaimed ");
	debug_print_dec(reclaimed_count);
	debug_puts(" bootloader pages (");
	debug_print_dec((reclaimed_count * PAGE_SIZE) / 1024);
	debug_puts(" KiB)\n");
}

size_t pmm_get_total_pages(void) { return pmm_total_pages; }

size_t pmm_get_free_pages(void) { return pmm_free_pages; }

size_t pmm_get_used_pages(void) { return pmm_used_pages; }

size_t pmm_get_total_memory(void) { return pmm_total_pages * PAGE_SIZE; }

size_t pmm_get_free_memory(void) { return pmm_free_pages * PAGE_SIZE; }
