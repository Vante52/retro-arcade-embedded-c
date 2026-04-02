#include "board.h"
#include <stdint.h>


//limpia una linea si es llenada, asume que ya se llamó board can place
static uint8_t board_clear_lines(board_t *board, const piece_t *piece);
//verifica si en una fila hay un bloque
static uint8_t piece_row_has_block(const piece_t *piece, uint8_t row);

//Inicializa el tablero
void board_init(board_t *board){
    for(uint8_t i = 0; i < BOARD_HEIGHT; i++){
        for(uint8_t j = 0; j < BOARD_WIDTH; j++){
            board->grid[i][j] = '.';
        }
    }
}


//verifica si la pieza cabe en el tablero
board_place_result_t board_check_placement(const board_t *board, const piece_t *piece) {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            if (get_piece(piece, i, j)) {
                int8_t x = piece->x + j;
                int8_t y = piece->y + i;

                if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
                    return BOARD_PLACE_OUT_OF_BOUNDS;
                }

                if (board->grid[y][x] == '#') {
                    return BOARD_PLACE_BLOCKED;
                }
            }
        }
    }
    return BOARD_PLACE_OK;
}

//fija una pieza en el tablero asumiendo que YA FUE VERIFICADA con board_check_placement
uint8_t board_lock_piece(board_t *board, const piece_t *piece) {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            if (get_piece(piece, i, j)) {
                int8_t x = piece->x + j;
                int8_t y = piece->y + i;

                if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                    board->grid[y][x] = '#';
                }
            }
        }
    }
    return board_clear_lines(board, piece);
}

//limpia una linea si es llenada
static uint8_t board_clear_lines(board_t *board, const piece_t *piece) {
    uint8_t cleared = 0;
    int8_t checked_rows[4];
    uint8_t checked_count = 0;

    for (uint8_t i = 0; i < 4; i++) {
        if (piece_row_has_block(piece, i)) {
            int8_t row = piece->y + i;

            if (row >= 0 && row < BOARD_HEIGHT) {
                uint8_t already_checked = 0;
                for (uint8_t r = 0; r < checked_count; r++) {
                    if (checked_rows[r] == row) {
                        already_checked = 1;
                        break;
                    }
                }

                if (!already_checked) {
                    checked_rows[checked_count++] = row;
                }
            }
        }
    }

    for (int8_t idx = checked_count - 1; idx >= 0; idx--) {
        int8_t row = checked_rows[idx];
        uint8_t full = 1;

        for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
            if (board->grid[row][col] != '#') {
                full = 0;
                break;
            }
        }

        if (full) {
            for (int8_t r = row; r > 0; r--) {
                for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
                    board->grid[r][col] = board->grid[r - 1][col];
                }
            }

            for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
                board->grid[0][col] = '.';
            }
            cleared++;
        }
    }
    return cleared;
}

//revisa si
static uint8_t piece_row_has_block(const piece_t *piece, uint8_t row) {
    for (uint8_t col = 0; col < 4; col++) {
        if (get_piece(piece, row, col)) {
            return 1;
        }
    }
    return 0;
}
