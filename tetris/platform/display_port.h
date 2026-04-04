#ifndef DISPLAY_PORT_H
#define DISPLAY_PORT_H
#include <stdint.h>

#include <stdint.h>

typedef struct {
    void (*begin_frame)(void *ctx);
    void (*draw_cell)(void *ctx, uint8_t x, uint8_t y, uint8_t color);
    void (*end_frame)(void *ctx);
    void *ctx;
} display_port_t;

#endif