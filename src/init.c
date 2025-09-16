#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

#include "debug.h"
#include "limine/features.h"
#include "sbi.h"

void init(void) {
	int i;
	i = 4;
	i = i + 1;
	sbi_init();

	if (limine_base_revision[2] == 3)
		early_panic("Limine failed to provide revision 3");

	early_panic("Hello, world from kernel!\n");
	while (1);
}
