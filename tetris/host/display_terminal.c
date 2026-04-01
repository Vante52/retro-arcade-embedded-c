#include <stdio.h>
#include <string.h>
#include "platform/display_port.h"
#include "core/board.h"

typedef struct {
    char frame[BOARD_HEIGHT][BOARD_WIDTH];
} terminal_display_t;

static void terminal_begin_frame(void *ctx) {
    terminal_display_t *term = (terminal_display_t *)ctx;
    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
        for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
            term->frame[y][x] = '.';
        }
    }
}

static void terminal_draw_cell(void *ctx, uint8_t x, uint8_t y, uint8_t value) {
    terminal_display_t *term = (terminal_display_t *)ctx;
    term->frame[y][x] = value ? '#' : '.';
}

static void terminal_end_frame(void *ctx) {
    terminal_display_t *term = (terminal_display_t *)ctx;
    printf("\033[H\033[J"); /* limpiar pantalla ANSI */
    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
        printf("|");
        for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
            printf("%c", term->frame[y][x]);
        }
        printf("|\n");
    }
    for (uint8_t i = 0; i < BOARD_WIDTH + 2; i++) {
        printf("-");
    }
    printf("\n");
}

display_port_t terminal_display_create(terminal_display_t *term) {
    display_port_t port;
    port.begin_frame = terminal_begin_frame;
    port.draw_cell = terminal_draw_cell;
    port.end_frame = terminal_end_frame;
    port.ctx = term;
    return port;
}