#ifndef GAME_H
#define GAME_H
#include "piece.h"

typedef struct game game_t;

/* ciclo de vida */
game_t *game_get_instance(void);
void game_init(game_t *game);

/* spawn */
uint8_t game_can_spawn_piece(const game_t *game);
void game_spawn_piece(game_t *game);

/* movimiento */
uint8_t game_can_move(const game_t *game, int8_t dx, int8_t dy);
void game_move_piece(game_t *game, int8_t dx, int8_t dy);

uint8_t game_can_rotate(const game_t *game);
void game_rotate_piece(game_t *game);

uint8_t game_can_fall(const game_t *game);
void game_fall_piece(game_t *game);

/* lock / score / fin */
void game_lock_piece(game_t *game);
uint8_t game_last_cleared_lines(const game_t *game);
void game_enter_game_over(game_t *game);
uint8_t game_is_game_over(const game_t *game);

/* consultas para render */
uint8_t game_board_cell(const game_t *game, uint8_t x, uint8_t y);
uint8_t game_current_piece_cell(const game_t *game, uint8_t row, uint8_t col);
int8_t game_current_piece_x(const game_t *game);
int8_t game_current_piece_y(const game_t *game);
piece_type_t game_next_type(const game_t *game);
uint32_t game_score(const game_t *game);

#endif