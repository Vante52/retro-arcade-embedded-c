/**
 * @file score.h
 * @author vante52 (valkacr.dev@gmail.com)
 * @brief Public interface for Tetris score.
 * @version 0.1
 * @date 2026-04-04
 * 
 * @copyright Copyright (c) 2026 Valka CR
 * 
 */
#ifndef SCORE_H
#define SCORE_H
#include <stdint.h>

/**
 * @brief score of the game
 * 
 */
typedef struct{
    uint32_t score;
}score_t;

/**
 * @brief initialization of the score of the game
 * 
 * @param[in, out] score set the score 0
 */
void score_init(score_t *score);

/**
 * @brief add points to the score
 * 
 * @param[in, out] score current score
 * @param[in] points points to add
 */
void score_add(score_t *score, uint32_t points);

/**
 * @brief Returns the current score
 * 
 * @param[in] score  current score
 * @return * uint32_t score
 */
uint32_t score_get(const score_t *score);

/**
 * @brief add cleared lines to the score
 * 
 * @param[in, out] score current score
 * @param[in] cleared_lines cleared lines in the game
 */
void score_add_lines(score_t *score, uint8_t cleared_lines);
#endif