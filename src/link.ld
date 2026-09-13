/* kernel.ld */

ENTRY(init)

SECTIONS {
    . = 0xffffffff80000000;

    .text : {
        *(.text .text.*)
    }

    . = ALIGN(0x1000);
    .rodata : {
        *(.rodata .rodata.*)
    }

    . = ALIGN(0x1000);
    .data : {
        *(.data .data.*)
    }

    . = ALIGN(0x1000);
    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    }

    . = ALIGN(0x1000);
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    }
}
