/* kernel.ld */

ENTRY(init)

SECTIONS {
    . = 0xffffffff80000000;

    __kernel_start = .;

    __text_start = .;
    .text : {
        *(.text .text.*)
    }
    . = ALIGN(0x1000);
    __text_end = .;

    __rodata_start = .;
    .rodata : {
        *(.rodata .rodata.*)
    }
    . = ALIGN(0x1000);
    __rodata_end = .;

    __data_start = .;
    .data : {
        *(.data .data.*)
    }

    . = ALIGN(0x1000);
    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_base_revision))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    }
    . = ALIGN(0x1000);
    __data_end = .;

    __bss_start = .;
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    }
    . = ALIGN(0x1000);
    __bss_end = .;

    __kernel_end = .;
}
