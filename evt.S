.global handle_sync_error

// evt.S - bufferbird
// let the fight begin...

.align 11
exception_vector_table:
    .align 7
    b handle_sync_err
    .align 7
    b handle_interrupt
    .align 7
    b handle_fast_interrupt
    .align 7
    b handle_system_error
    .align 7
    b handle_currentel_sync_error
    .align 7
    b handle_currentel_interrupt
    .align 7
    b handle_currentel_fast_interrupt
    .align 7
    b handle_currentel_system_error
    .align 7
    b handle_userel_sync_error
    .align 7
    b handle_userel_interrupt
    .align 7
    b handle_userel_fast_interrupt
    .align 7
    b handle_userel_system_error

.macro save_all_regs
    stp x0,  x1,  [sp, #-16]!
    stp x2,  x3,  [sp, #-16]!
    stp x4,  x5,  [sp, #-16]!
    stp x6,  x7,  [sp, #-16]!
    stp x8,  x9,  [sp, #-16]!
    stp x10, x11, [sp, #-16]!
    stp x12, x13, [sp, #-16]!
    stp x14, x15, [sp, #-16]!
    stp x16, x17, [sp, #-16]!
    stp x18, x19, [sp, #-16]!
    stp x20, x21, [sp, #-16]!
    stp x22, x23, [sp, #-16]!
    stp x24, x25, [sp, #-16]!
    stp x26, x27, [sp, #-16]!
    stp x28, x29, [sp, #-16]!
    str x30,      [sp, #-16]!  
.endm

.macro restore_all_regs
    ldp x0,  x1,  [sp, #0]
    ldp x2,  x3,  [sp, #16]
    ldp x4,  x5,  [sp, #32]
    ldp x6,  x7,  [sp, #48]
    ldp x8,  x9,  [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #208]
    ldp x28, x29, [sp, #224]
    ldr x30,      [sp, #240]
    add sp, sp, #256         // clean stack
    eret                     
.endm


handle_sync_error: 
    save_all_regs
    b c_sync_err
    restore_all_regs
