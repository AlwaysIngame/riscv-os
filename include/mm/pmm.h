#pragma once

#include <stddef.h>
#include <stdint.h>

/* Initialize the Physical Memory Manager (PMM) by parsing the Limine memory
   map. Adds all LIMINE_MEMMAP_USABLE pages to the intrusive free list. */
void pmm_init(void);

/* Allocates a single physical page.
   Returns the physical address of the page, or 0 if out of memory. */
uintptr_t pmm_alloc_page(void);

/* Allocates a single physical page and zeroes its content.
   Returns the physical address of the page, or 0 if out of memory. */
uintptr_t pmm_zalloc_page(void);

/* Frees a single physical page given its physical address.
   paddr must be page-aligned and non-zero. */
void pmm_free_page(uintptr_t paddr);

/* Convenience helper: allocates a single page and returns its
   virtual address in the Higher Half Direct Map (HHDM). Returns nullptr if OOM. */
void* pmm_alloc_page_virt(void);

/* Convenience helper: allocates a single zeroed page and returns its
   virtual address in the Higher Half Direct Map (HHDM). Returns nullptr if OOM. */
void* pmm_zalloc_page_virt(void);

/* Convenience helper: frees a single page given its HHDM virtual address. */
void pmm_free_page_virt(void* vaddr);

/* Reclaims memory previously reserved for bootloader info
   (LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) and adds those pages to the free page
   pool. Should only be called when bootloader structures are no longer needed. */
void pmm_reclaim_bootloader(void);

/* Memory statistics (in pages) */
size_t pmm_get_total_pages(void);
size_t pmm_get_free_pages(void);
size_t pmm_get_used_pages(void);

/* Memory statistics (in bytes) */
size_t pmm_get_total_memory(void);
size_t pmm_get_free_memory(void);
