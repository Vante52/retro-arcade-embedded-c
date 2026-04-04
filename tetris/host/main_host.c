#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "../core/fsm.h"
#include "../core/game.h"
#include "input_keyboard.h"
#include "../platform/input_port.h"

static void process_input(input_port_t *input, fsm_t *fsm, game_t *game, uint8_t *running) {
    input_cmd_t cmd = input->poll(input->ctx);

    switch (cmd) {
        case INPUT_CMD_LEFT:
            fsm_dispatch(fsm, game, FSM_EVENT_MOVE_LEFT);
            break;

        case INPUT_CMD_RIGHT:
            fsm_dispatch(fsm, game, FSM_EVENT_MOVE_RIGHT);
            break;

        case INPUT_CMD_ROTATE:
            fsm_dispatch(fsm, game, FSM_EVENT_ROTATE);
            break;

        case INPUT_CMD_DOWN:
            fsm_dispatch(fsm, game, FSM_EVENT_DOWN);
            break;

        case INPUT_CMD_NONE:
        default:
            break;
    }
}

int main_host_init() {
    game_t *game = game_get_instance();
    fsm_t fsm;
    keyboard_input_t keyboard;
    input_port_t input;
    uint8_t running = 1u;

    game_init(game);
    fsm_init(&fsm);

    input = input_keyboard_create(&keyboard);
    input.init(input.ctx);

    /* primer tick para spawn */
    fsm_dispatch(&fsm, game, FSM_EVENT_TICK);

    while (running) {
        struct timespec ts = {0, 50000000}; /* 50 ms */
        nanosleep(&ts, NULL);

        process_input(&input, &fsm, game, &running);

        /* tick periódico de caída */
        fsm_dispatch(&fsm, game, FSM_EVENT_TICK);

        if (game_is_game_over(game)) {
            running = 0u;
        }
    }

    input.shutdown(input.ctx);
    return 0;
}

static void render_game(const game_t *game, display_port_t *display) {
    display->begin_frame(display->ctx);

    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
        for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
            uint8_t value = (game_board_cell(game, x, y) == '#') ? 1u : 0u;
            display->draw_cell(display->ctx, x, y, value);
        }
    }

    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            if (game_current_piece_cell(game, row, col)) {
                int8_t x = game_current_piece_x(game) + col;
                int8_t y = game_current_piece_y(game) + row;

                if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                    display->draw_cell(display->ctx, (uint8_t)x, (uint8_t)y, 1u);
                }
            }
        }
    }

    display->end_frame(display->ctx);
}