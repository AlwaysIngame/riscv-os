#pragma once

#include <stdint.h>

#include "limine/limine.h"

extern uint64_t limine_base_revision[3];

extern struct limine_executable_cmdline_request executable_cmdline_request;

extern struct limine_hhdm_request hhdm_request;

extern struct limine_paging_mode_request paging_mode_request;

extern struct limine_executable_address_request executable_address_request;

#define LIMINE_HHDM_VTOP(addr) \
	((void *)((uint64_t)addr - hhdm_request.response->offset))

#define LIMINE_EXE_VTOP(addr)                                     \
	((void *)((uint64_t)addr -                                    \
			  executable_address_request.response->virtual_base + \
			  executable_address_request.response->physical_base))