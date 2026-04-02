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
uint32_t score_get(const score_t *score);
//acumula el puntaje dependiendo de las lineas
void score_add_lines(score_t *score, uint8_t cleared_lines);
#endif