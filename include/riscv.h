#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* RISC-V 4 KiB base page definitions */
constexpr size_t PAGE_SIZE = 4096ULL;
constexpr size_t PAGE_SHIFT = 12ULL;
constexpr uintptr_t PAGE_MASK = PAGE_SIZE - 1ULL;

/* Page alignment helpers */
static inline uintptr_t page_align_down(uintptr_t addr) {
	return addr & ~PAGE_MASK;
}

static inline uintptr_t page_align_up(uintptr_t addr) {
	return (addr + PAGE_MASK) & ~PAGE_MASK;
}

static inline bool page_is_aligned(uintptr_t addr) {
	return (addr & PAGE_MASK) == 0;
}

#define PAGE_ALIGN_DOWN(addr) page_align_down((uintptr_t)(addr))
#define PAGE_ALIGN_UP(addr) page_align_up((uintptr_t)(addr))
