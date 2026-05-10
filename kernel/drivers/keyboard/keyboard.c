#include "keyboard.h"
#include "terminal/terminal.h"
#include "io.h"
#include "shell.h"

bool is_app_running;
static bool shift_pressed = false;

static const char ascii_table[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

static const char ascii_table_shift[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

// void set_keyboard_leds(uint8_t status) {
//     while (inb(0x64) & 2);

//     outb(0x60, 0xED);

//     while (inb(0x64) & 2);

//     outb(0x60, status);
// }

char get_ascii_char(uint8_t scancode) {
    if (scancode == 0x2A || scancode == 0x36) { shift_pressed = true; return 0; }
    if (scancode == 0xAA || scancode == 0xB6) { shift_pressed = false; return 0; }
    if (scancode & 0x80) return 0;
    if (scancode >= sizeof(ascii_table)) return 0;
    return shift_pressed ? ascii_table_shift[scancode] : ascii_table[scancode];
}

void keyboard_handler() {
    uint8_t scancode = inb(0x60);

    char c = get_ascii_char(scancode);

    if (c && !is_app_running) {
        shell_handler_char(c);
    }
}