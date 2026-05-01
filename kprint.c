#include "kprint.h"
#include "font.h"
#include <stdint.h>
#include <stdarg.h>

extern uint32_t* fb_ptr;
static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 768

void kclear_screen(uint32_t color) {
    for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        fb_ptr[i] = color;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
        fb_ptr[y * SCREEN_WIDTH + x] = color;
    }
}

void kputc(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y += 8;
    } else if (c == '\r') {
        cursor_x = 0;
    } else {
        unsigned char *glyph = (unsigned char*)font8x8_basic[(unsigned char)c];

        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (glyph[y] & (1 << x)) {
                    draw_pixel(cursor_x + x, cursor_y + y, 0xFFFFFFFF); // Weiß
                } else {
                    draw_pixel(cursor_x + x, cursor_y + y, 0x00000000); // Schwarz
                }
            }
        }
        cursor_x += 8;
    }

    if (cursor_x >= SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y += 8;
    }
    

    if (cursor_y >= SCREEN_HEIGHT) {
        cursor_y = 0;
    }
}

void kprint(const char* s) {
    while (*s) {
        kputc(*s++);
    }
}

void kprint_hex(uint32_t d) {
    char hex_chars[] = "0123456789ABCDEF";
    kprint("0x");
    for (int i = 28; i >= 0; i -= 4) {
        kputc(hex_chars[(d >> i) & 0xF]);
    }
}





extern void kputc(char c);

void kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char* s;
    int n;
    uint32_t val;
    int nibble;
    char buf[1024];
    int i;
    const char* p;

    for (p = fmt; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 's':
                    s = va_arg(args, char*);
                    if (s == 0) s = "(null)";
                    while (*s) kputc(*s++);
                    break;

                case 'c':
                    kputc((char)va_arg(args, int));
                    break;

                case 'd':
                    n = va_arg(args, int);
                    if (n == 0) {
                        kputc('0');
                    } else {
                        if (n < 0) {
                            kputc('-');
                            n = -n;
                        }
                        i = 0;
                        while (n > 0) {
                            buf[i++] = (n % 10) + '0';
                            n /= 10;
                        }
                        while (i--) kputc(buf[i]);
                    }
                    break;

                case 'x':
                    val = va_arg(args, uint32_t);
                    for (i = 28; i >= 0; i -= 4) {
                        nibble = (val >> i) & 0xF;
                        kputc(nibble < 10 ? nibble + '0' : nibble - 10 + 'A');
                    }
                    break;

                case '%':
                    kputc('%');
                    break;

                default:
                    kputc('?');
                    break;
            }
        } else {
            kputc(*p);
        }
    }
    va_end(args);
}

