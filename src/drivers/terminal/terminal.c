#include "terminal.h"
#include "../include/font.h"

static struct {
    struct limine_framebuffer *fb;
    uint32_t *buffer;
    size_t width;
    size_t height;
    size_t pitch;
    uint32_t fg_color;
    uint32_t bg_color;
    size_t cursor_x;
    size_t cursor_y;
    size_t cols;
    size_t rows;
} term;

static void draw_char(char c, size_t x, size_t y, uint32_t fg, uint32_t bg) {
    if (c < 32 || c > 127) c = '?';

    for (size_t row = 0; row < 16; row++) {
        uint8_t line = font8x16[(uint8_t)c][row];
        for (size_t col = 0; col < 8; col++) {
            if (line & (0x80 >> col)) {
                size_t px_x = x * 8 + col;
                size_t px_y = y * 16 + row;
                if (px_x < term.width && px_y < term.height) {
                    term.buffer[px_y * (term.pitch / 4) + px_x] = fg;
                }
            } else {
                size_t px_x = x * 8 + col;
                size_t px_y = y * 16 + row;
                if (px_x < term.width && px_y < term.height) {
                    term.buffer[px_y * (term.pitch / 4) + px_x] = bg;
                }
            }
        }
    }
}

static void terminal_scroll(void) {
    for (size_t y = 1; y < term.rows; y++) {
        for (size_t x = 0; x < term.cols; x++) {

        }
    }

    for (size_t x = 0; x < term.cols; x++) {
        for (size_t row = 0; row < 16; row++) {
            for (size_t col = 0; col < 8; col++) {
                size_t px_x = x * 8 + col;
                size_t px_y = (term.rows - 1) * 16 + row;
                if (px_x < term.width && px_y < term.height) {
                    term.buffer[px_y * (term.pitch / 4) + px_x] = term.bg_color;
                }
            }
        }
    }

    term.cursor_y = term.rows - 1;
}

void terminal_init(struct limine_framebuffer *fb) {
    term.fb = fb;
    term.buffer = fb->address;
    term.width = fb->width;
    term.height = fb->height;
    term.pitch = fb->pitch;
    term.fg_color = 0xFFFFFF;
    term.bg_color = 0x000000;
    term.cols = term.width / 8;
    term.rows = term.height / 16;
    term.cursor_x = 0;
    term.cursor_y = 0;

    terminal_clear();
}

void terminal_clear(void) {
    for (size_t y = 0; y < term.height; y++) {
        for (size_t x = 0; x < term.width; x++) {
            term.buffer[y * (term.pitch / 4) + x] = term.bg_color;
        }
    }

    term.cursor_x = 0;
    term.cursor_y = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        term.cursor_x = 0;
        term.cursor_y++;
    } else if (c == '\r') {
        term.cursor_x = 0;
    } else if (c == '\t') {
        term.cursor_x = (term.cursor_x + 4) & -3;
    } else if (c == '\b') {
        if (term.cursor_x > 0) {
            term.cursor_x--;
            draw_char(' ', term.cursor_x, term.cursor_y, term.fg_color, term.bg_color);
        }
    } else {
        draw_char(c, term.cursor_x, term.cursor_y, term.fg_color, term.bg_color);
        term.cursor_x++;
    }

    if (term.cursor_x >= term.cols) {
        term.cursor_x = 0;
        term.cursor_y++;
    }

    if (term.cursor_y >= term.rows) {
        terminal_scroll();
    }
}

void terminal_write(const char *str) {
    while (*str) {
        terminal_putchar(*str++);
    }
}

void terminal_writeln(const char *str) {
    terminal_write(str);
    terminal_putchar('\n');
}

void terminal_set_color(uint32_t fg, uint32_t bg) {
    term.fg_color = fg;
    term.bg_color = bg;
}

void terminal_set_cursor_pos(size_t x, size_t y) {
    if (x < term.cols && y < term.rows) {
        term.cursor_x = x;
        term.cursor_y = y;
    }
}