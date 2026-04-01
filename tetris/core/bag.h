#ifndef BAG_H
#define BAG_H

#include <stdint.h>
#include "piece.h"

#define BAG_SIZE 7

typedef struct {
    piece_type_t pieces[BAG_SIZE];
    uint8_t index;
} piece_bag_t;


void bag_init(piece_bag_t *bag);
piece_type_t bag_next(piece_bag_t *bag);

#endif