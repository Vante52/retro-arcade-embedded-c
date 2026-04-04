#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "../core/fsm.h"
#include "../core/game.h"
#include "../core/board.h"
#include "../platform/display_port.h"
#include "../platform/input_port.h"
#include "attract.h"
#include "display_terminal.h"
#include "input_keyboard.h"

/* ================================================================
 * VELOCIDAD DE CAÍDA
 * Loop principal = 50 ms.  FALL_TICKS = cuántos ciclos esperamos
 * antes de enviar el tick de gravedad a la FSM.
 *
 *  FALL_TICKS  10 →  500 ms / paso   (normal)
 *  FALL_TICKS   5 →  250 ms / paso   (rápido)
 *  FALL_TICKS  20 → 1000 ms / paso   (lento, para debug)
 * ================================================================ */
#define FALL_TICKS 10u

/* Duración de la pantalla de score antes de volver a attract.
 * SCORE_TICKS × 50ms = tiempo visible.  60 → 3 segundos.        */
#define SCORE_TICKS 60u

/* ================================================================
 * FSM DE APLICACIÓN — por encima del juego
 * ================================================================ */
typedef enum {
    APP_STATE_ATTRACT,   /* pantalla de título con letras cayendo  */
    APP_STATE_PLAYING,   /* partida en curso                       */
    APP_STATE_SCORE,     /* game-over: muestra score unos segundos */
} app_state_t;

/* ================================================================
 * RENDER — definidas antes de main_host_init para que sean visibles
 * ================================================================ */

static void render_game(const game_t *game, display_port_t *display) {
    display->begin_frame(display->ctx);

    /* Celdas fijas del tablero */
    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
        for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
            uint8_t v = (game_board_cell(game, x, y) == '#') ? 1u : 0u;
            display->draw_cell(display->ctx, x, y, v);
        }
    }

    /* Superpone la pieza activa */
    for (uint8_t row = 0; row < 4u; row++) {
        for (uint8_t col = 0; col < 4u; col++) {
            if (game_current_piece_cell(game, row, col)) {
                int8_t px = game_current_piece_x(game) + (int8_t)col;
                int8_t py = game_current_piece_y(game) + (int8_t)row;
                if (px >= 0 && px < BOARD_WIDTH && py >= 0 && py < BOARD_HEIGHT) {
                    display->draw_cell(display->ctx, (uint8_t)px, (uint8_t)py, 1u);
                }
            }
        }
    }

    display->end_frame(display->ctx);
}

static void render_score_screen(uint32_t score) {
    printf("\033[H\033[J");
    printf("\n\n\n");
    printf("     +--------------+\n");
    printf("     |  GAME  OVER  |\n");
    printf("     +--------------+\n");
    printf("     |              |\n");
    printf("     | Score:%6lu |\n", (unsigned long)score);
    printf("     |              |\n");
    printf("     +--------------+\n");
    fflush(stdout);
}

/* ================================================================
 * ENTRY POINT
 * ================================================================ */
int main_host_init(void) {
    /* Buffer del tablero de juego (BOARD_WIDTH × BOARD_HEIGHT chars) */
    static char display_buffer[BOARD_WIDTH * BOARD_HEIGHT];

    /* Pipeline de display del juego */
    terminal_display_t  terminal_display;
    display_port_t      display;

    /* Input */
    keyboard_input_t    keyboard;
    input_port_t        input;

    /* Estado de la aplicación */
    app_state_t         app_state  = APP_STATE_ATTRACT;
    attract_t           attract;
    game_t             *game       = game_get_instance();
    fsm_t               fsm;

    uint8_t fall_counter  = 0u;
    uint8_t score_timer   = 0u;

    /* ----- Init ------------------------------------------------- */
    terminal_display_init(&terminal_display, display_buffer,
                          BOARD_WIDTH, BOARD_HEIGHT);
    display = terminal_display_create(&terminal_display);

    input = input_keyboard_create(&keyboard);
    input.init(input.ctx);

    attract_init(&attract);
    attract_render(&attract);   /* primer frame inmediato */

    /* ===== LOOP PRINCIPAL ======================================== */
    while (1) {
        struct timespec ts = {0, 50000000L}; /* 50 ms */
        nanosleep(&ts, NULL);

        input_cmd_t cmd = input.poll(input.ctx);

        /* Quit universal */
        if (cmd == INPUT_CMD_QUIT) break;

        switch (app_state) {

            /* ---- ATTRACT ---------------------------------------- */
            case APP_STATE_ATTRACT:
                attract_tick(&attract);
                attract_render(&attract);

                if (cmd != INPUT_CMD_NONE) {
                    /* Iniciar partida nueva */
                    game_init(game);
                    fsm_init(&fsm);
                    fall_counter = 0u;

                    /* Primer tick: spawn de la primera pieza */
                    fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                    render_game(game, &display);

                    app_state = APP_STATE_PLAYING;
                }
                break;

            /* ---- PLAYING ---------------------------------------- */
            case APP_STATE_PLAYING:

                /* Input con respuesta inmediata (cada 50 ms) */
                switch (cmd) {
                    case INPUT_CMD_LEFT:
                        fsm_dispatch(&fsm, game, FSM_EVENT_MOVE_LEFT);
                        break;
                    case INPUT_CMD_RIGHT:
                        fsm_dispatch(&fsm, game, FSM_EVENT_MOVE_RIGHT);
                        break;
                    case INPUT_CMD_ROTATE:
                        fsm_dispatch(&fsm, game, FSM_EVENT_ROTATE);
                        break;
                    case INPUT_CMD_DOWN:
                        /* DOWN dispara el tick directamente sin esperar el contador */
                        fsm_dispatch(&fsm, game, FSM_EVENT_DOWN);
                        fall_counter = 0u;  /* reinicia para no doblar la caída */
                        break;
                    default:
                        break;
                }

                /* SPAWN y LOCK transicionan inmediatamente (sin gravedad).
                 * Solo FALLING respeta el intervalo de caída.              */
                if (fsm.state == FSM_STATE_LOCK || fsm.state == FSM_STATE_SPAWN) {
                    fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                    fall_counter = 0u;
                }

                /* Gravedad: tick cada FALL_TICKS × 50ms */
                fall_counter++;
                if (fall_counter >= FALL_TICKS) {
                    fall_counter = 0u;
                    if (fsm.state == FSM_STATE_FALLING) {
                        fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                    }
                }

                render_game(game, &display);

                /* Detectar game over */
                if (game_is_game_over(game)) {
                    score_timer = 0u;
                    render_score_screen(game_score(game));
                    app_state = APP_STATE_SCORE;
                }
                break;

            /* ---- SCORE ------------------------------------------ */
            case APP_STATE_SCORE:
                score_timer++;
                if (score_timer >= SCORE_TICKS) {
                    /* Volver a attract después de mostrar el score */
                    attract_init(&attract);
                    attract_render(&attract);
                    app_state = APP_STATE_ATTRACT;
                }
                break;
        }
    }

    input.shutdown(input.ctx);
    return 0;
}