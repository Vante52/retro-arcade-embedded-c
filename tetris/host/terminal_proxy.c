#include "terminal_proxy.h"
#include <stdio.h>

void terminal_proxy_clear(terminal_proxy_t *term) {
    for (uint8_t y = 0; y < term->height; y++) {
        for (uint8_t x = 0; x<term->width; x++) {
            term -> buffer [y*term -> width +x] = '.';
        }
    }
}

void terminal_proxy_putc (terminal_proxy_t *term, uint8_t x, uint8_t y, char c) {
    if (x<term->width && y<term->height) {
        term -> buffer [y*term->width + x] = c;
    }
}

//vacia la pantalla
void terminal_proxy_flush(terminal_proxy_t *term) {
    printf("\033[H\033[J");

    for (uint8_t y = 0; y < term->height; y++) {
        printf("|");
        for (uint8_t x = 0; x < term->width; x++) {
            putchar(term->buffer[y * term->width + x]);
        }
        printf("|\n");
    }
    //borde inferior
    printf("+");
    for (uint8_t i = 0; i < term->width; i++) {
        putchar('-');
    }
    printf("+\n");

    printf("Score: %d\n", 0);

    fflush(stdout);
}