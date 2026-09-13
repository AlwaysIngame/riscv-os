#include "sbi.h"

/*
 * RISC-V Supervisor Binary Interface (SBI) Specification
 * Version 3.0 Implementation
 */

/* ========================================================================= */
/* Low-level SBI Call Interface                                              */
/* ========================================================================= */

struct sbiret sbi_ecall(long eid, long fid,
                        unsigned long arg0, unsigned long arg1,
                        unsigned long arg2, unsigned long arg3,
                        unsigned long arg4, unsigned long arg5) {
	register unsigned long a0 __asm__("a0") = arg0;
	register unsigned long a1 __asm__("a1") = arg1;
	register unsigned long a2 __asm__("a2") = arg2;
	register unsigned long a3 __asm__("a3") = arg3;
	register unsigned long a4 __asm__("a4") = arg4;
	register unsigned long a5 __asm__("a5") = arg5;
	register unsigned long a6 __asm__("a6") = fid;
	register unsigned long a7 __asm__("a7") = eid;

	__asm__ volatile(
		"ecall"
		: "+r"(a0), "+r"(a1)
		: "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
		: "memory"
	);

	struct sbiret ret;
	ret.error = (long)a0;
	ret.value = (long)a1;
	return ret;
}

long sbi_ecall_legacy(long eid, unsigned long arg0,
                      unsigned long arg1, unsigned long arg2) {
	register unsigned long a0 __asm__("a0") = arg0;
	register unsigned long a1 __asm__("a1") = arg1;
	register unsigned long a2 __asm__("a2") = arg2;
	register unsigned long a7 __asm__("a7") = eid;

	__asm__ volatile(
		"ecall"
		: "+r"(a0)
		: "r"(a1), "r"(a2), "r"(a7)
		: "memory"
	);

	return (long)a0;
}

/* ========================================================================= */
/* Base Extension (EID #0x10)                                                */
/* ========================================================================= */

struct sbiret sbi_get_spec_version(void) {
	return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_SPEC_VERSION, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_get_impl_id(void) {
	return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_IMPL_ID, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_get_impl_version(void) {
	return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_IMPL_VERSION, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_probe_extension(long extension_id) {
	return sbi_ecall(SBI_EXT_BASE, SBI_BASE_PROBE_EXTENSION, extension_id, 0, 0, 0, 0, 0);
}

struct sbiret sbi_get_mvendorid(void) {
	return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_MVENDORID, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_get_marchid(void) {
	return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_MARCHID, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_get_mimpid(void) {
	return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_MIMPID, 0, 0, 0, 0, 0, 0);
}

/* ========================================================================= */
/* Legacy Extensions (EIDs #0x00 - #0x08)                                    */
/* ========================================================================= */

long sbi_set_timer_legacy(uint64_t stime_value) {
	return sbi_ecall_legacy(SBI_EXT_0_1_SET_TIMER, (unsigned long)stime_value, 0, 0);
}

long sbi_console_putchar(int ch) {
	return sbi_ecall_legacy(SBI_EXT_0_1_CONSOLE_PUTCHAR, (unsigned long)ch, 0, 0);
}

long sbi_console_getchar(void) {
	return sbi_ecall_legacy(SBI_EXT_0_1_CONSOLE_GETCHAR, 0, 0, 0);
}

long sbi_clear_ipi(void) {
	return sbi_ecall_legacy(SBI_EXT_0_1_CLEAR_IPI, 0, 0, 0);
}

long sbi_send_ipi_legacy(const unsigned long *hart_mask) {
	return sbi_ecall_legacy(SBI_EXT_0_1_SEND_IPI, (unsigned long)hart_mask, 0, 0);
}

long sbi_remote_fence_i_legacy(const unsigned long *hart_mask) {
	return sbi_ecall_legacy(SBI_EXT_0_1_REMOTE_FENCE_I, (unsigned long)hart_mask, 0, 0);
}

long sbi_remote_sfence_vma_legacy(const unsigned long *hart_mask,
                                 unsigned long start, unsigned long size) {
	return sbi_ecall_legacy(SBI_EXT_0_1_REMOTE_SFENCE_VMA, (unsigned long)hart_mask,
	                        start, size);
}

long sbi_remote_sfence_vma_asid_legacy(const unsigned long *hart_mask,
                                      unsigned long start, unsigned long size,
                                      unsigned long asid) {
	register unsigned long a0 __asm__("a0") = (unsigned long)hart_mask;
	register unsigned long a1 __asm__("a1") = start;
	register unsigned long a2 __asm__("a2") = size;
	register unsigned long a3 __asm__("a3") = asid;
	register unsigned long a7 __asm__("a7") = SBI_EXT_0_1_REMOTE_SFENCE_VMA_ASID;

	__asm__ volatile(
		"ecall"
		: "+r"(a0)
		: "r"(a1), "r"(a2), "r"(a3), "r"(a7)
		: "memory"
	);

	return (long)a0;
}

long sbi_shutdown(void) {
	return sbi_ecall_legacy(SBI_EXT_0_1_SHUTDOWN, 0, 0, 0);
}

/* ========================================================================= */
/* Timer Extension (EID #0x54494D45 "TIME")                                  */
/* ========================================================================= */

struct sbiret sbi_set_timer(uint64_t stime_value) {
	return sbi_ecall(SBI_EXT_TIME, SBI_TIME_SET_TIMER, (unsigned long)stime_value,
	                 0, 0, 0, 0, 0);
}

/* ========================================================================= */
/* IPI Extension (EID #0x735049 "sPI")                                       */
/* ========================================================================= */

struct sbiret sbi_send_ipi(unsigned long hart_mask, unsigned long hart_mask_base) {
	return sbi_ecall(SBI_EXT_IPI, SBI_IPI_SEND_IPI, hart_mask, hart_mask_base,
	                 0, 0, 0, 0);
}

/* ========================================================================= */
/* RFENCE Extension (EID #0x52464E43 "RFNC")                                 */
/* ========================================================================= */

struct sbiret sbi_remote_fence_i(unsigned long hart_mask,
                                 unsigned long hart_mask_base) {
	return sbi_ecall(SBI_EXT_RFENCE, SBI_RFENCE_REMOTE_FENCE_I,
	                 hart_mask, hart_mask_base, 0, 0, 0, 0);
}

struct sbiret sbi_remote_sfence_vma(unsigned long hart_mask,
                                    unsigned long hart_mask_base,
                                    unsigned long start_addr,
                                    unsigned long size) {
	return sbi_ecall(SBI_EXT_RFENCE, SBI_RFENCE_REMOTE_SFENCE_VMA,
	                 hart_mask, hart_mask_base, start_addr, size, 0, 0);
}

struct sbiret sbi_remote_sfence_vma_asid(unsigned long hart_mask,
                                         unsigned long hart_mask_base,
                                         unsigned long start_addr,
                                         unsigned long size,
                                         unsigned long asid) {
	return sbi_ecall(SBI_EXT_RFENCE, SBI_RFENCE_REMOTE_SFENCE_VMA_ASID,
	                 hart_mask, hart_mask_base, start_addr, size, asid, 0);
}

struct sbiret sbi_remote_hfence_gvma_vmid(unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long vmid) {
	return sbi_ecall(SBI_EXT_RFENCE, SBI_RFENCE_REMOTE_HFENCE_GVMA_VMID,
	                 hart_mask, hart_mask_base, start_addr, size, vmid, 0);
}

struct sbiret sbi_remote_hfence_gvma(unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size) {
	return sbi_ecall(SBI_EXT_RFENCE, SBI_RFENCE_REMOTE_HFENCE_GVMA,
	                 hart_mask, hart_mask_base, start_addr, size, 0, 0);
}

struct sbiret sbi_remote_hfence_vvma_asid(unsigned long hart_mask,
                                          unsigned long hart_mask_base,
                                          unsigned long start_addr,
                                          unsigned long size,
                                          unsigned long asid) {
	return sbi_ecall(SBI_EXT_RFENCE, SBI_RFENCE_REMOTE_HFENCE_VVMA_ASID,
	                 hart_mask, hart_mask_base, start_addr, size, asid, 0);
}

struct sbiret sbi_remote_hfence_vvma(unsigned long hart_mask,
                                     unsigned long hart_mask_base,
                                     unsigned long start_addr,
                                     unsigned long size) {
	return sbi_ecall(SBI_EXT_RFENCE, SBI_RFENCE_REMOTE_HFENCE_VVMA,
	                 hart_mask, hart_mask_base, start_addr, size, 0, 0);
}

/* ========================================================================= */
/* Hart State Management Extension (EID #0x48534D "HSM")                     */
/* ========================================================================= */

struct sbiret sbi_hart_start(unsigned long hartid, unsigned long start_addr,
                             unsigned long opaque) {
	return sbi_ecall(SBI_EXT_HSM, SBI_HSM_HART_START,
	                 hartid, start_addr, opaque, 0, 0, 0);
}

struct sbiret sbi_hart_stop(void) {
	return sbi_ecall(SBI_EXT_HSM, SBI_HSM_HART_STOP, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_hart_get_status(unsigned long hartid) {
	return sbi_ecall(SBI_EXT_HSM, SBI_HSM_HART_GET_STATUS, hartid, 0, 0, 0, 0, 0);
}

struct sbiret sbi_hart_suspend(uint32_t suspend_type, unsigned long resume_addr,
                               unsigned long opaque) {
	return sbi_ecall(SBI_EXT_HSM, SBI_HSM_HART_SUSPEND,
	                 suspend_type, resume_addr, opaque, 0, 0, 0);
}

/* ========================================================================= */
/* System Reset Extension (EID #0x53525354 "SRST")                           */
/* ========================================================================= */

struct sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason) {
	return sbi_ecall(SBI_EXT_SRST, SBI_SRST_SYSTEM_RESET,
	                 reset_type, reset_reason, 0, 0, 0, 0);
}

/* ========================================================================= */
/* Performance Monitoring Unit Extension (EID #0x504D55 "PMU")               */
/* ========================================================================= */

struct sbiret sbi_pmu_num_counters(void) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_NUM_COUNTERS, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_pmu_counter_get_info(unsigned long counter_idx) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_COUNTER_GET_INFO, counter_idx,
	                 0, 0, 0, 0, 0);
}

struct sbiret sbi_pmu_counter_config_matching(unsigned long counter_idx_base,
                                              unsigned long counter_idx_mask,
                                              unsigned long config_flags,
                                              unsigned long event_idx,
                                              uint64_t event_data) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_COUNTER_CONFIG_MATCHING,
	                 counter_idx_base, counter_idx_mask, config_flags,
	                 event_idx, (unsigned long)event_data, 0);
}

struct sbiret sbi_pmu_counter_start(unsigned long counter_idx_base,
                                    unsigned long counter_idx_mask,
                                    unsigned long start_flags,
                                    uint64_t initial_value) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_COUNTER_START,
	                 counter_idx_base, counter_idx_mask, start_flags,
	                 (unsigned long)initial_value, 0, 0);
}

struct sbiret sbi_pmu_counter_stop(unsigned long counter_idx_base,
                                   unsigned long counter_idx_mask,
                                   unsigned long stop_flags) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_COUNTER_STOP,
	                 counter_idx_base, counter_idx_mask, stop_flags, 0, 0, 0);
}

struct sbiret sbi_pmu_counter_fw_read(unsigned long counter_idx) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_COUNTER_FW_READ, counter_idx,
	                 0, 0, 0, 0, 0);
}

struct sbiret sbi_pmu_counter_fw_read_hi(unsigned long counter_idx) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_COUNTER_FW_READ_HI, counter_idx,
	                 0, 0, 0, 0, 0);
}

struct sbiret sbi_pmu_snapshot_set_shmem(unsigned long shmem_phys_lo,
                                         unsigned long shmem_phys_hi,
                                         unsigned long flags) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_SNAPSHOT_SET_SHMEM,
	                 shmem_phys_lo, shmem_phys_hi, flags, 0, 0, 0);
}

struct sbiret sbi_pmu_event_get_info(unsigned long shmem_phys_lo,
                                     unsigned long shmem_phys_hi,
                                     unsigned long num_entries,
                                     unsigned long flags) {
	return sbi_ecall(SBI_EXT_PMU, SBI_PMU_EVENT_GET_INFO,
	                 shmem_phys_lo, shmem_phys_hi, num_entries, flags, 0, 0);
}

/* ========================================================================= */
/* Debug Console Extension (EID #0x4442434E "DBCN")                          */
/* ========================================================================= */

struct sbiret sbi_debug_console_write(unsigned long num_bytes,
                                      unsigned long base_addr_lo,
                                      unsigned long base_addr_hi) {
	return sbi_ecall(SBI_EXT_DBCN, SBI_DBCN_CONSOLE_WRITE,
	                 num_bytes, base_addr_lo, base_addr_hi, 0, 0, 0);
}

struct sbiret sbi_debug_console_read(unsigned long num_bytes,
                                     unsigned long base_addr_lo,
                                     unsigned long base_addr_hi) {
	return sbi_ecall(SBI_EXT_DBCN, SBI_DBCN_CONSOLE_READ,
	                 num_bytes, base_addr_lo, base_addr_hi, 0, 0, 0);
}

struct sbiret sbi_debug_console_write_byte(uint8_t byte) {
	return sbi_ecall(SBI_EXT_DBCN, SBI_DBCN_CONSOLE_WRITE_BYTE,
	                 (unsigned long)byte, 0, 0, 0, 0, 0);
}

/* ========================================================================= */
/* System Suspend Extension (EID #0x53555350 "SUSP")                         */
/* ========================================================================= */

struct sbiret sbi_system_suspend(uint32_t sleep_type, unsigned long resume_addr,
                                 unsigned long opaque) {
	return sbi_ecall(SBI_EXT_SUSP, SBI_SUSP_SYSTEM_SUSPEND,
	                 sleep_type, resume_addr, opaque, 0, 0, 0);
}

/* ========================================================================= */
/* CPPC Extension (EID #0x43505043 "CPPC")                                   */
/* ========================================================================= */

struct sbiret sbi_cppc_probe(uint32_t cppc_reg_id) {
	return sbi_ecall(SBI_EXT_CPPC, SBI_CPPC_PROBE, cppc_reg_id, 0, 0, 0, 0, 0);
}

struct sbiret sbi_cppc_read(uint32_t cppc_reg_id) {
	return sbi_ecall(SBI_EXT_CPPC, SBI_CPPC_READ, cppc_reg_id, 0, 0, 0, 0, 0);
}

struct sbiret sbi_cppc_read_hi(uint32_t cppc_reg_id) {
	return sbi_ecall(SBI_EXT_CPPC, SBI_CPPC_READ_HI, cppc_reg_id, 0, 0, 0, 0, 0);
}

struct sbiret sbi_cppc_write(uint32_t cppc_reg_id, uint64_t val) {
	return sbi_ecall(SBI_EXT_CPPC, SBI_CPPC_WRITE, cppc_reg_id,
	                 (unsigned long)val, 0, 0, 0, 0);
}

/* ========================================================================= */
/* Nested Acceleration Extension (EID #0x4E41434C "NACL")                    */
/* ========================================================================= */

struct sbiret sbi_nacl_probe_feature(uint32_t feature_id) {
	return sbi_ecall(SBI_EXT_NACL, SBI_NACL_PROBE_FEATURE, feature_id, 0, 0, 0, 0, 0);
}

struct sbiret sbi_nacl_set_shmem(unsigned long shmem_phys_lo,
                                 unsigned long shmem_phys_hi,
                                 unsigned long flags) {
	return sbi_ecall(SBI_EXT_NACL, SBI_NACL_SET_SHMEM,
	                 shmem_phys_lo, shmem_phys_hi, flags, 0, 0, 0);
}

struct sbiret sbi_nacl_sync_csr(unsigned long csr_num) {
	return sbi_ecall(SBI_EXT_NACL, SBI_NACL_SYNC_CSR, csr_num, 0, 0, 0, 0, 0);
}

struct sbiret sbi_nacl_sync_hfence(unsigned long entry_index) {
	return sbi_ecall(SBI_EXT_NACL, SBI_NACL_SYNC_HFENCE, entry_index, 0, 0, 0, 0, 0);
}

struct sbiret sbi_nacl_sync_sret(void) {
	return sbi_ecall(SBI_EXT_NACL, SBI_NACL_SYNC_SRET, 0, 0, 0, 0, 0, 0);
}

/* ========================================================================= */
/* Steal-time Accounting Extension (EID #0x535441 "STA")                     */
/* ========================================================================= */

struct sbiret sbi_steal_time_set_shmem(unsigned long shmem_phys_lo,
                                       unsigned long shmem_phys_hi,
                                       unsigned long flags) {
	return sbi_ecall(SBI_EXT_STA, SBI_STA_SET_SHMEM,
	                 shmem_phys_lo, shmem_phys_hi, flags, 0, 0, 0);
}

/* ========================================================================= */
/* Supervisor Software Events Extension (EID #0x535345 "SSE")                */
/* ========================================================================= */

struct sbiret sbi_sse_read_attrs(uint32_t event_id, uint32_t base_attr_id,
                                 uint32_t attr_count,
                                 unsigned long output_phys_lo,
                                 unsigned long output_phys_hi) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_READ_ATTRS,
	                 event_id, base_attr_id, attr_count,
	                 output_phys_lo, output_phys_hi, 0);
}

struct sbiret sbi_sse_write_attrs(uint32_t event_id, uint32_t base_attr_id,
                                  uint32_t attr_count,
                                  unsigned long input_phys_lo,
                                  unsigned long input_phys_hi) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_WRITE_ATTRS,
	                 event_id, base_attr_id, attr_count,
	                 input_phys_lo, input_phys_hi, 0);
}

struct sbiret sbi_sse_register(uint32_t event_id,
                               unsigned long handler_entry_pc,
                               unsigned long handler_entry_arg) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_REGISTER,
	                 event_id, handler_entry_pc, handler_entry_arg, 0, 0, 0);
}

struct sbiret sbi_sse_unregister(uint32_t event_id) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_UNREGISTER, event_id, 0, 0, 0, 0, 0);
}

struct sbiret sbi_sse_enable(uint32_t event_id) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_ENABLE, event_id, 0, 0, 0, 0, 0);
}

struct sbiret sbi_sse_disable(uint32_t event_id) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_DISABLE, event_id, 0, 0, 0, 0, 0);
}

struct sbiret sbi_sse_complete(void) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_COMPLETE, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_sse_inject(uint32_t event_id, unsigned long hart_id) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_INJECT, event_id, hart_id, 0, 0, 0, 0);
}

struct sbiret sbi_sse_hart_unmask(void) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_HART_UNMASK, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_sse_hart_mask(void) {
	return sbi_ecall(SBI_EXT_SSE, SBI_SSE_HART_MASK, 0, 0, 0, 0, 0, 0);
}

/* ========================================================================= */
/* SBI Firmware Features Extension (EID #0x46574654 "FWFT")                  */
/* ========================================================================= */

struct sbiret sbi_fwft_set(uint32_t feature, unsigned long value,
                           unsigned long flags) {
	return sbi_ecall(SBI_EXT_FWFT, SBI_FWFT_SET,
	                 feature, value, flags, 0, 0, 0);
}

struct sbiret sbi_fwft_get(uint32_t feature) {
	return sbi_ecall(SBI_EXT_FWFT, SBI_FWFT_GET, feature, 0, 0, 0, 0, 0);
}

/* ========================================================================= */
/* Debug Triggers Extension (EID #0x44425452 "DBTR")                          */
/* ========================================================================= */

struct sbiret sbi_debug_num_triggers(unsigned long trig_tdata1) {
	return sbi_ecall(SBI_EXT_DBTR, SBI_DBTR_NUM_TRIGGERS,
	                 trig_tdata1, 0, 0, 0, 0, 0);
}

struct sbiret sbi_debug_set_shmem(unsigned long shmem_phys_lo,
                                  unsigned long shmem_phys_hi,
                                  unsigned long flags) {
	return sbi_ecall(SBI_EXT_DBTR, SBI_DBTR_SET_SHMEM,
	                 shmem_phys_lo, shmem_phys_hi, flags, 0, 0, 0);
}

struct sbiret sbi_debug_read_triggers(unsigned long trig_idx_base,
                                      unsigned long trig_count) {
	return sbi_ecall(SBI_EXT_DBTR, SBI_DBTR_READ_TRIGGERS,
	                 trig_idx_base, trig_count, 0, 0, 0, 0);
}

struct sbiret sbi_debug_install_triggers(unsigned long trig_count) {
	return sbi_ecall(SBI_EXT_DBTR, SBI_DBTR_INSTALL_TRIGGERS,
	                 trig_count, 0, 0, 0, 0, 0);
}

struct sbiret sbi_debug_update_triggers(unsigned long trig_count) {
	return sbi_ecall(SBI_EXT_DBTR, SBI_DBTR_UPDATE_TRIGGERS,
	                 trig_count, 0, 0, 0, 0, 0);
}

struct sbiret sbi_debug_uninstall_triggers(unsigned long trig_idx_base,
                                           unsigned long trig_idx_mask) {
	return sbi_ecall(SBI_EXT_DBTR, SBI_DBTR_UNINSTALL_TRIGGERS,
	                 trig_idx_base, trig_idx_mask, 0, 0, 0, 0);
}

struct sbiret sbi_debug_enable_triggers(unsigned long trig_idx_base,
                                        unsigned long trig_idx_mask) {
	return sbi_ecall(SBI_EXT_DBTR, SBI_DBTR_ENABLE_TRIGGERS,
	                 trig_idx_base, trig_idx_mask, 0, 0, 0, 0);
}

struct sbiret sbi_debug_disable_triggers(unsigned long trig_idx_base,
                                         unsigned long trig_idx_mask) {
	return sbi_ecall(SBI_EXT_DBTR, SBI_DBTR_DISABLE_TRIGGERS,
	                 trig_idx_base, trig_idx_mask, 0, 0, 0, 0);
}

/* ========================================================================= */
/* Message Proxy Extension (EID #0x4D505859 "MPXY")                          */
/* ========================================================================= */

struct sbiret sbi_mpxy_get_shmem_size(void) {
	return sbi_ecall(SBI_EXT_MPXY, SBI_MPXY_GET_SHMEM_SIZE, 0, 0, 0, 0, 0, 0);
}

struct sbiret sbi_mpxy_set_shmem(unsigned long shmem_phys_lo,
                                 unsigned long shmem_phys_hi,
                                 unsigned long flags) {
	return sbi_ecall(SBI_EXT_MPXY, SBI_MPXY_SET_SHMEM,
	                 shmem_phys_lo, shmem_phys_hi, flags, 0, 0, 0);
}

struct sbiret sbi_mpxy_get_channel_ids(uint32_t start_index) {
	return sbi_ecall(SBI_EXT_MPXY, SBI_MPXY_GET_CHANNEL_IDS,
	                 start_index, 0, 0, 0, 0, 0);
}

struct sbiret sbi_mpxy_read_attributes(uint32_t channel_id,
                                       uint32_t base_attribute_id,
                                       uint32_t attribute_count) {
	return sbi_ecall(SBI_EXT_MPXY, SBI_MPXY_READ_ATTRIBUTES,
	                 channel_id, base_attribute_id, attribute_count, 0, 0, 0);
}

struct sbiret sbi_mpxy_write_attributes(uint32_t channel_id,
                                        uint32_t base_attribute_id,
                                        uint32_t attribute_count) {
	return sbi_ecall(SBI_EXT_MPXY, SBI_MPXY_WRITE_ATTRIBUTES,
	                 channel_id, base_attribute_id, attribute_count, 0, 0, 0);
}

struct sbiret sbi_mpxy_send_message_with_response(uint32_t channel_id,
                                                  uint32_t message_id,
                                                  unsigned long message_data_len) {
	return sbi_ecall(SBI_EXT_MPXY, SBI_MPXY_SEND_MSG_WITH_RESP,
	                 channel_id, message_id, message_data_len, 0, 0, 0);
}

struct sbiret sbi_mpxy_send_message_without_response(uint32_t channel_id,
                                                     uint32_t message_id,
                                                     unsigned long message_data_len) {
	return sbi_ecall(SBI_EXT_MPXY, SBI_MPXY_SEND_MSG_WITHOUT_RESP,
	                 channel_id, message_id, message_data_len, 0, 0, 0);
}

struct sbiret sbi_mpxy_get_notification_events(uint32_t channel_id) {
	return sbi_ecall(SBI_EXT_MPXY, SBI_MPXY_GET_NOTIFICATION_EVENTS,
	                 channel_id, 0, 0, 0, 0, 0);
}

/* ========================================================================= */
/* SBI Driver Lifecycle and Management                                       */
/* ========================================================================= */

#define SBI_DYNAMIC_CACHE_SIZE 16

struct sbi_ext_cache_entry {
	long eid;
	bool supported;
	bool valid;
};

struct sbi_driver_state {
	long spec_version;
	long impl_id;
	long impl_version;
	long mvendorid;
	long marchid;
	long mimpid;
	bool initialized;

	/* Pre-probed standard extensions */
	bool has_time;
	bool has_ipi;
	bool has_rfence;
	bool has_hsm;
	bool has_srst;
	bool has_pmu;
	bool has_dbcn;
	bool has_susp;
	bool has_cppc;
	bool has_nacl;
	bool has_sta;
	bool has_sse;
	bool has_fwft;
	bool has_dbtr;
	bool has_mpxy;

	/* Dynamic cache for non-standard / vendor / experimental extensions */
	struct sbi_ext_cache_entry dynamic_cache[SBI_DYNAMIC_CACHE_SIZE];
	unsigned int dynamic_cache_count;
};

static struct sbi_driver_state driver_state;

static bool raw_probe_extension(long eid) {
	struct sbiret ret = sbi_probe_extension(eid);
	return (ret.error == SBI_SUCCESS && ret.value != 0);
}

void sbi_init(void) {
	struct sbiret ret;

	ret = sbi_get_spec_version();
	driver_state.spec_version = (ret.error == SBI_SUCCESS) ? ret.value : 0;

	ret = sbi_get_impl_id();
	driver_state.impl_id = (ret.error == SBI_SUCCESS) ? ret.value : -1;

	ret = sbi_get_impl_version();
	driver_state.impl_version = (ret.error == SBI_SUCCESS) ? ret.value : 0;

	ret = sbi_get_mvendorid();
	driver_state.mvendorid = (ret.error == SBI_SUCCESS) ? ret.value : 0;

	ret = sbi_get_marchid();
	driver_state.marchid = (ret.error == SBI_SUCCESS) ? ret.value : 0;

	ret = sbi_get_mimpid();
	driver_state.mimpid = (ret.error == SBI_SUCCESS) ? ret.value : 0;

	/* Probe and cache all standard extensions defined in SBI v3.0 */
	driver_state.has_time   = raw_probe_extension(SBI_EXT_TIME);
	driver_state.has_ipi    = raw_probe_extension(SBI_EXT_IPI);
	driver_state.has_rfence = raw_probe_extension(SBI_EXT_RFENCE);
	driver_state.has_hsm    = raw_probe_extension(SBI_EXT_HSM);
	driver_state.has_srst   = raw_probe_extension(SBI_EXT_SRST);
	driver_state.has_pmu    = raw_probe_extension(SBI_EXT_PMU);
	driver_state.has_dbcn   = raw_probe_extension(SBI_EXT_DBCN);
	driver_state.has_susp   = raw_probe_extension(SBI_EXT_SUSP);
	driver_state.has_cppc   = raw_probe_extension(SBI_EXT_CPPC);
	driver_state.has_nacl   = raw_probe_extension(SBI_EXT_NACL);
	driver_state.has_sta    = raw_probe_extension(SBI_EXT_STA);
	driver_state.has_sse    = raw_probe_extension(SBI_EXT_SSE);
	driver_state.has_fwft   = raw_probe_extension(SBI_EXT_FWFT);
	driver_state.has_dbtr   = raw_probe_extension(SBI_EXT_DBTR);
	driver_state.has_mpxy   = raw_probe_extension(SBI_EXT_MPXY);

	driver_state.dynamic_cache_count = 0;
	driver_state.initialized = true;
}

bool sbi_probe(long extension_id) {
	if (!driver_state.initialized) {
		sbi_init();
	}

	switch (extension_id) {
	case SBI_EXT_BASE:
		return true;
	case SBI_EXT_TIME:
		return driver_state.has_time;
	case SBI_EXT_IPI:
		return driver_state.has_ipi;
	case SBI_EXT_RFENCE:
		return driver_state.has_rfence;
	case SBI_EXT_HSM:
		return driver_state.has_hsm;
	case SBI_EXT_SRST:
		return driver_state.has_srst;
	case SBI_EXT_PMU:
		return driver_state.has_pmu;
	case SBI_EXT_DBCN:
		return driver_state.has_dbcn;
	case SBI_EXT_SUSP:
		return driver_state.has_susp;
	case SBI_EXT_CPPC:
		return driver_state.has_cppc;
	case SBI_EXT_NACL:
		return driver_state.has_nacl;
	case SBI_EXT_STA:
		return driver_state.has_sta;
	case SBI_EXT_SSE:
		return driver_state.has_sse;
	case SBI_EXT_FWFT:
		return driver_state.has_fwft;
	case SBI_EXT_DBTR:
		return driver_state.has_dbtr;
	case SBI_EXT_MPXY:
		return driver_state.has_mpxy;
	default:
		break;
	}

	/* Check dynamic cache for other extensions */
	for (unsigned int i = 0; i < driver_state.dynamic_cache_count; i++) {
		if (driver_state.dynamic_cache[i].valid &&
		    driver_state.dynamic_cache[i].eid == extension_id) {
			return driver_state.dynamic_cache[i].supported;
		}
	}

	/* Not in cache: query firmware via SBI ecall */
	bool supported = raw_probe_extension(extension_id);

	/* Store in dynamic cache */
	if (driver_state.dynamic_cache_count < SBI_DYNAMIC_CACHE_SIZE) {
		unsigned int idx = driver_state.dynamic_cache_count++;
		driver_state.dynamic_cache[idx].eid = extension_id;
		driver_state.dynamic_cache[idx].supported = supported;
		driver_state.dynamic_cache[idx].valid = true;
	}

	return supported;
}

bool sbi_has_time(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_time;
}

bool sbi_has_ipi(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_ipi;
}

bool sbi_has_rfence(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_rfence;
}

bool sbi_has_hsm(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_hsm;
}

bool sbi_has_srst(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_srst;
}

bool sbi_has_pmu(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_pmu;
}

bool sbi_has_dbcn(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_dbcn;
}

bool sbi_has_susp(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_susp;
}

bool sbi_has_cppc(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_cppc;
}

bool sbi_has_nacl(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_nacl;
}

bool sbi_has_sta(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_sta;
}

bool sbi_has_sse(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_sse;
}

bool sbi_has_fwft(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_fwft;
}

bool sbi_has_dbtr(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_dbtr;
}

bool sbi_has_mpxy(void) {
	if (!driver_state.initialized) sbi_init();
	return driver_state.has_mpxy;
}

long sbi_spec_version(void) {
	return driver_state.spec_version;
}

long sbi_impl_id(void) {
	return driver_state.impl_id;
}

long sbi_impl_version(void) {
	return driver_state.impl_version;
}

const char *sbi_impl_name(long impl_id) {
	switch (impl_id) {
	case SBI_IMPL_BBL:
		return "Berkeley Boot Loader (BBL)";
	case SBI_IMPL_OPENSBI:
		return "OpenSBI";
	case SBI_IMPL_XVISOR:
		return "Xvisor";
	case SBI_IMPL_KVM:
		return "KVM";
	case SBI_IMPL_RUSTSBI:
		return "RustSBI";
	case SBI_IMPL_DIOSIX:
		return "Diosix";
	case SBI_IMPL_COFFER:
		return "Coffer";
	case SBI_IMPL_XEN:
		return "Xen Project";
	case SBI_IMPL_POLARFIRE_HSS:
		return "PolarFire Hart Software Services";
	case SBI_IMPL_COREBOOT:
		return "coreboot";
	case SBI_IMPL_OREBOOT:
		return "oreboot";
	case SBI_IMPL_BHYVE:
		return "bhyve";
	default:
		return "Unknown SBI Implementation";
	}
}

const char *sbi_strerror(long error) {
	switch (error) {
	case SBI_SUCCESS:
		return "SBI_SUCCESS";
	case SBI_ERR_FAILED:
		return "SBI_ERR_FAILED";
	case SBI_ERR_NOT_SUPPORTED:
		return "SBI_ERR_NOT_SUPPORTED";
	case SBI_ERR_INVALID_PARAM:
		return "SBI_ERR_INVALID_PARAM";
	case SBI_ERR_DENIED:
		return "SBI_ERR_DENIED";
	case SBI_ERR_INVALID_ADDRESS:
		return "SBI_ERR_INVALID_ADDRESS";
	case SBI_ERR_ALREADY_AVAILABLE:
		return "SBI_ERR_ALREADY_AVAILABLE";
	case SBI_ERR_ALREADY_STARTED:
		return "SBI_ERR_ALREADY_STARTED";
	case SBI_ERR_ALREADY_STOPPED:
		return "SBI_ERR_ALREADY_STOPPED";
	case SBI_ERR_NO_SHMEM:
		return "SBI_ERR_NO_SHMEM";
	case SBI_ERR_INVALID_STATE:
		return "SBI_ERR_INVALID_STATE";
	case SBI_ERR_BAD_RANGE:
		return "SBI_ERR_BAD_RANGE";
	case SBI_ERR_TIMEOUT:
		return "SBI_ERR_TIMEOUT";
	case SBI_ERR_IO:
		return "SBI_ERR_IO";
	case SBI_ERR_DENIED_LOCKED:
		return "SBI_ERR_DENIED_LOCKED";
	default:
		return "Unknown SBI Error";
	}
}
