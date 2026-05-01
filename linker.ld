ENTRY(_start)

SECTIONS
{
    . = 0x80000;

  
    .init : {
        __init_start = .;
        KEEP(*(.text.boot))   
        *(.init.text)         
        *(.init.data)        
        . = ALIGN(8);
        __init_end = .;      
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
        __bss_end = .;
    }
}
