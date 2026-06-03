#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "../core/fsm.h"
#include "../core/game.h"
#include "../core/board.h"
#include "../platform/display_port.h"
#include "../platform/input_port.h"
#include "terminal/attract.h"
#include "display_terminal.h"
#include "input_keyboard.h"

#define _POSIX_C_SOURCE 200809L
#define FALL_TICKS  10u   /* 10 × 50ms = 500ms por paso */
#define SCORE_TICKS 60u   /* 60 × 50ms = 3 segundos     */


typedef enum {
    APP_STATE_ATTRACT,
    APP_STATE_PLAYING,
    APP_STATE_SCORE,
} app_state_t;

static void render_game(const game_t *game, display_port_t *display) {
    display->begin_frame(display->ctx);

    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
        for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
            /* != 0u en lugar de == '#': compatible con el nuevo board */
            uint8_t value = (game_board_cell(game, x, y) != 0u) ? 1u : 0u;
            display->draw_cell(display->ctx, x, y, value);
        }
    }

    for (uint8_t row = 0; row < 4u; row++) {
        for (uint8_t col = 0; col < 4u; col++) {
            if (game_current_piece_cell(game, row, col)) {
                int8_t px = game_current_piece_x(game) + (int8_t)col;
                int8_t py = game_current_piece_y(game) + (int8_t)row;
                if (px >= 0 && px < BOARD_WIDTH && py >= 0 && py < BOARD_HEIGHT)
                    display->draw_cell(display->ctx, (uint8_t)px, (uint8_t)py, 1u);
            }
        }
    }

    display->end_frame(display->ctx);
}

int main_host_init(void) {
    static char display_buffer[BOARD_WIDTH * BOARD_HEIGHT];

    terminal_display_t  terminal_display;
    display_port_t      display;
    game_t             *game = game_get_instance();
    fsm_t               fsm;
    keyboard_input_t    keyboard;
    input_port_t        input;

    app_state_t app_state    = APP_STATE_ATTRACT;
    attract_t   attract;
    uint8_t     fall_counter = 0u;
    uint8_t     score_timer  = 0u;

    terminal_display_init(&terminal_display, display_buffer,
                          BOARD_WIDTH, BOARD_HEIGHT);
    display = terminal_display_create(&terminal_display);

    input = input_keyboard_create(&keyboard);
    input.init(input.ctx);

    attract_init(&attract);
    attract_render(&attract);

    while (1) {
        struct timespec ts = {0, 50000000L};
        nanosleep(&ts, NULL);

        input_cmd_t cmd = input.poll(input.ctx);
        if (cmd == INPUT_CMD_QUIT) break;

        switch (app_state) {

            case APP_STATE_ATTRACT:
                attract_tick(&attract);
                attract_render(&attract);
                if (cmd != INPUT_CMD_NONE) {
                    game_init(game);
                    fsm_init(&fsm);
                    fall_counter = 0u;
                    fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                    render_game(game, &display);
                    app_state = APP_STATE_PLAYING;
                }
                break;

            case APP_STATE_PLAYING:
                switch (cmd) {
                    case INPUT_CMD_LEFT:
                        fsm_dispatch(&fsm, game, FSM_EVENT_MOVE_LEFT); break;
                    case INPUT_CMD_RIGHT:
                        fsm_dispatch(&fsm, game, FSM_EVENT_MOVE_RIGHT); break;
                    case INPUT_CMD_ROTATE:
                        fsm_dispatch(&fsm, game, FSM_EVENT_ROTATE); break;
                    case INPUT_CMD_DOWN:
                        fsm_dispatch(&fsm, game, FSM_EVENT_DOWN);
                        fall_counter = 0u; break;
                    default: break;
                }

                if (fsm.state == FSM_STATE_LOCK || fsm.state == FSM_STATE_SPAWN) {
                    fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                    fall_counter = 0u;
                }

                fall_counter++;
                if (fall_counter >= FALL_TICKS) {
                    fall_counter = 0u;
                    if (fsm.state == FSM_STATE_FALLING)
                        fsm_dispatch(&fsm, game, FSM_EVENT_TICK);
                }

                render_game(game, &display);

                if (game_is_game_over(game)) {
                    score_timer = 0u;
                    printf("\033[H\033[J  GAME OVER  Score: %lu\n",
                           (unsigned long)game_score(game));
                    app_state = APP_STATE_SCORE;
                }
                break;

            case APP_STATE_SCORE:
                score_timer++;
                if (score_timer >= SCORE_TICKS) {
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