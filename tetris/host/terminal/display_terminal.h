#ifndef DISPLAY_TERMINAL_H
#define DISPLAY_TERMINAL_H
#include "terminal_proxy.h"
#include "../../platform/display_port.h"

typedef struct {
    terminal_proxy_t proxy;
} terminal_display_t;

void terminal_display_init(terminal_display_t *display, char *buffer, uint8_t width, uint8_t height);

display_port_t terminal_display_create(terminal_display_t *display);

#endif