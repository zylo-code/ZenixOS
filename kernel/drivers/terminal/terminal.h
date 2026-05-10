#ifndef TERMINAL_H
#define TERMINAL_H

#include "../include/stdint.h"
#include "../include/stddef.h"
#include "../include/stdbool.h"
#include "../../../boot/limine/limine.h"

void terminal_init(struct limine_framebuffer *fb);
void terminal_putchar(char c);
void terminal_write(const char *str);
void terminal_writeln(const char *str);
void terminal_clear(void);
void terminal_set_color(uint32_t fg, uint32_t bg);
void terminal_set_cursor_pos(size_t x, size_t y);

#endif