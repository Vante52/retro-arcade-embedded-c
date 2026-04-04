/**
 * @file piece.h
 * @author vante52 valkacr.dev@gmail.com
 * @brief Public interface for Tetris pieces and piece operations.
 * @version 0.1
 * @date 2026-04-04
 * 
 * @copyright Copyright (c) 2026 Valka CR
 * 
 */
#ifndef PIECE_H
#define PIECE_H
#include <stdint.h>

/**
 * @brief Block catalog
 * 
 */
typedef enum{
    PIECE_I, PIECE_O, PIECE_T, PIECE_J, PIECE_L, PIECE_S, PIECE_Z
}piece_type_t;

/**
 * @brief Represents a Tetris piece instance on the board.
 */
typedef struct {
    piece_type_t type;   /**< Piece shape type. */
    uint8_t rotation;    /**< Rotation state in the range [0, 3]. */
    int8_t x;            /**< X position on the board. */
    int8_t y;            /**< Y position on the board. */
} piece_t;

/**
 * @brief Initializes a piece with type, position, and default rotation.
 * @param[out] self Piece to initialize.
 * @param[in] type Tetris piece type.
 * @param[in] x Initial x position on the board.
 * @param[in] y Initial y position on the board.
 */
void piece_init(piece_t *self, piece_type_t type, int8_t x, int8_t y);

/**
 * @brief Rotates the piece to its next orientation.
 *
 * Advances the rotation state cyclically in the range [0, 3].
 *
 * @param[in,out] self Piece to rotate.
 */
void piece_rotate(piece_t *self);

/**
 * @brief Move the piece sent by parameter
 * 
 * @param[in, out] self Piece to modify
 * @param[in] dx New x position on the board
 * @param[in] dy New y position on the board
 */
void piece_move(piece_t *self, int8_t dx, int8_t dy);

/**
 * @brief Returns the occupancy value of a cell in the piece's 4x4 shape matrix.
 *
 * @param[in] self Piece to query.
 * @param[in] row Row inside the local 4x4 matrix. Valid range: 0 to 3.
 * @param[in] col Column inside the local 4x4 matrix. Valid range: 0 to 3.
 * @return 1 if the cell is occupied; 0 otherwise.
 */
uint8_t get_piece(const piece_t *self, int8_t row, int8_t col);
#endif