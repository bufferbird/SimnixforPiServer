#include "hardwarepi3.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include "kprint.h"


uint32_t* fb_ptr = 0;


static void vga_init(){
    /* Yes. we have to do this. */

    uint32_t mailbox[32];
    mailbox[0] = 35 * 4;  
    mailbox[1] = 0;        

    mailbox[2] = 0x00048003; 
    mailbox[3] = 8;
    mailbox[4] = 8;
    mailbox[5] = 1024; // width
    mailbox[6] = 768;  // height

    mailbox[7] = 0x00048004;
    mailbox[8] = 8;
    mailbox[9] = 8;
    mailbox[10] = 1024;
    mailbox[11] = 768;

    mailbox[12] = 0x00048005;
    mailbox[13] = 4;
    mailbox[14] = 4;
    mailbox[15] = 32;

    mailbox[16] = 0x00040001;
    mailbox[17] = 8;
    mailbox[18] = 8;
    mailbox[19] = 4096;
    mailbox[20] = 0; 

    mailbox[21] = 0; 

    // Send
    while (*(volatile uint32_t*)MBOX_STATUS & 0x80000000);
    *(volatile uint32_t*)MBOX_WRITE = ((uint32_t)(uintptr_t) mailbox & ~0xF) | 8;

    // Wait
    while (1) {
        while (*(volatile uint32_t*)MBOX_STATUS & 0x40000000);
        uint32_t response = *(volatile uint32_t*)MBOX_READ;
        if ((response & 0xF) == 8) break;
    }


    fb_ptr = (uint32_t*)(uintptr_t)(mailbox[20] & 0x3FFFFFFF);
}


static void __initscreen__(){
  kclear_screen(0x00000000);
  kprint("Kernel - ");
  kprint("Simnix - Simnix v3.6");
  kprint("Ready for input...");
  kprint("Server for TiChat."); 
  kprintf("~$>>"); 

}



void uart_init() {
    *UART0_CR = 0; 

  
    *GPFSEL1 &= ~((7 << 12) | (7 << 15));
    *GPFSEL1 |= (4 << 12) | (4 << 15);    

    *GPPUD = 0; 
    for(int i=0; i<150; i++) __asm__("nop");
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    for(int i=0; i<150; i++) __asm__("nop");
    *GPPUDCLK0 = 0;

    *UART0_ICR = 0x7FF; // delete all interrupts
    *UART0_IBRD = 26;   
    *UART0_FR = 3;
    *UART0_LCRH = (3 << 5); 
    *UART0_CR = 0x301;  
}


void k_main(void){
  uart_init(); 
  *UART0_DR = 'T';
  *UART0_DR = 'E';
  *UART0_DR = 'S';
  *UART0_DR = 'T';
  *UART0_DR = '\n';
  vga_init(); 
  
  if (fb_ptr != NULL){
    __initscreen__(); 
    kprintf(ANSI_GREEN "Loading Framebuffer at 0x%x", MBOX_BASE); 
    kprint(ANSI_GREEN "[OK] Successfully loaded Framebuffer.");
  }
  else {
    kprint("[fb_ptr] Framebuffer failed."); 
    __asm__("wfe");
  }
  kprintf("Loading UART at 0x%x",UART0_BASE); 
  kprint("[OK] Loaded UART successfully.");
  
  while (1){
    __asm__("wfe");
  }
}
