#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

#include "debug.h"
#include "limine/boot.h"
#include "limine/limine.h"
#include "sbi.h"

void init(void) {
	sbi_init();

	if (!LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision))
		early_panic("Limine failed to provide base revision 6\n");

	early_panic("Hello, world from kernel!\n");
	while (1);
}
