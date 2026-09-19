#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "limine/limine.h"

extern volatile uint64_t limine_base_revision[3];

/* Verifies that the bootloader responded to all registered Limine requests. */
bool limine_verify_requests(void);

struct limine_memmap_response* limine_get_memmap(void);

/* Translates a physical address to a virtual address in the Higher Half Direct
   Map. NOTE: Only regions mapped into HHDM (Usable, Bootloader Reclaimable,
   Executable, Framebuffer, Reserved Mapped, ACPI Reclaimable, ACPI NVS) can be
   safely accessed. */
void* limine_hhdm_ptov(uintptr_t phys);

/* Translates a virtual address in the Higher Half Direct Map to a physical
 * address. */
uintptr_t limine_hhdm_vtop(const void* addr);

/* Translates a kernel executable virtual address to a physical address. */
void* limine_exe_vtop(const void* addr);
