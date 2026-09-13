#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

#include "riscv.h"
#include "debug.h"
#include "limine/boot.h"
#include "limine/limine.h"
#include "mm/pmm.h"
#include "sbi.h"

void init(void) {
	sbi_init();

	if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision))
		early_panic("Limine failed to provide base revision 6\n");

	debug_puts("Hello, world from kernel!\n");

	pmm_init();

	debug_puts("[PMM] Running self-tests...\n");

	size_t initial_free = pmm_get_free_pages();
	uintptr_t page1 = pmm_alloc_page();
	if (!page1 || !page_is_aligned(page1)) {
		early_panic("Self-test failed: invalid page1 allocation\n");
	}

	uintptr_t page2 = pmm_zalloc_page();
	if (!page2 || !page_is_aligned(page2) || page2 == page1) {
		early_panic("Self-test failed: invalid page2 allocation\n");
	}

	uint8_t* vpage2 = (uint8_t*)limine_hhdm_ptov(page2);
	for (size_t i = 0; i < (size_t)PAGE_SIZE; i++) {
		if (vpage2[i] != 0) {
			early_panic(
				"Self-test failed: pmm_zalloc_page did not zero memory\n");
		}
	}

	constexpr uint64_t TEST_MAGIC = 0xdeadbeefcafebabeULL;
	uint64_t* vpage1 = (uint64_t*)limine_hhdm_ptov(page1);
	vpage1[0] = TEST_MAGIC;
	if (vpage1[0] != TEST_MAGIC) {
		early_panic("Self-test failed: memory read/write mismatch on page1\n");
	}

	if (pmm_get_free_pages() != initial_free - 2) {
		early_panic(
			"Self-test failed: free page count mismatch after allocation\n");
	}

	pmm_free_page(page2);
	pmm_free_page(page1);

	if (pmm_get_free_pages() != initial_free) {
		early_panic("Self-test failed: free page count mismatch after free\n");
	}

	uintptr_t reallocated = pmm_alloc_page();
	if (reallocated != page1) {
		early_panic("Self-test failed: LIFO order not preserved\n");
	}
	pmm_free_page(reallocated);

	debug_puts("[PMM] Self-tests passed successfully!\n");

	early_panic("Kernel Done!");
}
