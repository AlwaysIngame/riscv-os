#include "limine/limine.h"
#include "limine/features.h"

#include <stddef.h>

LIMINE_BASE_REVISION(3)

struct limine_executable_cmdline_request executable_cmdline_request = {
	LIMINE_EXECUTABLE_CMDLINE_REQUEST, 0, NULL};

struct limine_hhdm_request hhdm_request = {LIMINE_HHDM_REQUEST, 0, NULL};

struct limine_executable_address_request executable_address_request = {
    LIMINE_EXECUTABLE_ADDRESS_REQUEST, 0, NULL};

struct limine_memmap_request memmap_request = {
	LIMINE_MEMMAP_REQUEST, 0, NULL};