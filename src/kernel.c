#include "include/stdint.h"
#include "include/stddef.h"
#include "include/stdbool.h"
#include "boot/limine.h"
#include "drivers/terminal/terminal.h"

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

static void hcf(void) {
    for (;;) {
#if defined (__x86_64__)
        asm ("hlt");
#elif defined (__aarch64__) || defined (__riscv)
        asm ("wfi");
#elif defined (__loongarch64)
        asm ("idle 0");
#endif
    }
}

void kmain(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hcf();
    }

    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    terminal_init(framebuffer);

    terminal_set_color(0x00FF00, 0x000000);
    terminal_writeln("=========================================");
    terminal_writeln("       My Operating System v0.1         ");
    terminal_writeln("=========================================");
    terminal_writeln("");

    terminal_set_color(0xFFFFFF, 0x000000);
    terminal_write("[INFO] Framebuffer: ");
    terminal_write("OK");
    terminal_writeln("");

    terminal_write("[INFO] Resolution: ");
    terminal_write("1024x768");
    terminal_writeln("");

    terminal_write("[INFO] Terminal size: ");
    terminal_write("80x25");
    terminal_writeln("");
    terminal_writeln("");

    terminal_set_color(0x00AAFF, 0x000000);
    terminal_writeln("> System ready.");
    terminal_writeln("> Type 'help' for available commands.");
    terminal_writeln("");

    terminal_set_color(0xFFFFFF, 0x000000);
    terminal_write("user@zenixos:~$ ");

    hcf();
}