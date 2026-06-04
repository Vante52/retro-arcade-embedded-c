/**
 * @file board.h
 * @author vante52 (valkacr.dev@gmail.com) 
 * @brief Public interface for tetris board and its operations
 * @version 0.1
 * @date 2026-04-04
 * 
 * @copyright Copyright (c) 2026 Valka CR
 * 
 */
#ifndef BOARD_H
#define BOARD_H
//board width and board height in cells
#include "piece.h"
#include <stdint.h>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 16

/**
 * @brief Board status
 * 
 */
typedef struct{
    uint8_t grid[BOARD_HEIGHT][BOARD_WIDTH];
}board_t;

/**
 * @brief For error management depending if a piece fits or not on the board.
 * 
 */
typedef enum {
    BOARD_PLACE_BLOCKED = 0,
    BOARD_PLACE_OK = 1,
    BOARD_PLACE_OUT_OF_BOUNDS = 2
} board_place_result_t;

/**
 * @brief Initializes the board with its fixed height and width
 * 
 * @param[out] board Board to initialize
 */
void board_init(board_t *board);

/**
 * @brief Verifies if a piece fits on the board.
 * 
 * @param[in] board Current board status
 * @param[in] piece piece to query.
 * @return * board_place_result_t; BOARD_PLACE_OK if the piece fits, BOARD_PLACE_BLOCKED if there is a piece under the piece so it collides with occupied cells,  BOARD_PLACE_OUT_OF_BOUNDS if the piece is over the board height
 */
board_place_result_t board_check_placement (const board_t *board, const piece_t *piece);

/**
 * @brief Lock a piece on the board if its board_place_result_t fromboard_check_placement is BOARD_PLACE_BLOCKED and clear lines if the row is filled 
 * 
 * @param[in, out] board Current board
 * @param[in] piece piece to lock in the board
 * @return * uint8_t number of cleared lines
 */
uint8_t board_lock_piece(board_t *board, const piece_t *piece);
#endif