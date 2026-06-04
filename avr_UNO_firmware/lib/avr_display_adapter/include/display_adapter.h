#ifndef DISPLAY_ADAPTER_H
#define DISPLAY_ADAPTER_H

#include <stdint.h>
#include <display_port.h>
#include <dual_matrix_proxy.h>

typedef struct {
    dual_matrix_proxy_t *display;
} display_avr_t;

void display_avr_init(display_avr_t *self, dual_matrix_proxy_t *display);
display_port_t display_avr_create_port(display_avr_t *self);

#endif