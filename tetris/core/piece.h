#ifndef PIECE_H
#define PIECE_H
#include <stdint.h>

// Definición de las piezas de mi juego

//Catálogo de piezas
typedef enum{
    PIECE_I, PIECE_O, PIECE_T, PIECE_J, PIECE_L, PIECE_S, PIECE_Z
}piece_type_t;

//La pieza tiene un tipo, una rotación, y una posición en el tablero
typedef struct{
    piece_type_t type;
    uint8_t rotation;
    int8_t x;
    int8_t y;
}piece_t;

//Inicializa una pieza mandando el self pointer
void piece_init(piece_t *self, piece_type_t type, int8_t x, int8_t y);
//Rota una pieza
void piece_rotate(piece_t *self);
//Mueve una pieza
void piece_move(piece_t *self, int8_t dx, int8_t dy);
//getter pieza
uint8_t get_piece(const piece_t *self, int8_t row, int8_t col);
#endif