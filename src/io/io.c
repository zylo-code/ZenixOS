#include "io.h"
#include <stdint.h>

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outb(unsigned short port, unsigned char value) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(value), "Nd"(port));
}