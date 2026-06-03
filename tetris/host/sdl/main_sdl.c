#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "../../core/fsm.h"
#include "../../core/game.h"
#include "../../core/board.h"
#include "../../platform/display_port.h"
#include "../../platform/input_port.h"
#include "sdl_proxy.h"
#include "display_sdl.h"
#include "input_sdl.h"

/* ================================================================
 * VELOCIDAD DE CAÍDA
 * FALL_MS = milisegundos entre pasos de gravedad.
 *   500ms → normal   250ms → rápido   1000ms → debug
 * ================================================================ */
#define FALL_MS      350u
#define SCORE_SHOW_MS 3000u   /* cuánto se muestra el game-over score */

/* ================================================================
 * RENDER DEL TABLERO
 * Pasa el color por tipo de pieza al draw_cell del adapter SDL.
 * ================================================================ */
static void render_game(const game_t *game, display_port_t *display) {
    display->begin_frame(display->ctx);

    /* Celdas del tablero (0=vacío, 1-7=tipo+1 de pieza locked) */
    for (uint8_t y = 0; y < BOARD_HEIGHT; y++)
        for (uint8_t x = 0; x < BOARD_WIDTH; x++)
            display->draw_cell(display->ctx, x, y,
                               game_board_cell(game, x, y));

    /* Pieza activa: usa su tipo como índice de color */
    uint8_t active_color = (uint8_t)(game_current_piece_type(game) + 1u);
    for (uint8_t row = 0; row < 4u; row++) {
        for (uint8_t col = 0; col < 4u; col++) {
            if (game_current_piece_cell(game, row, col)) {
                int8_t px = game_current_piece_x(game) + (int8_t)col;
                int8_t py = game_current_piece_y(game) + (int8_t)row;
                if (px >= 0 && px < BOARD_WIDTH &&
                    py >= 0 && py < BOARD_HEIGHT)
                    display->draw_cell(display->ctx,
                                       (uint8_t)px, (uint8_t)py,
                                       active_color);
            }
        }
    }

    display->end_frame(display->ctx);
}

/* ================================================================
 * ENTRY POINT
 * ================================================================ */
int main_sdl_init(void) {
    /* ----- inicialización de hardware -----*/
    sdl_proxy_t  proxy;
    if (sdl_proxy_init(&proxy, "TETRIS", WIN_W, WIN_H) != 0)
        return 1;

    sdl_display_t  sdl_disp;
    sdl_input_t    sdl_in;
    game_t        *game  = game_get_instance();
    display_port_t display = sdl_display_create(&sdl_disp, &proxy, game);
    input_port_t   input   = sdl_input_create(&sdl_in, &proxy);

    input.init(input.ctx);

    /* ----- app FSM ----- */
    typedef enum { APP_ATTRACT, APP_PLAYING, APP_SCORE } app_state_t;
    app_state_t app = APP_ATTRACT;

    fsm_t    fsm;
    uint32_t fall_last   = 0u;
    uint32_t score_start = 0u;
    uint32_t attract_tick = 0u;
    int      title_y     = -(FONT_CHAR_H * 7);  /* fuera de pantalla arriba */

    /* Primer render de attract */
    sdl_proxy_poll(&proxy);
    sdl_attract_render(proxy.renderer, title_y, attract_tick);

    while (!proxy.should_quit) {
        sdl_proxy_poll(&proxy);
        if (proxy.should_quit) break;

        input_cmd_t cmd = input.poll(input.ctx);
        if (cmd == INPUT_CMD_QUIT) break;

        uint32_t now = SDL_GetTicks();

        switch (app) {

            /* ---- ATTRACT ---------------------------------------- */
            case APP_ATTRACT: {
                attract_tick++;

                /* Baja el título cada 100ms (~2 frames a 2fps) */
                if (attract_tick % 2 == 0) {
                    title_y++;
                    if (title_y > WIN_H) title_y = -(FONT_CHAR_H * 7);
                }

                sdl_attract_render(proxy.renderer, title_y, attract_tick);

                if (cmd != INPUT_CMD_NONE) {
                    game_init(game);
                    fsm_init(&fsm);
                    fall_last = now;
                    /* Primer spawn */
                    fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                    render_game(game, &display);
                    app = APP_PLAYING;
                }
                break;
            }

            /* ---- PLAYING ---------------------------------------- */
            case APP_PLAYING: {
                /* Input instantáneo */
                switch (cmd) {
                    case INPUT_CMD_LEFT:
                        fsm_dispatch(&fsm, game, FSM_EVENT_MOVE_LEFT); break;
                    case INPUT_CMD_RIGHT:
                        fsm_dispatch(&fsm, game, FSM_EVENT_MOVE_RIGHT); break;
                    case INPUT_CMD_ROTATE:
                        fsm_dispatch(&fsm, game, FSM_EVENT_ROTATE); break;
                    case INPUT_CMD_DOWN:
                        fsm_dispatch(&fsm, game, FSM_EVENT_DOWN);
                        fall_last = now;   /* reinicia intervalo */
                        break;
                    default: break;
                }

                /* SPAWN y LOCK transicionan sin esperar gravedad */
                if (fsm.state == FSM_STATE_LOCK || fsm.state == FSM_STATE_SPAWN) {
                    fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                    fall_last = now;   // ← reinicia el timer de gravedad tras spawn/lock
                }

                 /* Gravedad: solo si está cayendo */
                if (fsm.state == FSM_STATE_FALLING && now - fall_last >= FALL_MS) {
                    fall_last = now;
                    fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                }

                render_game(game, &display);

                if (game_is_game_over(game)) {
                    score_start = now;
                    sdl_score_render(proxy.renderer, game_score(game));
                    app = APP_SCORE;
                }
                break;
            }

            /* ---- SCORE ------------------------------------------ */
            case APP_SCORE:
                if (now - score_start >= SCORE_SHOW_MS) {
                    title_y      = -(FONT_CHAR_H * 7);
                    attract_tick = 0u;
                    sdl_attract_render(proxy.renderer, title_y, attract_tick);
                    app = APP_ATTRACT;
                }
                break;
        }

        /* Limita a ~60 fps si vsync no está disponible */
        SDL_Delay(8);
    }

    input.shutdown(input.ctx);
    sdl_proxy_shutdown(&proxy);
    return 0;
}