#include "core/game.h"
#include "core/board.h"
#include "platform/display_port.h"

static void render_game(const game_t *game, display_port_t *display);

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
int main(){
    game_t *game;
    display_port_t *display;
    render_game(game, display);

    return 0;
}