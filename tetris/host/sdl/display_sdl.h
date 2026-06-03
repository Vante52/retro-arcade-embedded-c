#ifndef DISPLAY_SDL_H
#define DISPLAY_SDL_H

#include "sdl_proxy.h"
#include "../../platform/display_port.h"
#include "../../core/game.h"

/* ----------------------------------------------------------------
 * Dimensiones del layout de la ventana (píxeles)
 * ---------------------------------------------------------------- */
#define CELL_SIZE    30           /* px por celda del tablero        */
#define MARGIN       20           /* margen exterior                 */
#define PANEL_W      130          /* ancho del panel lateral         */

/* Posición origen del tablero dentro de la ventana */
#define BOARD_OX     MARGIN
#define BOARD_OY     MARGIN

#define FONT_CHAR_H  6

/* Dimensiones totales de la ventana */
#define WIN_W  (MARGIN + (CELL_SIZE * 8) + MARGIN + PANEL_W + MARGIN)
#define WIN_H  (MARGIN + (CELL_SIZE * 16) + MARGIN)

/* ----------------------------------------------------------------
 * Contexto del adapter
 * ---------------------------------------------------------------- */
typedef struct {
    sdl_proxy_t     *proxy;
    const game_t    *game;   /* necesario para el panel lateral */
} sdl_display_t;

/* Inicializa el adapter y retorna el display_port_t listo para usar */
display_port_t sdl_display_create(sdl_display_t *d,
                                  sdl_proxy_t   *proxy,
                                  const game_t  *game);
void sdl_attract_render(SDL_Renderer *r, int title_y, uint32_t pulse_tick);
void sdl_score_render(SDL_Renderer *r, uint32_t score);

#endif /* DISPLAY_SDL_H */