#pragma once

#include <stddef.h>
#include <stdint.h>

/* Initialize the early boot memory allocator by parsing the Limine memory
   map. Adds all LIMINE_MEMMAP_USABLE pages to the intrusive free list. */
void boot_alloc_init(void);

/* Allocates a single physical page.
   Returns the virtual address of the page, or 0 if out of memory. */
void* boot_alloc_page(void);

/* Allocates a single physical page and zeroes its content.
   Returns the virtual address of the page, or 0 if out of memory. */
void* boot_zalloc_page(void);

/* Frees a single physical page given its virtual address.
   vaddr must be page-aligned and non-zero. */
void boot_free_page(void* vaddr);
