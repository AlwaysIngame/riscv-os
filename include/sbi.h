/* SBI Environment Call functions */

#pragma once

#include <stdint.h>

struct sbiret {
	long error;
	union {
		long value;
		unsigned long uvalue;
	};
};

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

/* Add a hartid to a hartmask. Hart mask is a list of hartids from
   hart_mask_base to hart_mask_base + 63. 0 is an empty hart mask. Returns -1 if
   hartid is out of range, else 0. */
int sbi_hartmask_add(unsigned long *hart_mask, unsigned long hart_mask_base,
					 unsigned long hartid);

// Base Extension

struct sbiret sbi_get_spec_version(void);
struct sbiret sbi_get_impl_id(void);
struct sbiret sbi_get_impl_version(void);
struct sbiret sbi_probe_extension(long extension_id);
struct sbiret sbi_get_mvendorid(void);
struct sbiret sbi_get_marchid(void);
struct sbiret sbi_get_mimpid(void);

// Timer Extension "TIME"

struct sbiret sbi_set_timer(uint64_t stime_value);

// IPI Extension "sPI"

struct sbiret sbi_send_ipi(unsigned long hart_mask,
						   unsigned long hart_mask_base);

// RFENCE Extension "RFNC"

struct sbiret sbi_remote_fence_i(unsigned long hart_mask,
								 unsigned long hart_mask_base);
struct sbiret sbi_remote_sfence_vma(unsigned long hart_mask,
									unsigned long hart_mask_base,
									unsigned long start_addr,
									unsigned long size);
struct sbiret sbi_remote_sfence_vma_asid(unsigned long hart_mask,
										 unsigned long hart_mask_base,
										 unsigned long start_addr,
										 unsigned long size,
										 unsigned long asid);
struct sbiret sbi_remote_hfence_gvma_vmid(unsigned long hart_mask,
										  unsigned long hart_mask_base,
										  unsigned long start_addr,
										  unsigned long size,
										  unsigned long vmid);
struct sbiret sbi_remote_hfence_gvma(unsigned long hart_mask,
									 unsigned long hart_mask_base,
									 unsigned long start_addr,
									 unsigned long size);
struct sbiret sbi_remote_hfence_vvma_asid(unsigned long hart_mask,
										  unsigned long hart_mask_base,
										  unsigned long start_addr,
										  unsigned long size,
										  unsigned long asid);
struct sbiret sbi_remote_hfence_vvma(unsigned long hart_mask,
									 unsigned long hart_mask_base,
									 unsigned long start_addr,
									 unsigned long size);

// Hart State Management Extension "HSM"

struct sbiret sbi_hart_start(unsigned long hartid, unsigned long start_addr,
							 unsigned long opaque);
struct sbiret sbi_hart_stop(void);
struct sbiret sbi_hart_get_status(unsigned long hartid);
struct sbiret sbi_hart_suspend(uint32_t suspend_type, unsigned long resume_addr,
							   unsigned long opaque);

// System Reset Extension "SRST"

struct sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason);

// Performance Monitoring Unit Extension "PMU"

struct sbiret sbi_pmu_num_counters(void);
struct sbiret sbi_pmu_counter_get_info(unsigned long counter_idx);
struct sbiret sbi_pmu_counter_config_matching(unsigned long counter_idx_base,
											  unsigned long counter_idx_mask,
											  unsigned long config_flags,
											  unsigned long event_idx,
											  uint64_t event_data);
struct sbiret sbi_pmu_counter_start(unsigned long counter_idx_base,
									unsigned long counter_idx_mask,
									unsigned long start_flags,
									uint64_t initial_value);
struct sbiret sbi_pmu_counter_stop(unsigned long counter_idx_base,
								   unsigned long counter_idx_mask,
								   unsigned long stop_flags);
struct sbiret sbi_pmu_counter_fw_read(unsigned long counter_idx);
struct sbiret sbi_pmu_counter_fw_read_hi(unsigned long counter_idx);
struct sbiret sbi_pmu_snapshot_set_shmem(unsigned long shmem_phys_lo,
										 unsigned long shmem_phys_hi,
										 unsigned long flags);
struct sbiret sbi_pmu_event_get_info(unsigned long shmem_phys_lo,
									 unsigned long shmem_phys_hi,
									 unsigned long num_entries,
									 unsigned long flags);

// Debug Console Extension "DBCN"

struct sbiret sbi_debug_console_write(unsigned long num_bytes,
									  unsigned long base_addr_lo,
									  unsigned long base_addr_hi);
struct sbiret sbi_debug_console_read(unsigned long num_bytes,
									 unsigned long base_addr_lo,
									 unsigned long base_addr_hi);
struct sbiret sbi_debug_console_write_byte(uint8_t byte);

// System Suspend Extension "SUSP"

struct sbiret sbi_system_suspend(uint32_t sleep_type, unsigned long resume_addr,
								 unsigned long opaque);

// CPPC Extension "CPPC"

struct sbiret sbi_cppc_probe(uint32_t cppc_reg_id);
struct sbiret sbi_cppc_read(uint32_t cppc_reg_id);
struct sbiret sbi_cppc_read_hi(uint32_t cppc_reg_id);
struct sbiret sbi_cppc_write(uint32_t cppc_reg_id, uint64_t val);

// Nested Acceleration Extension "NACL"

struct sbiret sbi_nacl_probe_feature(uint32_t feature_id);
struct sbiret sbi_nacl_set_shmem(unsigned long shmem_phys_lo,
								 unsigned long shmem_phys_hi,
								 unsigned long flags);
struct sbiret sbi_nacl_sync_csr(unsigned long csr_num);
struct sbiret sbi_nacl_sync_hfence(unsigned long entry_index);
struct sbiret sbi_nacl_sync_sret(void);

// Steal-time Accounting Extension "STA"

struct sbiret sbi_steal_time_set_shmem(unsigned long shmem_phys_lo,
									   unsigned long shmem_phys_hi,
									   unsigned long flags);

// Supervisor Software Events Extension "SSE"

struct sbiret sbi_sse_read_attrs(uint32_t event_id, uint32_t base_attr_id,
								 uint32_t attr_count,
								 unsigned long output_phys_lo,
								 unsigned long output_phys_hi);
struct sbiret sbi_sse_write_attrs(uint32_t event_id, uint32_t base_attr_id,
								  uint32_t attr_count,
								  unsigned long input_phys_lo,
								  unsigned long input_phys_hi);
struct sbiret sbi_sse_register(uint32_t event_id,
							   unsigned long handler_entry_pc,
							   unsigned long handler_entry_arg);
struct sbiret sbi_sse_unregister(uint32_t event_id);
struct sbiret sbi_sse_enable(uint32_t event_id);
struct sbiret sbi_sse_disable(uint32_t event_id);
struct sbiret sbi_sse_complete(void);
struct sbiret sbi_sse_inject(uint32_t event_id, unsigned long hart_id);
struct sbiret sbi_sse_hart_unmask(void);
struct sbiret sbi_sse_hart_mask(void);

// SBI Firmware Features Extension "FWFT"

struct sbiret sbi_fwft_set(uint32_t feature, unsigned long value,
						   unsigned long flags);
struct sbiret sbi_fwft_get(uint32_t feature);

// Debug Triggers Extension "DBTR"

struct sbiret sbi_debug_num_triggers(unsigned long trig_tdata1);
struct sbiret sbi_debug_set_shmem(unsigned long shmem_phys_lo,
								  unsigned long shmem_phys_hi,
								  unsigned long flags);
struct sbiret sbi_debug_read_triggers(unsigned long trig_idx_base,
									  unsigned long trig_count);
struct sbiret sbi_debug_install_triggers(unsigned long trig_count);
struct sbiret sbi_debug_update_triggers(unsigned long trig_count);
struct sbiret sbi_debug_uninstall_triggers(unsigned long trig_idx_base,
										   unsigned long trig_idx_mask);
struct sbiret sbi_debug_enable_triggers(unsigned long trig_idx_base,
										unsigned long trig_idx_mask);
struct sbiret sbi_debug_disable_triggers(unsigned long trig_idx_base,
										 unsigned long trig_idx_mask);

// Message Proxy Extension "MPXY"

struct sbiret sbi_mpxy_get_shmem_size(void);
struct sbiret sbi_mpxy_set_shmem(unsigned long shmem_phys_lo,
								 unsigned long shmem_phys_hi,
								 unsigned long flags);
struct sbiret sbi_mpxy_get_channel_ids(uint32_t start_index);
struct sbiret sbi_mpxy_read_attributes(uint32_t channel_id,
									   uint32_t base_attribute_id,
									   uint32_t attribute_count);
struct sbiret sbi_mpxy_write_attributes(uint32_t channel_id,
										uint32_t base_attribute_id,
										uint32_t attribute_count);
struct sbiret sbi_mpxy_send_message_with_response(
	uint32_t channel_id, uint32_t message_id, unsigned long message_data_len);
struct sbiret sbi_mpxy_send_message_without_response(
	uint32_t channel_id, uint32_t message_id, unsigned long message_data_len);
struct sbiret sbi_mpxy_get_notification_events(uint32_t channel_id);