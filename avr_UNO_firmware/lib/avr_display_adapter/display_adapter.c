#include "display_adapter.h"

static void avr_begin_frame(void *ctx) {
    display_avr_t *self = (display_avr_t *)ctx;
    dual_matrix_proxy_clear(self->display);
}

static void avr_draw_cell(void *ctx, uint8_t x, uint8_t y, uint8_t value) {
    display_avr_t *self = (display_avr_t *)ctx;
    dual_matrix_proxy_set_pixel(self->display, x, y, value ? 1u : 0u);
}

static void avr_end_frame(void *ctx) {
    (void)ctx;
    /* no hace nada:
       el barrido real ocurre en dual_matrix_proxy_scan_step() */
}

void display_avr_init(display_avr_t *self, dual_matrix_proxy_t *display) {
    self->display = display;
}

display_port_t display_avr_create_port(display_avr_t *self) {
    display_port_t port;

    port.begin_frame = avr_begin_frame;
    port.draw_cell   = avr_draw_cell;
    port.end_frame   = avr_end_frame;
    port.ctx         = self;

    return port;
}