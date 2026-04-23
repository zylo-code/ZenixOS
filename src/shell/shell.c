#include "shell.h"
#include <stdbool.h>
#include <stdint.h>
#include "../system/memory.h"
#include "../drivers/terminal/terminal.h"

char shell_buffer[64] = {0};
int shell_idx = 0;

void shell_init() {
    memset(shell_buffer, 0, 64);
    shell_idx = 0;
}

void shell_handler_char(char c) {
    if (c == '\n') {
        terminal_writeln("");
        terminal_write("user@zenixos:~$ ");
        shell_buffer[shell_idx] = '\0';

        if (strcmp(shell_buffer, "help") == 0) {
            terminal_writeln("");
            terminal_writeln("help");
        } else if (shell_buffer[0] != '\0') {
            terminal_writeln("Unknow command.");
        }

        shell_idx = 0;
        memset(shell_buffer, 0, 64);
    }

    if (c == '\b' && shell_idx > 0) {
        shell_idx--;
        terminal_putchar('\b');
    }

    if (shell_idx < 63 && c >= ' ' && c <= '~') {
        shell_buffer[shell_idx++] = c;
        terminal_putchar(c);
    }
}