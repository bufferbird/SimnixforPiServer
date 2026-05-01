#ifndef KSTDIO_H
#define KSTDIO_H

#include <stdint.h>


void kputc(char c);
void kprint(const char* s);
void kprintf(const char* format, ...); 

void kprint_hex(uint32_t d);
void kset_cursor(uint32_t x, uint32_t y);
void kclear_screen(uint32_t color);

#endif
