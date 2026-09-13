#include "limine/boot.h"
#include "limine/limine.h"

#include <stddef.h>
#include <stdint.h>

__attribute__((used, section(".limine_requests")))
volatile uint64_t limine_base_revision[3] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[4] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_cmdline_request executable_cmdline_request = {
	.id = LIMINE_EXECUTABLE_CMDLINE_REQUEST_ID,
	.revision = 0,
	.response = NULL
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
	.id = LIMINE_HHDM_REQUEST_ID,
	.revision = 0,
	.response = NULL
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request executable_address_request = {
	.id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
	.revision = 0,
	.response = NULL
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST_ID,
	.revision = 0,
	.response = NULL
};

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[2] = LIMINE_REQUESTS_END_MARKER;

/* ========================================================================= */
/* Virtual-to-Physical (VTOP) Translation                                    */
/* ========================================================================= */

void *limine_hhdm_vtop(const void *addr) {
	return (void *)((uintptr_t)addr - hhdm_request.response->offset);
}

void *limine_exe_vtop(const void *addr) {
	return (void *)((uintptr_t)addr -
	                executable_address_request.response->virtual_base +
	                executable_address_request.response->physical_base);
}
