#ifndef TETRIS_TERMINAL_PROXY_H
#define TETRIS_TERMINAL_PROXY_H

#include <stdint.h>

typedef struct {
    char *buffer;
    uint8_t width;
    uint8_t height;
} terminal_proxy_t;

void terminal_proxy_clear(terminal_proxy_t *term);
void terminal_proxy_putc(terminal_proxy_t *term, uint8_t x, uint8_t y, char c);
void terminal_proxy_flush(terminal_proxy_t *term);

#endif //TETRIS_TERMINAL_PROXY_H