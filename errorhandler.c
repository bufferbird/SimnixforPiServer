#include "errorhandler.h"
#include <stdint.h>

volatile char reason = 'x'; 

void c_sync_error(void){
    kprintf("Can't divide by Zero"); 
    asm("wfe"); 
    reason = 'A'; 
    panic(); 
}

void print_stack_trace() {
    unsigned long *fp;
    /*
        We get the fp from now
    */
    asm volatile("mov %0, x29" : "=r"(fp));

    kprintf("Stack Trace:");
    while(fp != 0) {
        unsigned long return_addr = *(fp + 1);
        kprintf("  [<%p>]", return_addr);
        
        // Next frame...
        
        fp = (unsigned long *)(*fp);
        
        // Security Stop, so it finds an end
        if (return_addr == 0) break;
    }
}

void c_system_error(void){
    kprintf("An Error occured. ");
    kprintf("<KernelPanic>"); 
    reason = 'S'; 
    panic();
}

void errorpanic(){
    void* sp;

    // the fight against intellisense
    #ifndef __INTELLISENSE__
    // GCC sees this, and compliles it
    asm volatile("mov %%rsp, %0" : "=r"(sp));
    #else
        // VS Code sees this and dont come with his error squiggles
        sp = 0; 
    #endif
    print_stack_trace(); 

}



void panic(void){
    switch (reason)
    {
    case 'K':
        kprintf("Kernel Panic - Kernel Error");
        break;
    case 'S':
        kprintf("Kernel Panic - Sys Error");
        break;
    case 'U':
        kprintf("Kernel Panic - User Error");
        break;
    case 'A':
        kprintf("Kernel Panic - Arithmetical Error");
        break; 
    case 'F':
        kprintf("Kernel Panic - Fatal Exception");
        break;
    default:
        kprintf("Kernel Panic - Unknown Error");
        break;
    }
}
