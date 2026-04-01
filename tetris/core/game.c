#include "game.h"
#include <stdint.h>
#include <stdio.h>

#include "fsm.h"

//Inicializa el juego
void game_init(game_t *game){
    board_init(&game->board);
    bag_init(&game-> bag);
    game_spawn_piece(game);
    game_input(game);
    score_init(&game->score);
    game->game_over = 0;
}
//Instancia la pieza
void game_spawn_piece(game_t *game) {
    piece_init(&game->current_piece, bag_next(&game->bag), BOARD_WIDTH / 2, 0);
}

//Me trae el tipo de la siguiente pieza
piece_type_t game_piece_type(game_t *game) {
    return bag_next(&game ->bag);
}

//Dibuja el juego usando el display port que se esté usando
void game_render(const game_t *game, display_port_t *display) {
    display->begin_frame(display->ctx);
    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
        for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
            uint8_t value = (game->board.grid[y][x] == '#') ? 1u : 0u;
            display->draw_cell(display->ctx, x, y, value);
        }
    }
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            if (get_piece(&game->current_piece, i, j)) {
                int8_t x = game->current_piece.x + j;
                int8_t y = game->current_piece.y + i;
                if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                    display->draw_cell(display->ctx, (uint8_t)x, (uint8_t)y, 1u);
                }
            }
        }
    }
    display->end_frame(display->ctx);
}

void game_clear_line(game_t *game) {

}

//Mueve la pieza una pieza hacia abajo
void game_fall(game_t *game){
    game_move_piece(game, 0, -1);
    board_check_placement(&game -> board, &game -> current_piece);
}

//Mueve la pieza
uint8_t game_move_piece(game_t *game, int8_t dx, int8_t dy) {
    piece_t temp = game->current_piece;
    piece_move(&temp, dx, dy);
    return board_check_placement(&game -> board, &game -> current_piece);


}

//rota la pieza
uint8_t game_rotate_piece(game_t *game) {
    piece_t temp = game->current_piece;
    piece_rotate(&temp);
    return board_check_placement(&game -> board, &game -> current_piece);

}

//Maneja la entrada del usuario
void game_input(game_t *game){
    
}

uint8_t game_line_filled(game_t *game) {
    return 1;
}