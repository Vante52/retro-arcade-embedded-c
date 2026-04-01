#ifndef BOARD_H
#define BOARD_H
#include "piece.h"
#include <stdint.h>
#include "platform/display_port.h"

//Definición de las dimensiones del tablero
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 16

//Definición del tablero
typedef struct{
    uint8_t grid[BOARD_HEIGHT][BOARD_WIDTH];
}board_t;

//por buenas practicas para el retorno de los errores
typedef enum {
    BOARD_PLACE_BLOCKED = 0,
    BOARD_PLACE_OK = 1,
    BOARD_PLACE_OUT_OF_BOUNDS = 2
} board_place_result_t;

//Inicializa el tablero
void board_init(board_t *board);
//Dibuja el tablero
void board_render(const board_t *board, display_port_t *display);
//verifica si la pieza cabe en el tablero
board_place_result_t board_check_placement (const board_t *board, const piece_t *piece);
//coloca la pieza en el tablero
uint8_t board_lock_piece(board_t *board, const piece_t *piece);
//limpia una linea si es llenada
uint8_t board_clear_lines(board_t *board, const piece_t *piece);
//verifica si en una fila hay un bloque
static uint8_t piece_row_has_block(const piece_t *piece, uint8_t row);
#endif