#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>
typedef volatile uint32_t reg32;

/* --- Basis-Adressen (Pi 3 / BCM2837) --- */
#define PERIPHERAL_BASE 0x3F000000
#define GPIO_BASE       (PERIPHERAL_BASE + 0x200000)
#define UART0_BASE      (PERIPHERAL_BASE + 0x201000)
#define TIMER_BASE      (PERIPHERAL_BASE + 0x003000)
#define IRQ_BASE        (PERIPHERAL_BASE + 0x00B200)

/* --- GPIO Register  --- */
#define GPFSEL0         ((reg32*)(GPIO_BASE + 0x00))
#define GPFSEL1         ((reg32*)(GPIO_BASE + 0x04))
#define GPPUD           ((reg32*)(GPIO_BASE + 0x94)) // Pull-up/down Steuerung
#define GPPUDCLK0       ((reg32*)(GPIO_BASE + 0x98)) // Pull-up/down Clock

/* --- UART0 (PL011) Register --- */
#define UART0_DR        ((reg32*)(UART0_BASE + 0x00)) 
#define UART0_FR        ((reg32*)(UART0_BASE + 0x18)) 
#define UART0_IBRD       ((reg32*)(UART0_BASE + 0x24)) 
#define UART0_FBRD       ((reg32*)(UART0_BASE + 0x28)) 
#define UART0_LCRH       ((reg32*)(UART0_BASE + 0x2C)) 
#define UART0_CR         ((reg32*)(UART0_BASE + 0x30)) 
#define UART0_ICR        ((reg32*)(UART0_BASE + 0x44)) 

#define GPSET0           ((reg32*)(GPIO_BASE + 0x1c))
#define GPCLR0           ((reg32*)(GPIO_BASE + 0x28))

/* --- System Timer --- */
#define TIMER_CS        ((reg32*)(TIMER_BASE + 0x00)) // Control/Status
#define TIMER_CLO       ((reg32*)(TIMER_BASE + 0x04)) // Counter Low (32-bit)
#define TIMER_CHI       ((reg32*)(TIMER_BASE + 0x08)) // Counter High
#define TIMER_C0        ((reg32*)(TIMER_BASE + 0x0C)) // Compare Register 0
#define TIMER_C1        ((reg32*)(TIMER_BASE + 0x10)) // Compare Register 1
#define TIMER_C2        ((reg32*)(TIMER_BASE + 0x14)) // Compare Register 2
#define TIMER_C3        ((reg32*)(TIMER_BASE + 0x18)) // Compare Register 3

/* --- Interrupt Controller --- */
#define IRQ_BASIC_PENDING   ((reg32*)(IRQ_BASE + 0x00))
#define IRQ_PENDING_1       ((reg32*)(IRQ_BASE + 0x04))
#define IRQ_ENABLE_1        ((reg32*)(IRQ_BASE + 0x10))
#define IRQ_DISABLE_1       ((reg32*)(IRQ_BASE + 0x1C))

#define UART_TX_FIFO_FULL  (1 << 5)
#define UART_RX_FIFO_EMPTY (1 << 4)
#define UART_BUSY          (1 << 3)

#define SYS_TIMER_IRQ_1    (1 << 1)
#define SYS_TIMER_IRQ_3    (1 << 3)

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_RESET   "\x1b[0m"
#define ANSI_BOLD    "\x1b[1m"

#define MBOX_BASE    (PERIPHERAL_BASE + 0x00B880)
#define MBOX_READ    ((reg32*)(MBOX_BASE + 0x00))
#define MBOX_STATUS  ((reg32*)(MBOX_BASE + 0x18))
#define MBOX_WRITE   ((reg32*)(MBOX_BASE + 0x20))

#define MBOX_FULL    0x80000000
#define MBOX_EMPTY   0x40000000
#define MBOX_CH_PROP 8  /* Channel 8: Property Tags */


#endif
