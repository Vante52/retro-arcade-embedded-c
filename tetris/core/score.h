#ifndef SCORE_H
#define SCORE_H
#include <stdint.h>

//Estructura del puntaje
typedef struct{
    uint32_t score;
}score_t;

//Inicializa el puntaje
void score_init(score_t *score);
//Suma puntos
void score_add(score_t *score, uint32_t points);
//Obtiene el puntaje
uint32_t score_get(score_t *score);

#endif