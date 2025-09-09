#include "limine/limine.h"
#include "limine/features.h"

#include <stddef.h>

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

struct limine_executable_address_request executable_address_request = {
    LIMINE_EXECUTABLE_ADDRESS_REQUEST, 0, NULL};