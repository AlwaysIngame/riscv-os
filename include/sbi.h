#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * RISC-V Supervisor Binary Interface (SBI) Specification
 * Version 3.0
 */

/* ========================================================================= */
/* 1. SBI Return Structure and Standard Error Codes                          */
/* ========================================================================= */

struct sbiret {
	long error;
	union {
		long value;
		unsigned long uvalue;
	};
};

/* Standard SBI error codes (Table 1) */
#define SBI_SUCCESS                       0
#define SBI_ERR_FAILED                   -1
#define SBI_ERR_NOT_SUPPORTED            -2
#define SBI_ERR_INVALID_PARAM            -3
#define SBI_ERR_DENIED                   -4
#define SBI_ERR_INVALID_ADDRESS          -5
#define SBI_ERR_ALREADY_AVAILABLE        -6
#define SBI_ERR_ALREADY_STARTED          -7
#define SBI_ERR_ALREADY_STOPPED          -8
#define SBI_ERR_NO_SHMEM                 -9
#define SBI_ERR_INVALID_STATE           -10
#define SBI_ERR_BAD_RANGE               -11
#define SBI_ERR_TIMEOUT                 -12
#define SBI_ERR_IO                      -13
#define SBI_ERR_DENIED_LOCKED           -14

/* ========================================================================= */
/* 2. SBI Extension IDs (EIDs)                                               */
/* ========================================================================= */

/* Legacy Extensions (v0.1) */
#define SBI_EXT_0_1_SET_TIMER               0x00
#define SBI_EXT_0_1_CONSOLE_PUTCHAR         0x01
#define SBI_EXT_0_1_CONSOLE_GETCHAR         0x02
#define SBI_EXT_0_1_CLEAR_IPI               0x03
#define SBI_EXT_0_1_SEND_IPI                0x04
#define SBI_EXT_0_1_REMOTE_FENCE_I          0x05
#define SBI_EXT_0_1_REMOTE_SFENCE_VMA       0x06
#define SBI_EXT_0_1_REMOTE_SFENCE_VMA_ASID  0x07
#define SBI_EXT_0_1_SHUTDOWN                0x08

/* Standard Extensions (v0.2+) */
#define SBI_EXT_BASE                        0x10
#define SBI_EXT_TIME                        0x54494D45 /* "TIME" */
#define SBI_EXT_IPI                         0x735049   /* "sPI"  */
#define SBI_EXT_RFENCE                      0x52464E43 /* "RFNC" */
#define SBI_EXT_HSM                         0x48534D   /* "HSM"  */
#define SBI_EXT_SRST                        0x53525354 /* "SRST" */
#define SBI_EXT_PMU                         0x504D55   /* "PMU"  */
#define SBI_EXT_DBCN                        0x4442434E /* "DBCN" */
#define SBI_EXT_SUSP                        0x53555350 /* "SUSP" */
#define SBI_EXT_CPPC                        0x43505043 /* "CPPC" */
#define SBI_EXT_NACL                        0x4E41434C /* "NACL" */
#define SBI_EXT_STA                         0x535441   /* "STA"  */
#define SBI_EXT_SSE                         0x535345   /* "SSE"  */
#define SBI_EXT_FWFT                        0x46574654 /* "FWFT" */
#define SBI_EXT_DBTR                        0x44425452 /* "DBTR" */
#define SBI_EXT_MPXY                        0x4D505859 /* "MPXY" */

/* Experimental / Vendor / Firmware extension spaces */
#define SBI_EXT_EXPERIMENTAL_START          0x08000000
#define SBI_EXT_EXPERIMENTAL_END            0x08FFFFFF
#define SBI_EXT_VENDOR_START                0x09000000
#define SBI_EXT_VENDOR_END                  0x09FFFFFF
#define SBI_EXT_FIRMWARE_START              0x0A000000
#define SBI_EXT_FIRMWARE_END                0x0AFFFFFF

/* ========================================================================= */
/* 3. Base Extension (EID #0x10)                                             */
/* ========================================================================= */

#define SBI_BASE_GET_SPEC_VERSION           0
#define SBI_BASE_GET_IMPL_ID                1
#define SBI_BASE_GET_IMPL_VERSION           2
#define SBI_BASE_PROBE_EXTENSION            3
#define SBI_BASE_GET_MVENDORID              4
#define SBI_BASE_GET_MARCHID                5
#define SBI_BASE_GET_MIMPID                 6

/* SBI specification version decoding */
#define SBI_SPEC_VERSION_MAJOR_MASK         0x7F
#define SBI_SPEC_VERSION_MAJOR_SHIFT        24
#define SBI_SPEC_VERSION_MINOR_MASK         0xFFFFFF
#define SBI_SPEC_VERSION_MINOR_SHIFT        0
#define SBI_SPEC_VERSION(major, minor) \
	((((major) & SBI_SPEC_VERSION_MAJOR_MASK) << SBI_SPEC_VERSION_MAJOR_SHIFT) | \
	 (((minor) & SBI_SPEC_VERSION_MINOR_MASK) << SBI_SPEC_VERSION_MINOR_SHIFT))
#define SBI_SPEC_VERSION_MAJOR(ver) \
	(((ver) >> SBI_SPEC_VERSION_MAJOR_SHIFT) & SBI_SPEC_VERSION_MAJOR_MASK)
#define SBI_SPEC_VERSION_MINOR(ver) \
	(((ver) >> SBI_SPEC_VERSION_MINOR_SHIFT) & SBI_SPEC_VERSION_MINOR_MASK)

/* SBI Implementation IDs (Table 4) */
#define SBI_IMPL_BBL                        0
#define SBI_IMPL_OPENSBI                    1
#define SBI_IMPL_XVISOR                     2
#define SBI_IMPL_KVM                        3
#define SBI_IMPL_RUSTSBI                    4
#define SBI_IMPL_DIOSIX                     5
#define SBI_IMPL_COFFER                     6
#define SBI_IMPL_XEN                        7
#define SBI_IMPL_POLARFIRE_HSS              8
#define SBI_IMPL_COREBOOT                   9
#define SBI_IMPL_OREBOOT                   10
#define SBI_IMPL_BHYVE                     11

/* ========================================================================= */
/* 4. Timer Extension (EID #0x54494D45 "TIME")                               */
/* ========================================================================= */

#define SBI_TIME_SET_TIMER                  0

/* ========================================================================= */
/* 5. IPI Extension (EID #0x735049 "sPI")                                    */
/* ========================================================================= */

#define SBI_IPI_SEND_IPI                    0

/* ========================================================================= */
/* 6. RFENCE Extension (EID #0x52464E43 "RFNC")                              */
/* ========================================================================= */

#define SBI_RFENCE_REMOTE_FENCE_I           0
#define SBI_RFENCE_REMOTE_SFENCE_VMA        1
#define SBI_RFENCE_REMOTE_SFENCE_VMA_ASID   2
#define SBI_RFENCE_REMOTE_HFENCE_GVMA_VMID  3
#define SBI_RFENCE_REMOTE_HFENCE_GVMA       4
#define SBI_RFENCE_REMOTE_HFENCE_VVMA_ASID  5
#define SBI_RFENCE_REMOTE_HFENCE_VVMA       6

/* ========================================================================= */
/* 7. Hart State Management Extension (EID #0x48534D "HSM")                  */
/* ========================================================================= */

#define SBI_HSM_HART_START                  0
#define SBI_HSM_HART_STOP                   1
#define SBI_HSM_HART_GET_STATUS             2
#define SBI_HSM_HART_SUSPEND                3

/* Hart States (Table 17) */
#define SBI_HSM_STATE_STARTED               0
#define SBI_HSM_STATE_STOPPED               1
#define SBI_HSM_STATE_START_PENDING         2
#define SBI_HSM_STATE_STOP_PENDING          3
#define SBI_HSM_STATE_SUSPENDED             4
#define SBI_HSM_STATE_SUSPEND_PENDING       5
#define SBI_HSM_STATE_RESUME_PENDING        6

/* Hart Suspend Types (Table 23) */
#define SBI_HSM_SUSPEND_RETENTIVE           0x00000000
#define SBI_HSM_SUSPEND_NON_RETENTIVE       0x80000000

/* ========================================================================= */
/* 8. System Reset Extension (EID #0x53525354 "SRST")                        */
/* ========================================================================= */

#define SBI_SRST_SYSTEM_RESET               0

/* Reset Types (Table 26) */
#define SBI_SRST_RESET_TYPE_SHUTDOWN        0x00000000
#define SBI_SRST_RESET_TYPE_COLD_REBOOT     0x00000001
#define SBI_SRST_RESET_TYPE_WARM_REBOOT     0x00000002

/* Reset Reasons (Table 27) */
#define SBI_SRST_RESET_REASON_NO_REASON     0x00000000
#define SBI_SRST_RESET_REASON_SYS_FAILURE   0x00000001

/* ========================================================================= */
/* 9. Performance Monitoring Unit Extension (EID #0x504D55 "PMU")            */
/* ========================================================================= */

#define SBI_PMU_NUM_COUNTERS                0
#define SBI_PMU_COUNTER_GET_INFO            1
#define SBI_PMU_COUNTER_CONFIG_MATCHING     2
#define SBI_PMU_COUNTER_START               3
#define SBI_PMU_COUNTER_STOP                4
#define SBI_PMU_COUNTER_FW_READ             5
#define SBI_PMU_COUNTER_FW_READ_HI          6
#define SBI_PMU_SNAPSHOT_SET_SHMEM          7
#define SBI_PMU_EVENT_GET_INFO              8

/* PMU Event types (Table 30) */
#define SBI_PMU_EVENT_TYPE_HW_GENERAL       0
#define SBI_PMU_EVENT_TYPE_HW_CACHE         1
#define SBI_PMU_EVENT_TYPE_HW_RAW           2
#define SBI_PMU_EVENT_TYPE_HW_RAW_V2        3
#define SBI_PMU_EVENT_TYPE_FW               15

/* PMU Hardware Events (Table 31) */
#define SBI_PMU_HW_NO_EVENT                 0
#define SBI_PMU_HW_CPU_CYCLES               1
#define SBI_PMU_HW_INSTRUCTIONS            2
#define SBI_PMU_HW_CACHE_REFERENCES         3
#define SBI_PMU_HW_CACHE_MISSES             4
#define SBI_PMU_HW_BRANCH_INSTRUCTIONS      5
#define SBI_PMU_HW_BRANCH_MISSES            6
#define SBI_PMU_HW_BUS_CYCLES               7
#define SBI_PMU_HW_STALLED_CYCLES_FRONTEND  8
#define SBI_PMU_HW_STALLED_CYCLES_BACKEND   9
#define SBI_PMU_HW_REF_CPU_CYCLES           10

/* PMU Cache Event IDs (Table 32) */
#define SBI_PMU_HW_CACHE_L1D                0
#define SBI_PMU_HW_CACHE_L1I                1
#define SBI_PMU_HW_CACHE_LL                 2
#define SBI_PMU_HW_CACHE_DTLB               3
#define SBI_PMU_HW_CACHE_ITLB               4
#define SBI_PMU_HW_CACHE_BPU                5
#define SBI_PMU_HW_CACHE_NODE               6

/* PMU Cache Operation IDs (Table 33) */
#define SBI_PMU_HW_CACHE_OP_READ            0
#define SBI_PMU_HW_CACHE_OP_WRITE           1
#define SBI_PMU_HW_CACHE_OP_PREFETCH        2

/* PMU Cache Operation Result IDs (Table 34) */
#define SBI_PMU_HW_CACHE_RESULT_ACCESS      0
#define SBI_PMU_HW_CACHE_RESULT_MISS        1

/* PMU Firmware Events (Table 35) */
#define SBI_PMU_FW_MISALIGNED_LOAD          0
#define SBI_PMU_FW_MISALIGNED_STORE         1
#define SBI_PMU_FW_ACCESS_LOAD              2
#define SBI_PMU_FW_ACCESS_STORE             3
#define SBI_PMU_FW_ILLEGAL_INSN             4
#define SBI_PMU_FW_SET_TIMER                5
#define SBI_PMU_FW_IPI_SENT                 6
#define SBI_PMU_FW_IPI_RECEIVED             7
#define SBI_PMU_FW_FENCE_I_SENT             8
#define SBI_PMU_FW_FENCE_I_RECEIVED         9
#define SBI_PMU_FW_SFENCE_VMA_SENT          10
#define SBI_PMU_FW_SFENCE_VMA_RECEIVED      11
#define SBI_PMU_FW_SFENCE_VMA_ASID_SENT     12
#define SBI_PMU_FW_SFENCE_VMA_ASID_RECEIVED 13
#define SBI_PMU_FW_HFENCE_GVMA_SENT         14
#define SBI_PMU_FW_HFENCE_GVMA_RECEIVED     15
#define SBI_PMU_FW_HFENCE_GVMA_VMID_SENT    16
#define SBI_PMU_FW_HFENCE_GVMA_VMID_RECEIVED 17
#define SBI_PMU_FW_HFENCE_VVMA_SENT         18
#define SBI_PMU_FW_HFENCE_VVMA_RECEIVED     19
#define SBI_PMU_FW_HFENCE_VVMA_ASID_SENT    20
#define SBI_PMU_FW_HFENCE_VVMA_ASID_RECEIVED 21
#define SBI_PMU_FW_PLATFORM                 65535

/* PMU Config Matching Flags (Table 37) */
#define SBI_PMU_CFG_FLAG_SKIP_MATCH         (1UL << 0)
#define SBI_PMU_CFG_FLAG_CLEAR_VALUE        (1UL << 1)
#define SBI_PMU_CFG_FLAG_AUTO_START         (1UL << 2)
#define SBI_PMU_CFG_FLAG_SET_VUINH          (1UL << 3)
#define SBI_PMU_CFG_FLAG_SET_VSINH          (1UL << 4)
#define SBI_PMU_CFG_FLAG_SET_UINH           (1UL << 5)
#define SBI_PMU_CFG_FLAG_SET_SINH           (1UL << 6)
#define SBI_PMU_CFG_FLAG_SET_MINH           (1UL << 7)

/* PMU Start Flags (Table 39) */
#define SBI_PMU_START_SET_INIT_VALUE        (1UL << 0)
#define SBI_PMU_START_FLAG_INIT_SNAPSHOT    (1UL << 1)

/* PMU Stop Flags (Table 41) */
#define SBI_PMU_STOP_FLAG_RESET             (1UL << 0)
#define SBI_PMU_STOP_FLAG_TAKE_SNAPSHOT     (1UL << 1)

/* PMU Snapshot Shared Memory (Table 45) */
struct sbi_pmu_snapshot_shmem {
	uint64_t counter_overflow_bitmap;
	uint64_t counter_values[64];
	uint8_t reserved[3576];
};

/* PMU Event Info Entry (Table 47) */
struct sbi_pmu_event_info_entry {
	uint32_t event_idx;
	uint32_t output;
};

/* PMU counter info helpers */
#define SBI_PMU_CTR_INFO_CSR(info)          ((info) & 0xFFFUL)
#define SBI_PMU_CTR_INFO_WIDTH(info)        (((info) >> 12) & 0x3FUL)
#define SBI_PMU_CTR_INFO_TYPE(info)         (((info) >> 63) & 0x1UL)
#define SBI_PMU_CTR_TYPE_HW                 0
#define SBI_PMU_CTR_TYPE_FW                 1

/* ========================================================================= */
/* 10. Debug Console Extension (EID #0x4442434E "DBCN")                      */
/* ========================================================================= */

#define SBI_DBCN_CONSOLE_WRITE              0
#define SBI_DBCN_CONSOLE_READ               1
#define SBI_DBCN_CONSOLE_WRITE_BYTE         2

/* ========================================================================= */
/* 11. System Suspend Extension (EID #0x53555350 "SUSP")                     */
/* ========================================================================= */

#define SBI_SUSP_SYSTEM_SUSPEND             0

/* System Sleep Types (Table 54) */
#define SBI_SUSP_SLEEP_TYPE_SUSPEND_TO_RAM  0x00000000

/* ========================================================================= */
/* 12. CPPC Extension (EID #0x43505043 "CPPC")                               */
/* ========================================================================= */

#define SBI_CPPC_PROBE                      0
#define SBI_CPPC_READ                       1
#define SBI_CPPC_READ_HI                    2
#define SBI_CPPC_WRITE                      3

/* CPPC Registers (Table 58) */
#define SBI_CPPC_HIGHEST_PERF               0x00000000
#define SBI_CPPC_NOMINAL_PERF               0x00000001
#define SBI_CPPC_LOW_NON_LINEAR_PERF        0x00000002
#define SBI_CPPC_LOWEST_PERF                0x00000003
#define SBI_CPPC_GUARANTEED_PERF            0x00000004
#define SBI_CPPC_DESIRED_PERF               0x00000005
#define SBI_CPPC_MIN_PERF                   0x00000006
#define SBI_CPPC_MAX_PERF                   0x00000007
#define SBI_CPPC_PERF_REDUC_TOLERANCE       0x00000008
#define SBI_CPPC_TIME_WINDOW                0x00000009
#define SBI_CPPC_CTR_WRAP_TIME              0x0000000A
#define SBI_CPPC_REF_PERF_CTR               0x0000000B
#define SBI_CPPC_DELIVERED_PERF_CTR         0x0000000C
#define SBI_CPPC_PERF_LIMITED               0x0000000D
#define SBI_CPPC_ENABLE                     0x0000000E
#define SBI_CPPC_AUTO_SEL_ENABLE            0x0000000F
#define SBI_CPPC_AUTO_ACT_WINDOW            0x00000010
#define SBI_CPPC_ENERGY_PERF_PREFERENCE     0x00000011
#define SBI_CPPC_REF_PERF                   0x00000012
#define SBI_CPPC_LOWEST_FREQ                0x00000013
#define SBI_CPPC_NOMINAL_FREQ               0x00000014

/* ========================================================================= */
/* 13. Nested Acceleration Extension (EID #0x4E41434C "NACL")                */
/* ========================================================================= */

#define SBI_NACL_PROBE_FEATURE              0
#define SBI_NACL_SET_SHMEM                  1
#define SBI_NACL_SYNC_CSR                   2
#define SBI_NACL_SYNC_HFENCE                3
#define SBI_NACL_SYNC_SRET                  4

/* NACL Feature IDs (Table 64) */
#define SBI_NACL_FEAT_SYNC_CSR              0
#define SBI_NACL_FEAT_SYNC_HFENCE           1
#define SBI_NACL_FEAT_SYNC_SRET             2
#define SBI_NACL_FEAT_AUTOSWAP_CSR          3

/* NACL HFENCE Entry Format (Table 67) */
struct sbi_nacl_hfence_entry {
	unsigned long paddr;
	unsigned long size;
	unsigned long vmid;
	unsigned long asid;
	unsigned long type;
};

/* NACL HFENCE Types (Table 68) */
#define SBI_NACL_HFENCE_TYPE_GVMA_ALL       0
#define SBI_NACL_HFENCE_TYPE_GVMA_VMID      1
#define SBI_NACL_HFENCE_TYPE_VVMA_ALL       2
#define SBI_NACL_HFENCE_TYPE_VVMA_ASID      3

/* ========================================================================= */
/* 14. Steal-time Accounting Extension (EID #0x535441 "STA")                 */
/* ========================================================================= */

#define SBI_STA_SET_SHMEM                   0

/* STA Shared Memory Structure (Table 76) */
struct sbi_sta_shmem {
	uint32_t sequence;
	uint32_t flags;
	uint64_t steal;
	uint8_t preempted;
	uint8_t pad[47];
};

/* ========================================================================= */
/* 15. Supervisor Software Events Extension (EID #0x535345 "SSE")            */
/* ========================================================================= */

#define SBI_SSE_READ_ATTRS                  0
#define SBI_SSE_WRITE_ATTRS                 1
#define SBI_SSE_REGISTER                    2
#define SBI_SSE_UNREGISTER                  3
#define SBI_SSE_ENABLE                      4
#define SBI_SSE_DISABLE                     5
#define SBI_SSE_COMPLETE                    6
#define SBI_SSE_INJECT                      7
#define SBI_SSE_HART_UNMASK                 8
#define SBI_SSE_HART_MASK                   9

/* SSE Event ID Space (Table 79) */
#define SBI_SSE_EVENT_LOCAL_HIGH_PRIO_RAS   0x00000000
#define SBI_SSE_EVENT_LOCAL_DOUBLE_TRAP     0x00000001
#define SBI_SSE_EVENT_GLOBAL_HIGH_PRIO_RAS  0x00008000
#define SBI_SSE_EVENT_LOCAL_PMU_OVERFLOW    0x00010000
#define SBI_SSE_EVENT_LOCAL_LOW_PRIO_RAS    0x00100000
#define SBI_SSE_EVENT_GLOBAL_LOW_PRIO_RAS   0x00108000
#define SBI_SSE_EVENT_LOCAL_SW_INJECTED     0xFFFF0000
#define SBI_SSE_EVENT_GLOBAL_SW_INJECTED    0xFFFF8000

/* SSE Event States (Section 17.2) */
#define SBI_SSE_STATE_UNUSED                0
#define SBI_SSE_STATE_REGISTERED            1
#define SBI_SSE_STATE_ENABLED               2
#define SBI_SSE_STATE_RUNNING               3

/* SSE Event Attributes (Table 80) */
#define SBI_SSE_ATTR_STATUS                 0x00000000
#define SBI_SSE_ATTR_PRIORITY               0x00000001
#define SBI_SSE_ATTR_CONFIG                 0x00000002
#define SBI_SSE_ATTR_PREFERRED_HART         0x00000003
#define SBI_SSE_ATTR_ENTRY_PC               0x00000004
#define SBI_SSE_ATTR_ENTRY_ARG              0x00000005
#define SBI_SSE_ATTR_INTERRUPTED_SEPC       0x00000006
#define SBI_SSE_ATTR_INTERRUPTED_FLAGS      0x00000007
#define SBI_SSE_ATTR_INTERRUPTED_A6         0x00000008
#define SBI_SSE_ATTR_INTERRUPTED_A7         0x00000009

/* SSE Status Attribute bit definitions */
#define SBI_SSE_ATTR_STATUS_STATE_MASK      0x3UL
#define SBI_SSE_ATTR_STATUS_PENDING_SHIFT   2
#define SBI_SSE_ATTR_STATUS_PENDING         (1UL << 2)
#define SBI_SSE_ATTR_STATUS_INJECT_SHIFT    3
#define SBI_SSE_ATTR_STATUS_INJECT          (1UL << 3)

/* SSE Config Attribute bit definitions */
#define SBI_SSE_ATTR_CONFIG_ONESHOT         (1UL << 0)

/* ========================================================================= */
/* 16. SBI Firmware Features Extension (EID #0x46574654 "FWFT")              */
/* ========================================================================= */

#define SBI_FWFT_SET                        0
#define SBI_FWFT_GET                        1

/* Firmware Feature IDs (Table 92) */
#define SBI_FWFT_MISALIGNED_EXC_DELEG       0x00000000
#define SBI_FWFT_LANDING_PAD                0x00000001
#define SBI_FWFT_SHADOW_STACK               0x00000002
#define SBI_FWFT_DOUBLE_TRAP                0x00000003
#define SBI_FWFT_PTE_AD_HW_UPDATING         0x00000004
#define SBI_FWFT_POINTER_MASKING_PMLEN      0x00000005

/* Firmware Feature Set Flags (Table 93) */
#define SBI_FWFT_SET_FLAG_LOCK              (1UL << 0)

/* ========================================================================= */
/* 17. Debug Triggers Extension (EID #0x44425452 "DBTR")                      */
/* ========================================================================= */

#define SBI_DBTR_NUM_TRIGGERS               0
#define SBI_DBTR_SET_SHMEM                  1
#define SBI_DBTR_READ_TRIGGERS              2
#define SBI_DBTR_INSTALL_TRIGGERS           3
#define SBI_DBTR_UPDATE_TRIGGERS            4
#define SBI_DBTR_UNINSTALL_TRIGGERS         5
#define SBI_DBTR_ENABLE_TRIGGERS            6
#define SBI_DBTR_DISABLE_TRIGGERS           7

/* DBTR Shared Memory Entry (Table 100) */
struct sbi_dbtr_shmem_entry {
	unsigned long tdata1;
	unsigned long tdata2;
	unsigned long tdata3;
};

/* ========================================================================= */
/* 18. Message Proxy Extension (EID #0x4D505859 "MPXY")                      */
/* ========================================================================= */

#define SBI_MPXY_GET_SHMEM_SIZE             0
#define SBI_MPXY_SET_SHMEM                  1
#define SBI_MPXY_GET_CHANNEL_IDS            2
#define SBI_MPXY_READ_ATTRIBUTES            3
#define SBI_MPXY_WRITE_ATTRIBUTES           4
#define SBI_MPXY_SEND_MSG_WITH_RESP         5
#define SBI_MPXY_SEND_MSG_WITHOUT_RESP      6
#define SBI_MPXY_GET_NOTIFICATION_EVENTS    7

/* MPXY Channel Attributes (Table 109) */
#define SBI_MPXY_ATTR_MSG_PROT_ID           0x00000000
#define SBI_MPXY_ATTR_MSG_PROT_VER          0x00000001
#define SBI_MPXY_ATTR_MSG_MAX_LEN           0x00000002
#define SBI_MPXY_ATTR_MSG_SEND_TIMEOUT      0x00000003
#define SBI_MPXY_ATTR_MSG_COMPL_TIMEOUT     0x00000004
#define SBI_MPXY_ATTR_CHANNEL_CAPABILITY    0x00000005
#define SBI_MPXY_ATTR_SSE_EVENT_ID          0x00000006
#define SBI_MPXY_ATTR_MSI_CONTROL           0x00000007
#define SBI_MPXY_ATTR_MSI_ADDR_LOW          0x00000008
#define SBI_MPXY_ATTR_MSI_ADDR_HIGH         0x00000009
#define SBI_MPXY_ATTR_MSI_DATA              0x0000000A
#define SBI_MPXY_ATTR_EVENTS_STATE_CONTROL  0x0000000B

/* MPXY Message Protocols (Table 110) */
#define SBI_MPXY_PROT_RPMI                  0x00000000

/* MPXY Set Shmem Flags */
#define SBI_MPXY_SHMEM_FLAG_OVERWRITE       0x0
#define SBI_MPXY_SHMEM_FLAG_RETURN          0x1

/* ========================================================================= */
/* 19. Low-level SBI Call Interface                                          */
/* ========================================================================= */

struct sbiret sbi_ecall(long eid, long fid,
                        unsigned long arg0, unsigned long arg1,
                        unsigned long arg2, unsigned long arg3,
                        unsigned long arg4, unsigned long arg5);

long sbi_ecall_legacy(long eid, unsigned long arg0,
                      unsigned long arg1, unsigned long arg2);

/* ========================================================================= */
/* 20. High-level SBI Function Declarations                                   */
/* ========================================================================= */

/* Base Extension */
struct sbiret sbi_get_spec_version(void);
struct sbiret sbi_get_impl_id(void);
struct sbiret sbi_get_impl_version(void);
struct sbiret sbi_probe_extension(long extension_id);
struct sbiret sbi_get_mvendorid(void);
struct sbiret sbi_get_marchid(void);
struct sbiret sbi_get_mimpid(void);

/* Legacy Extensions */
long sbi_set_timer_legacy(uint64_t stime_value);
long sbi_console_putchar(int ch);
long sbi_console_getchar(void);
long sbi_clear_ipi(void);
long sbi_send_ipi_legacy(const unsigned long *hart_mask);
long sbi_remote_fence_i_legacy(const unsigned long *hart_mask);
long sbi_remote_sfence_vma_legacy(const unsigned long *hart_mask,
                                 unsigned long start, unsigned long size);
long sbi_remote_sfence_vma_asid_legacy(const unsigned long *hart_mask,
                                      unsigned long start, unsigned long size,
                                      unsigned long asid);
long sbi_shutdown(void);

/* Timer Extension */
struct sbiret sbi_set_timer(uint64_t stime_value);

/* IPI Extension */
struct sbiret sbi_send_ipi(unsigned long hart_mask, unsigned long hart_mask_base);

/* RFENCE Extension */
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

/* Hart State Management Extension */
struct sbiret sbi_hart_start(unsigned long hartid, unsigned long start_addr,
                             unsigned long opaque);
struct sbiret sbi_hart_stop(void);
struct sbiret sbi_hart_get_status(unsigned long hartid);
struct sbiret sbi_hart_suspend(uint32_t suspend_type, unsigned long resume_addr,
                               unsigned long opaque);

/* System Reset Extension */
struct sbiret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason);

/* Performance Monitoring Unit Extension */
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

/* Debug Console Extension */
struct sbiret sbi_debug_console_write(unsigned long num_bytes,
                                      unsigned long base_addr_lo,
                                      unsigned long base_addr_hi);
struct sbiret sbi_debug_console_read(unsigned long num_bytes,
                                     unsigned long base_addr_lo,
                                     unsigned long base_addr_hi);
struct sbiret sbi_debug_console_write_byte(uint8_t byte);

/* System Suspend Extension */
struct sbiret sbi_system_suspend(uint32_t sleep_type, unsigned long resume_addr,
                                 unsigned long opaque);

/* CPPC Extension */
struct sbiret sbi_cppc_probe(uint32_t cppc_reg_id);
struct sbiret sbi_cppc_read(uint32_t cppc_reg_id);
struct sbiret sbi_cppc_read_hi(uint32_t cppc_reg_id);
struct sbiret sbi_cppc_write(uint32_t cppc_reg_id, uint64_t val);

/* Nested Acceleration Extension */
struct sbiret sbi_nacl_probe_feature(uint32_t feature_id);
struct sbiret sbi_nacl_set_shmem(unsigned long shmem_phys_lo,
                                 unsigned long shmem_phys_hi,
                                 unsigned long flags);
struct sbiret sbi_nacl_sync_csr(unsigned long csr_num);
struct sbiret sbi_nacl_sync_hfence(unsigned long entry_index);
struct sbiret sbi_nacl_sync_sret(void);

/* Steal-time Accounting Extension */
struct sbiret sbi_steal_time_set_shmem(unsigned long shmem_phys_lo,
                                       unsigned long shmem_phys_hi,
                                       unsigned long flags);

/* Supervisor Software Events Extension */
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

/* SBI Firmware Features Extension */
struct sbiret sbi_fwft_set(uint32_t feature, unsigned long value,
                           unsigned long flags);
struct sbiret sbi_fwft_get(uint32_t feature);

/* Debug Triggers Extension */
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

/* Message Proxy Extension */
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
struct sbiret sbi_mpxy_send_message_with_response(uint32_t channel_id,
                                                  uint32_t message_id,
                                                  unsigned long message_data_len);
struct sbiret sbi_mpxy_send_message_without_response(uint32_t channel_id,
                                                     uint32_t message_id,
                                                     unsigned long message_data_len);
struct sbiret sbi_mpxy_get_notification_events(uint32_t channel_id);

/* ========================================================================= */
/* 21. SBI Driver Initialization and Query Interface                         */
/* ========================================================================= */

void sbi_init(void);
bool sbi_probe(long extension_id);
long sbi_spec_version(void);
long sbi_impl_id(void);
long sbi_impl_version(void);
const char *sbi_impl_name(long impl_id);
const char *sbi_strerror(long error);

/* Extension Availability Helpers */
bool sbi_has_time(void);
bool sbi_has_ipi(void);
bool sbi_has_rfence(void);
bool sbi_has_hsm(void);
bool sbi_has_srst(void);
bool sbi_has_pmu(void);
bool sbi_has_dbcn(void);
bool sbi_has_susp(void);
bool sbi_has_cppc(void);
bool sbi_has_nacl(void);
bool sbi_has_sta(void);
bool sbi_has_sse(void);
bool sbi_has_fwft(void);
bool sbi_has_dbtr(void);
bool sbi_has_mpxy(void);

#ifdef __cplusplus
}
#endif
