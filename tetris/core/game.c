#include "game.h"
#include <stdint.h>
#include "board.h"
#include "bag.h"
#include "score.h"

struct game {
    board_t board;
    piece_t current_piece;
    piece_bag_t bag;
    score_t score;

    piece_type_t next_type;

    uint8_t game_over;
    uint8_t last_cleared_lines;
};

//instancia del opaque de game
static game_t game_instance;

//getter de game
game_t *game_get_instance(void) {
    return &game_instance;
}

//Inicializa el juego
void game_init(game_t *game){
    board_init(&game->board);
    bag_init(&game-> bag);
    score_init(&game->score);

    game->game_over = 0;
    game->last_cleared_lines = 0;

    game -> next_type = bag_next(&game->bag);
}
//Verifica si la pieza cabe
uint8_t game_can_spawn_piece(const game_t *game) {
    piece_t temp;
    piece_init(&temp, game->next_type, BOARD_WIDTH/2, 0);
    return board_check_placement(&game->board, &temp)== BOARD_PLACE_OK;
}
//spawnea la pieza
void game_spawn_piece(game_t *game) {
    piece_init(&game->current_piece, game->next_type, BOARD_WIDTH/2, 0);
    game->next_type = bag_next(&game->bag);
}

//Revisa si la pieza puede moverse hacia abajo
uint8_t game_can_move(const game_t *game, int8_t dx, int8_t dy) {
    piece_t temp = game->current_piece;
    piece_move(&temp, dx, dy);
    return board_check_placement(&game->board, &temp) == BOARD_PLACE_OK;
}
//si se puede mover hacia abajo, la coloca
void game_move_piece(game_t *game, int8_t dx, int8_t dy) {
    piece_t temp = game->current_piece;
    piece_move(&temp, dx, dy);
    if (board_check_placement(&game->board, &temp) == BOARD_PLACE_OK) {
        game->current_piece = temp;
    }
}

//revisa si la pieza puede caer
uint8_t game_can_fall(const game_t *game){
    return game_can_move(game, 0, 1);
}
//deja caer la pieza
void game_fall_piece( game_t *game){
    game_move_piece(game, 0, 1);
}

//revisa si la pieza puede rotar
uint8_t game_can_rotate(const game_t *game) {
    piece_t temp = game->current_piece;
    piece_rotate(&temp);
    return board_check_placement(&game->board, &temp) == BOARD_PLACE_OK;
}
//rota la pieza
void game_rotate_piece(game_t *game) {
    piece_t temp = game->current_piece;
    piece_rotate(&temp);

    if (board_check_placement(&game->board, &temp) == BOARD_PLACE_OK) {
        game->current_piece = temp;
    }
}

//Coloca la pieza
void game_lock_piece(game_t *game) {
    game->last_cleared_lines = board_lock_piece(&game->board, &game->current_piece);
    score_add_lines(&game->score, game->last_cleared_lines);
}


uint8_t game_board_cell(const game_t *game, uint8_t x, uint8_t y) {
    return game->board.grid[y][x];
}
//manda la celda de la pieza
uint8_t game_current_piece_cell(const game_t *game, uint8_t row, uint8_t col) {
    return get_piece(&game->current_piece, row, col);
}
//pos x de la pieza
int8_t game_current_piece_x(const game_t *game) {
    return game->current_piece.x;
}
//pos y de la pieza
int8_t game_current_piece_y(const game_t *game) {
    return game->current_piece.y;
}
//tipo de la siguiente pieza
piece_type_t game_next_type(const game_t *game) {
    return game->next_type;
}
//puntaje
uint32_t game_score(const game_t *game) {
    return score_get(&game->score);
}
//lineas limpiadas
uint8_t game_last_cleared_lines(const game_t *game) {
    return game->last_cleared_lines;
}
//game over
void game_enter_game_over(game_t *game) {
    game->game_over = 1u;
}
// juego terminado?
uint8_t game_is_game_over(const game_t *game) {
    return game->game_over;
}

piece_type_t game_current_piece_type(const game_t *game) {
     return game->current_piece.type;
}
