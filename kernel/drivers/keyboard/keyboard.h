#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../include/stdint.h"
#include "../include/stddef.h"
#include "../include/stdbool.h"

void set_keyboard_leds(uint8_t status);
void keyboard_handler();
char get_ascii_char(uint8_t scancode);

#endif