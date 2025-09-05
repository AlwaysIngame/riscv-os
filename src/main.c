#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

#include "limine.h"
#include "sbi.h"

LIMINE_BASE_REVISION(3)

struct limine_executable_cmdline_request executable_cmdline_request = {
	LIMINE_EXECUTABLE_CMDLINE_REQUEST, 0, NULL};

struct limine_hhdm_request hhdm_request = {LIMINE_HHDM_REQUEST, 0, NULL};

struct limine_paging_mode_request paging_mode_request = {
	LIMINE_PAGING_MODE_REQUEST,
	1,
	NULL,
	.mode = LIMINE_PAGING_MODE_RISCV_SV39,
	.max_mode = LIMINE_PAGING_MODE_RISCV_SV39,
	.min_mode = LIMINE_PAGING_MODE_RISCV_SV39};

struct limine_kernel_address_request kernel_address_request = {
	LIMINE_KERNEL_ADDRESS_REQUEST, 0, NULL};

void kmain() {
	long sbi_dbcr = sbi_probe_extension(SBI_EXT_DBCN).value;
	struct sbiret ret;
	char * hw = "Hello, world!\n" - kernel_address_request.response->virtual_base + kernel_address_request.response->physical_base;
	if (sbi_dbcr > 0)
		ret = sbi_debug_console_write(14, (unsigned long)hw & ((1ul << 32) - 1),
									  (unsigned long)hw >> 32);
	ret.value = ret.value;
	if (limine_base_revision[2] == 3)
		;
}