#pragma once

#include <stdint.h>

extern volatile uint64_t limine_base_revision[3];

void *limine_hhdm_vtop(const void *addr);
void *limine_exe_vtop(const void *addr);
