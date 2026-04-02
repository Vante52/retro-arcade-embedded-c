#include "score.h"
#include <stdint.h>

//Inicializa el puntaje
void score_init(score_t *score){
    score->score = 0;
}
//Suma puntos
void score_add(score_t *score, uint32_t points){
    score->score += points;
}

//Obtiene el puntaje
uint32_t score_get(const score_t *score){
    return score->score;
}

void score_add_lines(score_t *score, uint8_t cleared_lines) {
    score->score += cleared_lines*100;
}