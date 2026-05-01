#include "hardwarepi3.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdalign.h>
#include "kprint.h"

// Die Mailbox MUSS außerhalb der Funktion liegen (nicht auf dem Stack) 
// und 16-Byte aligned sein.
static alignas(16) uint32_t mailbox[36];

uint32_t* fb_ptr = 0;

static void vga_init() {
    // Mailbox-Buffer vorbereiten
    mailbox[0] = 35 * 4;        
    mailbox[1] = 0;            

    mailbox[2] = 0x00048003;    // Tag: Set Physical Width/Height
    mailbox[3] = 8;             // Value Buffer Größe
    mailbox[4] = 8;             // Respons/Request Code
    mailbox[5] = 1024;          // Width
    mailbox[6] = 768;           // Height

    mailbox[7] = 0x00048004;    // Tag: Set Virtual Width/Height
    mailbox[8] = 8;
    mailbox[9] = 8;
    mailbox[10] = 1024;
    mailbox[11] = 768;

    mailbox[12] = 0x00048005;   // Tag: Set Depth
    mailbox[13] = 4;
    mailbox[14] = 4;
    mailbox[15] = 32;           // 32-bit Farben (RGBA)

    mailbox[16] = 0x00040001;   // Tag: Allocate Framebuffer
    mailbox[17] = 8;
    mailbox[18] = 8;
    mailbox[19] = 4096;         // Alignment (Request)
    mailbox[20] = 0;            // Framebuffer Adresse (Response)

    mailbox[21] = 0;            // End Tag
    while (*(volatile uint32_t*)MBOX_STATUS & 0x80000000);
    *(volatile uint32_t*)MBOX_WRITE = ((uint32_t)(uintptr_t)mailbox & ~0xF) | 8;


    while (1) {
        while (*(volatile uint32_t*)MBOX_STATUS & 0x40000000); 
        uint32_t response = *(volatile uint32_t*)MBOX_READ;
        if ((response & 0xF) == 8) break;
    }


    if (mailbox[1] == 0x80000000 && mailbox[20] != 0) {
        fb_ptr = (uint32_t*)(uintptr_t)(mailbox[20] & 0x3FFFFFFF);
    } else {
        fb_ptr = 0; 
    }
}

static void __initscreen__(){
    if (!fb_ptr) return; 
    
    kclear_screen(0x00008B);
    kprint("Kernel - Version 1.0.0, May 2026.");
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

    *UART0_ICR = 0x7FF; 
    *UART0_IBRD = 26;   
    *UART0_FBRD = 3;
    *UART0_LCRH = (3 << 5);
    *UART0_CR = 0x301;  

void k_main(void){
    uart_init(); 
    
   
    *((volatile uint32_t*)0x3F201000) = 'O';
    *((volatile uint32_t*)0x3F201000) = 'K';
    *((volatile uint32_t*)0x3F201000) = '\n';

    vga_init(); 

    if (fb_ptr != 0) {
        __initscreen__();
        kprintf(ANSI_GREEN "FB loaded at 0x%x\n", (uint32_t)(uintptr_t)fb_ptr);
    } else {
        *((volatile uint32_t*)0x3F201000) = 'F';
        *((volatile uint32_t*)0x3F201000) = 'B';
        *((volatile uint32_t*)0x3F201000) = '!';
    }

    kprint("[OK] Kernel is running.");

    while (1){
        __asm__("wfe");
    }
}
