#ifndef TETRIS_ATTRACT_H
#define TETRIS_ATTRACT_H

#include <stdint.h>

/* Dimensiones del buffer de pantalla de atracción */
#define ATTRACT_W  24
#define ATTRACT_H  18

/* Ticks de 50 ms que deja pasar antes de bajar el título 1 fila.
 * ATTRACT_FALL_DIV × 50ms = ms por paso.  4 → 200ms por fila.  */
#define ATTRACT_FALL_DIV 4u

typedef struct {
    int16_t title_y;   /* fila actual de la primera línea del título  */
    uint8_t tick_div;  /* contador interno para ralentizar la animación */
} attract_t;

void attract_init(attract_t *a);
void attract_tick(attract_t *a);
void attract_render(const attract_t *a);
#endif //TETRIS_ATTRACT_H