#include "attract.h"
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------------------------
 * Fuente de píxeles 3×5 para las letras de TETRIS
 * ---------------------------------------------------------------- */
#define FONT_H  5
#define FONT_W  3
#define N_LETTERS 6

static const uint8_t FONT[N_LETTERS][FONT_H][FONT_W] = {
    /* T */ { {1,1,1}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0} },
    /* E */ { {1,1,1}, {1,0,0}, {1,1,0}, {1,0,0}, {1,1,1} },
    /* T */ { {1,1,1}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0} },
    /* R */ { {1,1,0}, {1,0,1}, {1,1,0}, {1,1,0}, {1,0,1} },
    /* I */ { {1,1,1}, {0,1,0}, {0,1,0}, {0,1,0}, {1,1,1} },
    /* S */ { {0,1,1}, {1,0,0}, {0,1,0}, {0,0,1}, {1,1,0} },
};

/* Ancho total del título: 6 letras × 3 cols + 5 huecos × 1 = 23 */
#define TITLE_W (N_LETTERS * FONT_W + (N_LETTERS - 1))

/* ---------------------------------------------------------------- */

void attract_init(attract_t *a) {
    a->title_y  = -FONT_H;   /* empieza justo por encima del borde */
    a->tick_div = 0u;
}

void attract_tick(attract_t *a) {
    a->tick_div++;
    if (a->tick_div < ATTRACT_FALL_DIV) return;
    a->tick_div = 0u;

    a->title_y++;

    /* cuando la última fila del título sale por abajo, reinicia */
    if (a->title_y >= ATTRACT_H) {
        a->title_y = -FONT_H;
    }
}

void attract_render(const attract_t *a) {
    /* buffer de caracteres (espacio = vacío) */
    char buf[ATTRACT_H][ATTRACT_W];
    memset(buf, ' ', sizeof(buf));

    /* centrar horizontalmente: (24 - 23) / 2 = 0 → margen izq. de 1 para la piped */
    const int start_x = (ATTRACT_W - TITLE_W) / 2;

    for (int li = 0; li < N_LETTERS; li++) {
        int lx = start_x + li * (FONT_W + 1);  /* +1 = hueco entre letras */
        for (int row = 0; row < FONT_H; row++) {
            int sy = a->title_y + row;
            if (sy < 0 || sy >= ATTRACT_H) continue;
            for (int col = 0; col < FONT_W; col++) {
                int sx = lx + col;
                if (sx >= 0 && sx < ATTRACT_W && FONT[li][row][col]) {
                    buf[sy][sx] = '#';
                }
            }
        }
    }

    /* imprimir: usamos printf directo, no terminal_proxy (buffer distinto) */
    printf("\033[H\033[J");
    printf("+------------------------+\n");
    for (int y = 0; y < ATTRACT_H; y++) {
        printf("|");
        for (int x = 0; x < ATTRACT_W; x++) putchar(buf[y][x]);
        printf("|\n");
    }
    printf("+------------------------+\n");
    printf("   PRESIONA CUALQUIER TECLA   \n");
    fflush(stdout);
}