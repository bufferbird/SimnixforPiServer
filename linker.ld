ENTRY(_start)

SECTIONS
{
    . = 0x80000;
    _start_limit = .;

    .init : {
        KEEP(*(.text.boot))
        *(.init.text)
        *(.init.data)
        . = ALIGN(8);
    }

    .text : {
        *(.text)
        . = ALIGN(8);
    }

    .rodata : {
        *(.rodata)
        . = ALIGN(8);
    }

    .data : {
        *(.data)
        . = ALIGN(8);
    }

    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(8); 
        __bss_end = .;
    }

    . = ALIGN(16);
    . += 0x100000; 
    _stack_top = .;
}
