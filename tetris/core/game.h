#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "piece.h"
#include "bag.h"
#include "score.h"

//Estructura del juego
typedef struct{
    board_t board;
    piece_t current_piece;
    piece_type_t next_type;
    piece_bag_t bag;
    score_t score;
    uint8_t game_over;
    uint8_t tick_accum;
}game_t;

//Inicializa el juego
void game_init(game_t *game);
//Dibuja el juego
void game_draw(game_t *game);
//Instancia la pieza
void game_spawn_piece(game_t *game);
//Me trae el tipo de la siguiente pieza
piece_type_t game_piece_type(game_t *game);
//Deja caer la pieza
void game_fall(game_t *game);
//Maneja la entrada del usuario
void game_input(game_t *game);
//intenta mover la pieza
uint8_t game_move_piece(game_t *game, int8_t dx, int8_t dy);
//intenta rotar la pieza
uint8_t game_rotate_piece(game_t *game);
//revisa si una linea es llenada
uint8_t game_line_filled(game_t *game);
//limpia las lineas llenadas del tablero
void game_clear_line(game_t *game);
//Juego terminado
void game_over(game_t *game);
#endif