#include "display_terminal.h"

static void terminal_begin_frame(void *ctx) {
    terminal_display_t *display = (terminal_display_t *)ctx;
    terminal_proxy_clear(&display->proxy);
}

static void terminal_draw_cell(void *ctx, uint8_t x, uint8_t y, uint8_t value) {
    terminal_display_t *display = (terminal_display_t *)ctx;
    terminal_proxy_putc(&display->proxy, x, y, value ? '#' : '.');
}

static void terminal_end_frame(void *ctx) {
    terminal_display_t *display = (terminal_display_t *)ctx;
    terminal_proxy_flush(&display->proxy);
}

void terminal_display_init(terminal_display_t *display, char *buffer, uint8_t width, uint8_t height) {
    display->proxy.buffer = buffer;
    display->proxy.width = width;
    display->proxy.height = height;
}

display_port_t terminal_display_create(terminal_display_t *display) {
    display_port_t port;

    port.begin_frame = terminal_begin_frame;
    port.draw_cell = terminal_draw_cell;
    port.end_frame = terminal_end_frame;
    port.ctx = display;

    return port;
}