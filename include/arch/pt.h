#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ========================================================================= */
/* RISC-V RV64 Page Table Definitions & Bit Flags                            */
/* ========================================================================= */

typedef uint64_t pte_t;
typedef pte_t* page_table_t;

/* Standard 4 KiB Base Page */
constexpr size_t PAGE_SIZE = 4096ULL;
constexpr size_t PAGE_SHIFT = 12ULL;
constexpr uintptr_t PAGE_MASK = PAGE_SIZE - 1ULL;

/* Sv39 / Sv48 / Sv57 Level-1 Megapage (2 MiB) */
constexpr size_t MEGAPAGE_SIZE = 2ULL * 1024ULL * 1024ULL;
constexpr size_t MEGAPAGE_MASK = MEGAPAGE_SIZE - 1ULL;

/* Sv39 / Sv48 / Sv57 Level-2 Gigapage (1 GiB) */
constexpr size_t GIGAPAGE_SIZE = 1ULL * 1024ULL * 1024ULL * 1024ULL;
constexpr size_t GIGAPAGE_MASK = GIGAPAGE_SIZE - 1ULL;

/* Page Table Entry (PTE) Flags */
constexpr uint64_t PTE_V = 1ULL << 0; /* Valid */
constexpr uint64_t PTE_R = 1ULL << 1; /* Readable */
constexpr uint64_t PTE_W = 1ULL << 2; /* Writable */
constexpr uint64_t PTE_X = 1ULL << 3; /* Executable */
constexpr uint64_t PTE_U = 1ULL << 4; /* User accessible */
constexpr uint64_t PTE_G = 1ULL << 5; /* Global mapping */
constexpr uint64_t PTE_A = 1ULL << 6; /* Accessed */
constexpr uint64_t PTE_D = 1ULL << 7; /* Dirty */

/* RSW (Reserved for Software) bits 8-9 */
constexpr uint64_t PTE_RSW_MASK = 3ULL << 8;

/* Common Flag Combinations */
constexpr uint64_t PTE_RW = PTE_R | PTE_W;
constexpr uint64_t PTE_RX = PTE_R | PTE_X;
constexpr uint64_t PTE_RWX = PTE_R | PTE_W | PTE_X;

/* Extract Physical Page Number (PPN) from PTE (bits 53:10) */
constexpr size_t PTE_PPN_SHIFT = 10ULL;
constexpr uint64_t PTE_PPN_MASK = 0x003FFFFFFFFFFC00ULL;

/* VPN bit width per level in Sv39/Sv48/Sv57 */
constexpr size_t VM_VPN_BITS = 9ULL;
constexpr size_t VM_VPN_MASK = 0x1FFULL;
constexpr size_t VM_ENTRIES_PER_PT = 512ULL;

/* ========================================================================= */
/* Address Space Layout & Mode Configuration                                 */
/* ========================================================================= */

/* Supervisor Address Translation and Protection (satp) Register Definitions */
constexpr size_t SATP_MODE_SHIFT = 60ULL;
constexpr size_t SATP_ASID_SHIFT = 44ULL;
constexpr uint64_t SATP_ASID_MASK = 0xFFFFULL;
constexpr uint64_t SATP_PPN_MASK = 0xFFFFFFFFFFFULL;

/* RISC-V RV64 Address Translation Modes (satp.MODE) */
typedef enum vm_mode {
	VM_MODE_BARE = 0,
	VM_MODE_SV39 = 8,
	VM_MODE_SV48 = 9,
	VM_MODE_SV57 = 10,
} vm_mode_t;

/* Active Kernel VM Mode: configured at compile time (set to Sv39) */
constexpr vm_mode_t VM_KERNEL_MODE = VM_MODE_SV39;

/* Derived Compile-Time Architecture Constants */
constexpr int VM_LEVELS = (VM_KERNEL_MODE == VM_MODE_SV39)   ? 3
						  : (VM_KERNEL_MODE == VM_MODE_SV48) ? 4
						  : (VM_KERNEL_MODE == VM_MODE_SV57) ? 5
															 : 0;

constexpr int VM_ROOT_LEVEL = VM_LEVELS - 1;

/* Page alignment check helper */
static inline bool page_is_aligned(const void* addr) {
	return ((uintptr_t)addr & PAGE_MASK) == 0;
}

/* ========================================================================= */
/* Inline Translation Helpers                                                */
/* ========================================================================= */

/* Extracts the physical address from a PTE */
static inline uintptr_t vm_pte_to_paddr(pte_t pte) {
	return ((pte & PTE_PPN_MASK) >> PTE_PPN_SHIFT) << PAGE_SHIFT;
}

/* Encodes a physical address and permission flags into a PTE */
static inline pte_t vm_paddr_to_pte(uintptr_t paddr, uint64_t flags) {
	return ((paddr >> PAGE_SHIFT) << PTE_PPN_SHIFT) | flags;
}

/* Extracts the Virtual Page Number (VPN) for a specific tree level (0-indexed)
 */
static inline size_t vm_vpn(void* va, int level) {
	return ((uintptr_t)va >> (PAGE_SHIFT + VM_VPN_BITS * (size_t)level)) &
		   VM_VPN_MASK;
}

/* Computes the page size in bytes mapped by a leaf entry at `level` */
static inline size_t vm_level_page_size(int level) {
	return 1ULL << (PAGE_SHIFT + VM_VPN_BITS * (size_t)level);
}

/* Constructs a 64-bit `satp` register value */
static inline uint64_t vm_make_satp(uint16_t asid, uintptr_t root_paddr) {
	return ((uint64_t)VM_KERNEL_MODE << SATP_MODE_SHIFT) |
		   (((uint64_t)asid & SATP_ASID_MASK) << SATP_ASID_SHIFT) |
		   ((root_paddr >> PAGE_SHIFT) & SATP_PPN_MASK);
}

/* Checks if a PTE is a valid leaf entry (has read, write, or execute
 * permissions) */
static inline bool vm_pte_is_leaf(pte_t pte) {
	return (pte & PTE_V) && (pte & (PTE_R | PTE_W | PTE_X));
}

/* Checks if a PTE is a pointer to the next-level page table */
static inline bool vm_pte_is_table(pte_t pte) {
	return (pte & PTE_V) && !(pte & (PTE_R | PTE_W | PTE_X));
}

/* Writes the Supervisor Address Translation and Protection (satp) register */
static inline void vm_write_satp(uint64_t val) {
	__asm__ volatile("csrw satp, %0" ::"r"(val) : "memory");
}

/* Flushes all local TLB entries across all address spaces */
static inline void vm_flush_tlb(void) {
	__asm__ volatile("sfence.vma x0, x0" ::: "memory");
}
