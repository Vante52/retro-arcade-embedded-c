#include "board.h"
#include <stdint.h>


//clear a line if its filled assuming that board_check_placement has already been called
static uint8_t board_clear_lines(board_t *board, const piece_t *piece);
//verify if there is a piece in that row
static uint8_t piece_row_has_block(const piece_t *piece, uint8_t row);

void board_init(board_t *board){
    for(uint8_t i = 0; i < BOARD_HEIGHT; i++){
        for(uint8_t j = 0; j < BOARD_WIDTH; j++){
            board->grid[i][j] = 0u;
        }
    }
}


board_place_result_t board_check_placement(const board_t *board, const piece_t *piece) {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            if (get_piece(piece, i, j)) {
                int8_t x = piece->x + j;
                int8_t y = piece->y + i;

                if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
                    return BOARD_PLACE_OUT_OF_BOUNDS;
                }

                if (board->grid[y][x] != 0u) {
                    return BOARD_PLACE_BLOCKED;
                }
            }
        }
    }
    return BOARD_PLACE_OK;
}

uint8_t board_lock_piece(board_t *board, const piece_t *piece) {
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            if (get_piece(piece, i, j)) {
                int8_t x = piece->x + (int8_t)j;
                int8_t y = piece->y + (int8_t)i;

                if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                    board->grid[y][x] = (uint8_t)(piece->type + 1u);
                }
            }
        }
    }
    return board_clear_lines(board, piece);
}

//clear a line if the row is filled
static uint8_t board_clear_lines(board_t *board, const piece_t *piece) {
    (void)piece;  // ya no hace falta usarla aquí

    uint8_t cleared = 0;

    for (int row = BOARD_HEIGHT - 1; row >= 0; row--) {
        uint8_t full = 1;

        for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
            if (board->grid[row][col] == 0u) {
                full = 0;
                break;
            }
        }

        if (full) {
            // bajar todo lo de arriba una fila
            for (int r = row; r > 0; r--) {
                for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
                    board->grid[r][col] = board->grid[r - 1][col];
                }
            }

            // limpiar la fila superior
            for (uint8_t col = 0; col < BOARD_WIDTH; col++) {
                board->grid[0][col] = 0u;
            }

            cleared++;

            // volver a revisar esta misma fila,
            // porque ahora cayó una nueva aquí
            row++;
        }
    }

    return cleared;
}

//check if there is a block in this row
static uint8_t piece_row_has_block(const piece_t *piece, uint8_t row) {
    for (uint8_t col = 0; col < 4; col++) {
        if (get_piece(piece, row, col)) {
            return 1;
        }
    }
    return 0;
}
