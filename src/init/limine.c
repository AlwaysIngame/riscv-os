#include "limine/limine.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "arch/pt.h"
#include "init/early_print.h"
#include "limine/boot.h"

__attribute__((
	used,
	section(".limine_base_revision"))) volatile uint64_t limine_base_revision[3] =
	LIMINE_BASE_REVISION(6);

__attribute__((used,
			   section(".limine_requests_start"))) static volatile uint64_t
	limine_requests_start_marker[4] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests"))) static volatile struct
	limine_executable_cmdline_request executable_cmdline_request = {
		.id = LIMINE_EXECUTABLE_CMDLINE_REQUEST_ID,
		.revision = 0,
		.response = nullptr};

__attribute__(
	(used,
	 section(".limine_requests"))) static volatile struct limine_hhdm_request
	hhdm_request = {
		.id = LIMINE_HHDM_REQUEST_ID, .revision = 0, .response = nullptr};

__attribute__((used, section(".limine_requests"))) static volatile struct
	limine_executable_address_request executable_address_request = {
		.id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
		.revision = 0,
		.response = nullptr};

__attribute__(
	(used,
	 section(".limine_requests"))) static volatile struct limine_memmap_request
	memmap_request = {
		.id = LIMINE_MEMMAP_REQUEST_ID, .revision = 0, .response = nullptr};

__attribute__(
	(used, section(".limine_requests"))) static volatile struct limine_mp_request
	mp_request = {.id = LIMINE_MP_REQUEST_ID,
				  .revision = 0,
				  .response = nullptr,
				  .flags = 0};

__attribute__(
	(used,
	 section(".limine_requests"))) static volatile struct limine_paging_mode_request
	paging_mode_request = {
		.id = LIMINE_PAGING_MODE_REQUEST_ID,
		.revision = 0,
		.response = nullptr,
		.mode = (VM_KERNEL_MODE == VM_MODE_SV39)   ? LIMINE_PAGING_MODE_RISCV_SV39
				: (VM_KERNEL_MODE == VM_MODE_SV48) ? LIMINE_PAGING_MODE_RISCV_SV48
													: LIMINE_PAGING_MODE_RISCV_SV57,
		.max_mode = (VM_KERNEL_MODE == VM_MODE_SV39)   ? LIMINE_PAGING_MODE_RISCV_SV39
					: (VM_KERNEL_MODE == VM_MODE_SV48) ? LIMINE_PAGING_MODE_RISCV_SV48
														: LIMINE_PAGING_MODE_RISCV_SV57,
		.min_mode = (VM_KERNEL_MODE == VM_MODE_SV39)   ? LIMINE_PAGING_MODE_RISCV_SV39
					: (VM_KERNEL_MODE == VM_MODE_SV48) ? LIMINE_PAGING_MODE_RISCV_SV48
														: LIMINE_PAGING_MODE_RISCV_SV57};

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t
	limine_requests_end_marker[2] = LIMINE_REQUESTS_END_MARKER;

/* ========================================================================= */
/* Request Verification                                                      */
/* ========================================================================= */

bool limine_verify_requests(void) {
	if (executable_cmdline_request.response == nullptr) {
		early_puts(
			"[Limine FAIL] Executable cmdline request not responded to\n");
		return false;
	}
	if (hhdm_request.response == nullptr) {
		early_puts("[Limine FAIL] HHDM request not responded to\n");
		return false;
	}
	if (executable_address_request.response == nullptr) {
		early_puts(
			"[Limine FAIL] Executable address request not responded to\n");
		return false;
	}
	if (memmap_request.response == nullptr) {
		early_puts("[Limine FAIL] Memory map request not responded to\n");
		return false;
	}
	if (memmap_request.response->entries == nullptr ||
		memmap_request.response->entry_count == 0) {
		early_puts("[Limine FAIL] Memory map entries missing or empty\n");
		return false;
	}
	if (mp_request.response == nullptr) {
		early_puts("[Limine FAIL] MP request not responded to\n");
		return false;
	}
	if (paging_mode_request.response == nullptr) {
		early_puts("[Limine FAIL] Paging mode request not responded to\n");
		return false;
	}
	return true;
}

/* ========================================================================= */
/* Request Accessors                                                         */
/* ========================================================================= */

struct limine_memmap_response* limine_get_memmap(void) {
	return memmap_request.response;
}

/* ========================================================================= */
/* Virtual-to-Physical & Physical-to-Virtual Translation                     */
/* ========================================================================= */

void* limine_hhdm_ptov(uintptr_t phys) {
	return (void*)(phys + hhdm_request.response->offset);
}

uintptr_t limine_hhdm_vtop(const void* addr) {
	return (uintptr_t)addr - hhdm_request.response->offset;
}

void* limine_exe_vtop(const void* addr) {
	return (void*)((uintptr_t)addr -
				   executable_address_request.response->virtual_base +
				   executable_address_request.response->physical_base);
}
