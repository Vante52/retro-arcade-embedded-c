/**
 * @file bag.h
 * @author vante52 (valkacr.dev@gmail.com)
 * @brief Public interface for Tetris piece bag.
 * @version 0.1
 * @date 2026-04-04
 * 
 * @copyright Copyright (c) 2026 Valka CR
 * 
 */
#ifndef BAG_H
#define BAG_H

#include <stdint.h>
#include "piece.h"
//The size of the bag is 7 since there are 7 different piece shapes
#define BAG_SIZE 7

/**
 * @brief array of pieces
 * 
 */
typedef struct {
    piece_type_t pieces[BAG_SIZE];
    uint8_t index;
} piece_bag_t;

/**
 * @brief Initializes the bag filling it with pieces randomly
 * 
 * @param[in, out] bag array of pieces
 */
void bag_init(piece_bag_t *bag);

/**
 * @brief Returns the next piece from the bag
 * 
 * @param[in] bag array of pieces
 * @return * piece_type_t Type of the next piece
 */
piece_type_t bag_next(piece_bag_t *bag);

#endif