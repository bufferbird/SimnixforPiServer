/*
  Copyright (c) by bufferbird
  License: GPL, see: github.com/bufferbird/Simnix/LICENSE
*/

.section ".text.boot"
.global _start

_start:
    mrs     x1, mpidr_el1
    and     x1, x1, #3          
    cbz     x1, master_core    

park_core:
    wfe
    b       park_core

master_core:
    ldr     x1, =_stack_top
    mov     sp, x1

    ldr     x1, =__bss_start    
    ldr     x2, =__bss_end

bss_loop:
    cmp     x1, x2              
    b.ge    jump_to_main        
    str     xzr, [x1], #8     
    b       bss_loop

jump_to_main:
    bl      k_main


    b       park_core
