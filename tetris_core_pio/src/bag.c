#include "bag.h"

//Pseudo Random Number Generator
static uint32_t prng_state = 0xA341316Cu;

//Advances the internar xorshift PRNG state
static uint32_t prng_next(void) {
    uint32_t x = prng_state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    prng_state = x;
    return x;
}

//swap positions of two pieces inside the array
static void bag_swap(piece_type_t *a, piece_type_t *b) {
    piece_type_t temp = *a;
    *a = *b;
    *b = temp;
}

//Shuffle the indexes inside the bag
static void bag_shuffle(piece_bag_t *bag) {
    for (int8_t i = BAG_SIZE - 1; i > 0; i--) {
        uint8_t j = (uint8_t)(prng_next() % (uint32_t)(i + 1));

        bag_swap(&bag->pieces[i], &bag->pieces[j]);
    }
}
//Initialice the bag with the block catalog
void bag_init(piece_bag_t *bag) {
    bag->pieces[0] = PIECE_I;
    bag->pieces[1] = PIECE_O;
    bag->pieces[2] = PIECE_T;
    bag->pieces[3] = PIECE_J;
    bag->pieces[4] = PIECE_L;
    bag->pieces[5] = PIECE_S;
    bag->pieces[6] = PIECE_Z;

    bag_shuffle(bag);
    bag->index = 0u;
}

// When the bag is exhausted, it is reinitialized and shuffled again.
piece_type_t bag_next(piece_bag_t *bag) {
    if (bag->index >= BAG_SIZE) {
        bag_init(bag);
    }

    return bag->pieces[bag->index++];
}