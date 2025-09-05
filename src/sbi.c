/* Implementation of SBI calls (version 3.0) */

#include "sbi.h"

#include <stdint.h>

#define SBI_SUCCESS 0
#define SBI_ERR_FAILED -1
#define SBI_ERR_NOT_SUPPORTED -2
#define SBI_ERR_INVALID_PARAM -3
#define SBI_ERR_DENIED -4
#define SBI_ERR_INVALID_ADDRESS -5
#define SBI_ERR_ALREADY_AVAILABLE -6
#define SBI_ERR_ALREADY_STARTED -7
#define SBI_ERR_ALREADY_STOPPED -8
#define SBI_ERR_NO_SHMEM -9
#define SBI_ERR_INVALID_STATE -10
#define SBI_ERR_BAD_RANGE -11
#define SBI_ERR_TIMEOUT -12
#define SBI_ERR_IO -13
#define SBI_ERR_DENIED_LOCKED -14

/* Standard Extensions */
#define SBI_EXT_BASE 0x10
#define SBI_EXT_TIME 0x54494d45
#define SBI_EXT_IPI 0x735049
#define SBI_EXT_RFENCE 0x52464e43
#define SBI_EXT_HSM 0x48534d
#define SBI_EXT_SRST 0x53525354
#define SBI_EXT_PMU 0x504d55
#define SBI_EXT_DBCN 0x4442434e
#define SBI_EXT_SUSP 0x53555350
#define SBI_EXT_CPPC 0x43505043
#define SBI_EXT_NACL 0x4e41434c
#define SBI_EXT_STA 0x535441
#define SBI_EXT_SSE 0x535345
#define SBI_EXT_FWFT 0x46574654
#define SBI_EXT_DBTR 0x44425452
#define SBI_EXT_MPXY 0x4d505859

int sbi_hartmask_add(unsigned long *hart_mask, unsigned long hart_mask_base,
					 unsigned long hartid) {
	if (hartid >= (8 * sizeof(unsigned long)) + hart_mask_base) return -1;
	*hart_mask |= (1UL << (hartid - hart_mask_base));
	return 0;
}

static inline struct sbiret sbi_ecall0(long ext, long fid) {
	struct sbiret ret;
	register uint64_t a0 asm("a0");
	register uint64_t a1 asm("a1");
	register uint64_t a6 asm("a6") = fid;
	register uint64_t a7 asm("a7") = ext;
	asm volatile("ecall" : "=r"(a0), "=r"(a1) : "r"(a6), "r"(a7) : "memory");
	ret.error = a0;
	ret.value = a1;
	return ret;
}

static inline struct sbiret sbi_ecall1(long ext, long fid, long arg0) {
	struct sbiret ret;
	register uint64_t a0 asm("a0") = arg0;
	register uint64_t a1 asm("a1");
	register uint64_t a6 asm("a6") = fid;
	register uint64_t a7 asm("a7") = ext;
	asm volatile("ecall" : "+r"(a0), "=r"(a1) : "r"(a6), "r"(a7) : "memory");
	ret.error = a0;
	ret.value = a1;
	return ret;
}

static inline struct sbiret sbi_ecall2(long ext, long fid, long arg0,
									   long arg1) {
	struct sbiret ret;
	register uint64_t a0 asm("a0") = arg0;
	register uint64_t a1 asm("a1") = arg1;
	register uint64_t a6 asm("a6") = fid;
	register uint64_t a7 asm("a7") = ext;
	asm volatile("ecall" : "+r"(a0), "+r"(a1) : "r"(a6), "r"(a7) : "memory");
	ret.error = a0;
	ret.uvalue = a1;
	return ret;
}

static inline struct sbiret sbi_ecall3(long ext, long fid, long arg0, long arg1,
									   long arg2) {
	struct sbiret ret;
	register uint64_t a0 asm("a0") = arg0;
	register uint64_t a1 asm("a1") = arg1;
	register uint64_t a2 asm("a2") = arg2;
	register uint64_t a6 asm("a6") = fid;
	register uint64_t a7 asm("a7") = ext;
	asm volatile("ecall"
				 : "+r"(a0), "+r"(a1)
				 : "r"(a2), "r"(a6), "r"(a7)
				 : "memory");
	ret.error = a0;
	ret.value = a1;
	return ret;
}

static inline struct sbiret sbi_ecall4(long ext, long fid, long arg0, long arg1,
									   long arg2, long arg3) {
	struct sbiret ret;
	register uint64_t a0 asm("a0") = arg0;
	register uint64_t a1 asm("a1") = arg1;
	register uint64_t a2 asm("a2") = arg2;
	register uint64_t a3 asm("a3") = arg3;
	register uint64_t a6 asm("a6") = fid;
	register uint64_t a7 asm("a7") = ext;
	asm volatile("ecall"
				 : "+r"(a0), "+r"(a1)
				 : "r"(a2), "r"(a3), "r"(a6), "r"(a7)
				 : "memory");
	ret.error = a0;
	ret.value = a1;
	return ret;
}

static inline struct sbiret sbi_ecall5(long ext, long fid, long arg0, long arg1,
									   long arg2, long arg3, long arg4) {
	struct sbiret ret;
	register uint64_t a0 asm("a0") = arg0;
	register uint64_t a1 asm("a1") = arg1;
	register uint64_t a2 asm("a2") = arg2;
	register uint64_t a3 asm("a3") = arg3;
	register uint64_t a4 asm("a4") = arg4;
	register uint64_t a6 asm("a6") = fid;
	register uint64_t a7 asm("a7") = ext;
	asm volatile("ecall"
				 : "+r"(a0), "+r"(a1)
				 : "r"(a2), "r"(a3), "r"(a4), "r"(a6), "r"(a7)
				 : "memory");
	ret.error = a0;
	ret.value = a1;
	return ret;
}

// Base Extension

struct sbiret sbi_get_spec_version(void) { return sbi_ecall0(SBI_EXT_BASE, 0); }

struct sbiret sbi_get_impl_id(void) { return sbi_ecall0(SBI_EXT_BASE, 1); }

struct sbiret sbi_get_impl_version(void) { return sbi_ecall0(SBI_EXT_BASE, 2); }

struct sbiret sbi_probe_extension(long extension_id) {
	return sbi_ecall1(SBI_EXT_BASE, 3, extension_id);
}

struct sbiret sbi_get_mvendorid(void) { return sbi_ecall0(SBI_EXT_BASE, 4); }

struct sbiret sbi_get_marchid(void) { return sbi_ecall0(SBI_EXT_BASE, 5); }

struct sbiret sbi_get_mimpid(void) { return sbi_ecall0(SBI_EXT_BASE, 6); }

// Timer Extension "TIME"

struct sbiret sbi_set_timer(uint64_t stime_value) {
	// On RV64, uint64_t is passed in a single register (a0)
	// On RV32, it should be passed in a0 and a1. This implementation assumes
	// XLEN=64.
	return sbi_ecall1(SBI_EXT_TIME, 0, (long)stime_value);
}

// IPI Extension "sPI"

struct sbiret sbi_send_ipi(unsigned long hart_mask,
						   unsigned long hart_mask_base) {
	return sbi_ecall2(SBI_EXT_IPI, 0, hart_mask, hart_mask_base);
}

// RFENCE Extension "RFNC"

struct sbiret sbi_remote_fence_i(unsigned long hart_mask,
								 unsigned long hart_mask_base) {
	return sbi_ecall2(SBI_EXT_RFENCE, 0, hart_mask, hart_mask_base);
}

struct sbiret sbi_remote_sfence_vma(unsigned long hart_mask,
									unsigned long hart_mask_base,
									unsigned long start_addr,
									unsigned long size) {
	return sbi_ecall4(SBI_EXT_RFENCE, 1, hart_mask, hart_mask_base, start_addr,
					  size);
}

struct sbiret sbi_remote_sfence_vma_asid(unsigned long hart_mask,
										 unsigned long hart_mask_base,
										 unsigned long start_addr,
										 unsigned long size,
										 unsigned long asid) {
	return sbi_ecall5(SBI_EXT_RFENCE, 2, hart_mask, hart_mask_base, start_addr,
					  size, asid);
}

struct sbiret sbi_remote_hfence_gvma_vmid(unsigned long hart_mask,
										  unsigned long hart_mask_base,
										  unsigned long start_addr,
										  unsigned long size,
										  unsigned long vmid) {
	return sbi_ecall5(SBI_EXT_RFENCE, 3, hart_mask, hart_mask_base, start_addr,
					  size, vmid);
}

struct sbiret sbi_remote_hfence_gvma(unsigned long hart_mask,
									 unsigned long hart_mask_base,
									 unsigned long start_addr,
									 unsigned long size) {
	return sbi_ecall4(SBI_EXT_RFENCE, 4, hart_mask, hart_mask_base, start_addr,
					  size);
}

struct sbiret sbi_remote_hfence_vvma_asid(unsigned long hart_mask,
										  unsigned long hart_mask_base,
										  unsigned long start_addr,
										  unsigned long size,
										  unsigned long asid) {
	return sbi_ecall5(SBI_EXT_RFENCE, 5, hart_mask, hart_mask_base, start_addr,
					  size, asid);
}

struct sbiret sbi_remote_hfence_vvma(unsigned long hart_mask,
									 unsigned long hart_mask_base,
									 unsigned long start_addr,
									 unsigned long size) {
	return sbi_ecall4(SBI_EXT_RFENCE, 6, hart_mask, hart_mask_base, start_addr,
					  size);
}

// Hart State Management Extension "HSM"

struct sbiret sbi_hart_start(unsigned long hartid, unsigned long start_addr,
							 unsigned long opaque) {
	return sbi_ecall3(SBI_EXT_HSM, 0, hartid, start_addr, opaque);
}

struct sbiret sbi_hart_stop(void) { return sbi_ecall0(SBI_EXT_HSM, 1); }

struct sbiret sbi_hart_get_status(unsigned long hartid) {
	return sbi_ecall1(SBI_EXT_HSM, 2, hartid);
}

struct sbiret sbi_hart_suspend(uint32_t suspend_type, unsigned long resume_addr,
							   unsigned long opaque) {
	return sbi_ecall3(SBI_EXT_HSM, 3, suspend_type, resume_addr, opaque);
}

// System Reset Extension "SRST"

struct sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason) {
	return sbi_ecall2(SBI_EXT_SRST, 0, reset_type, reset_reason);
}

// Performance Monitoring Unit Extension "PMU"

struct sbiret sbi_pmu_num_counters(void) { return sbi_ecall0(SBI_EXT_PMU, 0); }

struct sbiret sbi_pmu_counter_get_info(unsigned long counter_idx) {
	return sbi_ecall1(SBI_EXT_PMU, 1, counter_idx);
}

struct sbiret sbi_pmu_counter_config_matching(unsigned long counter_idx_base,
											  unsigned long counter_idx_mask,
											  unsigned long config_flags,
											  unsigned long event_idx,
											  uint64_t event_data) {
	return sbi_ecall5(SBI_EXT_PMU, 2, counter_idx_base, counter_idx_mask,
					  config_flags, event_idx, event_data);
}

struct sbiret sbi_pmu_counter_start(unsigned long counter_idx_base,
									unsigned long counter_idx_mask,
									unsigned long start_flags,
									uint64_t initial_value) {
	return sbi_ecall4(SBI_EXT_PMU, 3, counter_idx_base, counter_idx_mask,
					  start_flags, initial_value);
}

struct sbiret sbi_pmu_counter_stop(unsigned long counter_idx_base,
								   unsigned long counter_idx_mask,
								   unsigned long stop_flags) {
	return sbi_ecall3(SBI_EXT_PMU, 4, counter_idx_base, counter_idx_mask,
					  stop_flags);
}

struct sbiret sbi_pmu_counter_fw_read(unsigned long counter_idx) {
	return sbi_ecall1(SBI_EXT_PMU, 5, counter_idx);
}

struct sbiret sbi_pmu_counter_fw_read_hi(unsigned long counter_idx) {
	return sbi_ecall1(SBI_EXT_PMU, 6, counter_idx);
}

struct sbiret sbi_pmu_snapshot_set_shmem(unsigned long shmem_phys_lo,
										 unsigned long shmem_phys_hi,
										 unsigned long flags) {
	return sbi_ecall3(SBI_EXT_PMU, 7, shmem_phys_lo, shmem_phys_hi, flags);
}

struct sbiret sbi_pmu_event_get_info(unsigned long shmem_phys_lo,
									 unsigned long shmem_phys_hi,
									 unsigned long num_entries,
									 unsigned long flags) {
	return sbi_ecall4(SBI_EXT_PMU, 8, shmem_phys_lo, shmem_phys_hi, num_entries,
					  flags);
}

// Debug Console Extension "DBCN"

struct sbiret sbi_debug_console_write(unsigned long num_bytes,
									  unsigned long base_addr_lo,
									  unsigned long base_addr_hi) {
	return sbi_ecall3(SBI_EXT_DBCN, 0, num_bytes, base_addr_lo, base_addr_hi);
}

struct sbiret sbi_debug_console_read(unsigned long num_bytes,
									 unsigned long base_addr_lo,
									 unsigned long base_addr_hi) {
	return sbi_ecall3(SBI_EXT_DBCN, 1, num_bytes, base_addr_lo, base_addr_hi);
}

struct sbiret sbi_debug_console_write_byte(uint8_t byte) {
	return sbi_ecall1(SBI_EXT_DBCN, 2, byte);
}

// System Suspend Extension "SUSP"

struct sbiret sbi_system_suspend(uint32_t sleep_type, unsigned long resume_addr,
								 unsigned long opaque) {
	return sbi_ecall3(SBI_EXT_SUSP, 0, sleep_type, resume_addr, opaque);
}

// CPPC Extension "CPPC"

struct sbiret sbi_cppc_probe(uint32_t cppc_reg_id) {
	return sbi_ecall1(SBI_EXT_CPPC, 0, cppc_reg_id);
}

struct sbiret sbi_cppc_read(uint32_t cppc_reg_id) {
	return sbi_ecall1(SBI_EXT_CPPC, 1, cppc_reg_id);
}

struct sbiret sbi_cppc_read_hi(uint32_t cppc_reg_id) {
	return sbi_ecall1(SBI_EXT_CPPC, 2, cppc_reg_id);
}

struct sbiret sbi_cppc_write(uint32_t cppc_reg_id, uint64_t val) {
	return sbi_ecall2(SBI_EXT_CPPC, 3, cppc_reg_id, val);
}

// Nested Acceleration Extension "NACL"

struct sbiret sbi_nacl_probe_feature(uint32_t feature_id) {
	return sbi_ecall1(SBI_EXT_NACL, 0, feature_id);
}

struct sbiret sbi_nacl_set_shmem(unsigned long shmem_phys_lo,
								 unsigned long shmem_phys_hi,
								 unsigned long flags) {
	return sbi_ecall3(SBI_EXT_NACL, 1, shmem_phys_lo, shmem_phys_hi, flags);
}

struct sbiret sbi_nacl_sync_csr(unsigned long csr_num) {
	return sbi_ecall1(SBI_EXT_NACL, 2, csr_num);
}

struct sbiret sbi_nacl_sync_hfence(unsigned long entry_index) {
	return sbi_ecall1(SBI_EXT_NACL, 3, entry_index);
}

struct sbiret sbi_nacl_sync_sret(void) { return sbi_ecall0(SBI_EXT_NACL, 4); }

// Steal-time Accounting Extension "STA"

struct sbiret sbi_steal_time_set_shmem(unsigned long shmem_phys_lo,
									   unsigned long shmem_phys_hi,
									   unsigned long flags) {
	return sbi_ecall3(SBI_EXT_STA, 0, shmem_phys_lo, shmem_phys_hi, flags);
}

// Supervisor Software Events Extension "SSE"

struct sbiret sbi_sse_read_attrs(uint32_t event_id, uint32_t base_attr_id,
								 uint32_t attr_count,
								 unsigned long output_phys_lo,
								 unsigned long output_phys_hi) {
	return sbi_ecall5(SBI_EXT_SSE, 0, event_id, base_attr_id, attr_count,
					  output_phys_lo, output_phys_hi);
}

struct sbiret sbi_sse_write_attrs(uint32_t event_id, uint32_t base_attr_id,
								  uint32_t attr_count,
								  unsigned long input_phys_lo,
								  unsigned long input_phys_hi) {
	return sbi_ecall5(SBI_EXT_SSE, 1, event_id, base_attr_id, attr_count,
					  input_phys_lo, input_phys_hi);
}

struct sbiret sbi_sse_register(uint32_t event_id,
							   unsigned long handler_entry_pc,
							   unsigned long handler_entry_arg) {
	return sbi_ecall3(SBI_EXT_SSE, 2, event_id, handler_entry_pc,
					  handler_entry_arg);
}

struct sbiret sbi_sse_unregister(uint32_t event_id) {
	return sbi_ecall1(SBI_EXT_SSE, 3, event_id);
}

struct sbiret sbi_sse_enable(uint32_t event_id) {
	return sbi_ecall1(SBI_EXT_SSE, 4, event_id);
}

struct sbiret sbi_sse_disable(uint32_t event_id) {
	return sbi_ecall1(SBI_EXT_SSE, 5, event_id);
}

struct sbiret sbi_sse_complete(void) { return sbi_ecall0(SBI_EXT_SSE, 6); }

struct sbiret sbi_sse_inject(uint32_t event_id, unsigned long hart_id) {
	return sbi_ecall2(SBI_EXT_SSE, 7, event_id, hart_id);
}

struct sbiret sbi_sse_hart_unmask(void) { return sbi_ecall0(SBI_EXT_SSE, 8); }

struct sbiret sbi_sse_hart_mask(void) { return sbi_ecall0(SBI_EXT_SSE, 9); }

// SBI Firmware Features Extension "FWFT"

struct sbiret sbi_fwft_set(uint32_t feature, unsigned long value,
						   unsigned long flags) {
	return sbi_ecall3(SBI_EXT_FWFT, 0, feature, value, flags);
}

struct sbiret sbi_fwft_get(uint32_t feature) {
	return sbi_ecall1(SBI_EXT_FWFT, 1, feature);
}

// Debug Triggers Extension "DBTR"

struct sbiret sbi_debug_num_triggers(unsigned long trig_tdata1) {
	return sbi_ecall1(SBI_EXT_DBTR, 0, trig_tdata1);
}

struct sbiret sbi_debug_set_shmem(unsigned long shmem_phys_lo,
								  unsigned long shmem_phys_hi,
								  unsigned long flags) {
	return sbi_ecall3(SBI_EXT_DBTR, 1, shmem_phys_lo, shmem_phys_hi, flags);
}

struct sbiret sbi_debug_read_triggers(unsigned long trig_idx_base,
									  unsigned long trig_count) {
	return sbi_ecall2(SBI_EXT_DBTR, 2, trig_idx_base, trig_count);
}

struct sbiret sbi_debug_install_triggers(unsigned long trig_count) {
	return sbi_ecall1(SBI_EXT_DBTR, 3, trig_count);
}

struct sbiret sbi_debug_update_triggers(unsigned long trig_count) {
	return sbi_ecall1(SBI_EXT_DBTR, 4, trig_count);
}

struct sbiret sbi_debug_uninstall_triggers(unsigned long trig_idx_base,
										   unsigned long trig_idx_mask) {
	return sbi_ecall2(SBI_EXT_DBTR, 5, trig_idx_base, trig_idx_mask);
}

struct sbiret sbi_debug_enable_triggers(unsigned long trig_idx_base,
										unsigned long trig_idx_mask) {
	return sbi_ecall2(SBI_EXT_DBTR, 6, trig_idx_base, trig_idx_mask);
}

struct sbiret sbi_debug_disable_triggers(unsigned long trig_idx_base,
										 unsigned long trig_idx_mask) {
	return sbi_ecall2(SBI_EXT_DBTR, 7, trig_idx_base, trig_idx_mask);
}

// Message Proxy Extension "MPXY"

struct sbiret sbi_mpxy_get_shmem_size(void) {
	return sbi_ecall0(SBI_EXT_MPXY, 0);
}

struct sbiret sbi_mpxy_set_shmem(unsigned long shmem_phys_lo,
								 unsigned long shmem_phys_hi,
								 unsigned long flags) {
	return sbi_ecall3(SBI_EXT_MPXY, 1, shmem_phys_lo, shmem_phys_hi, flags);
}

struct sbiret sbi_mpxy_get_channel_ids(uint32_t start_index) {
	return sbi_ecall1(SBI_EXT_MPXY, 2, start_index);
}

struct sbiret sbi_mpxy_read_attributes(uint32_t channel_id,
									   uint32_t base_attribute_id,
									   uint32_t attribute_count) {
	return sbi_ecall3(SBI_EXT_MPXY, 3, channel_id, base_attribute_id,
					  attribute_count);
}

struct sbiret sbi_mpxy_write_attributes(uint32_t channel_id,
										uint32_t base_attribute_id,
										uint32_t attribute_count) {
	return sbi_ecall3(SBI_EXT_MPXY, 4, channel_id, base_attribute_id,
					  attribute_count);
}

struct sbiret sbi_mpxy_send_message_with_response(
	uint32_t channel_id, uint32_t message_id, unsigned long message_data_len) {
	return sbi_ecall3(SBI_EXT_MPXY, 5, channel_id, message_id,
					  message_data_len);
}

struct sbiret sbi_mpxy_send_message_without_response(
	uint32_t channel_id, uint32_t message_id, unsigned long message_data_len) {
	return sbi_ecall3(SBI_EXT_MPXY, 6, channel_id, message_id,
					  message_data_len);
}

struct sbiret sbi_mpxy_get_notification_events(uint32_t channel_id) {
	return sbi_ecall1(SBI_EXT_MPXY, 7, channel_id);
}