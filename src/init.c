#include <stddef.h>

#include "init/boot_alloc.h"
#include "init/early_print.h"
#include "init/vm_early.h"
#include "limine/boot.h"
#include "limine/limine.h"
#include "sbi.h"

[[noreturn]] void init(void) {
	sbi_init();

	early_assert(LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision),
				 "Limine failed to provide base revision 6\n");
	early_assert(limine_verify_requests(),
				 "Limine failed to respond to all requests\n");

	early_puts("Hello, world from kernel!\n");

	boot_alloc_init();

	/* Initialize early kernel Sv39 page tables and per-hart virtual stacks */
	vm_early_init();

	early_panic("Kernel Done!\n");

}
