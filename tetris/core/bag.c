#include "bag.h"

//semilla de la aleatoriedad
static uint32_t prng_state = 0xA341316Cu;

//Va haciendo xorshift sobre la semilla para que haya aleatoriedad siempre
static uint32_t prng_next(void) {
    uint32_t x = prng_state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    prng_state = x;
    return x;
}

//Intercambio de dos piezas
static void bag_swap(piece_type_t *a, piece_type_t *b) {
    piece_type_t temp = *a;
    *a = *b;
    *b = temp;
}
//Revuelvo los indices de la bolsa
static void bag_shuffle(piece_bag_t *bag) {
    for (int8_t i = BAG_SIZE - 1; i > 0; i--) {
        uint8_t j = (uint8_t)(prng_next() % (uint32_t)(i + 1));

        bag_swap(&bag->pieces[i], &bag->pieces[j]);
    }
}
//Inicializo la bolsa con las figuras que tengo en el catalogo
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

//saca la siguiente pieza y si se acabo, vuelve a llenar y a mezclar
piece_type_t bag_next(piece_bag_t *bag) {
    if (bag->index >= BAG_SIZE) {
        bag_init(bag);
    }

    return bag->pieces[bag->index++];
}