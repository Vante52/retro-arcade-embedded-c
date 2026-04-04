/**
 * @file game.h
 * @author vante52 (valkacr.dev@gmail.com)
 * @brief Public opaque singleton interface for tetris game.
 * @version 0.1
 * @date 2026-04-04
 * 
 * @copyright Copyright (c) 2026 Valkacr
 * 
 */

#ifndef GAME_H
#define GAME_H
#include "piece.h"


typedef struct game game_t;


/**
 * @brief returns the game with its status acting as a singleton object
 * 
 * @return * game_t* game status
 */
game_t *game_get_instance(void);

/**
 * @brief initialize the game initializing theboard, bag and score
 * 
 * @param[in, out] game 
 */
void game_init(game_t *game);

/**
 * @brief initializes the next piece and check if the piece fits ine board 
 * 
 * @param[in] game current game status
 * @return * uint8_t 1 if the piece fits, else 0
 */
uint8_t game_can_spawn_piece(const game_t *game);

/**
 * @brief Spawn the piece initialized on the board
 * 
 * @param[in,out] game current game and update the next type of piece from the bag
 */
void game_spawn_piece(game_t *game);

/**
 * @brief check if the movement that the piece is trying to do is legal
 * 
 * @param[in] game current state of the game with the position of the pieces on the board
 * @param[in] dx x position that the piece wants to move on the board
 * @param[in] dy y position that the piece wants to move on the board
 * @return uint8_t returns 1 if there is nothing on the way of the piece's trayectory, else return 0
 */
uint8_t game_can_move(const game_t *game, int8_t dx, int8_t dy);

/**
 * @brief Move the piece the distance sent by parameter
 * 
 * @param[in, out] game current state of the game with the position of the pieces on the board
 * @param[in] dx x position that the piece wants to move on the board
 * @param[in] dy y position that the piece wants to move on the board
 */
void game_move_piece(game_t *game, int8_t dx, int8_t dy);

/**
 * @brief check if the piece fits in the board after rotation
 * 
 * @param[in] game current state of the game
 * @return * uint8_t returns 1 if there is nothing on the way of the piece's trayectory, else return 0
 */
uint8_t game_can_rotate(const game_t *game);

/**
 * @brief rotates the current piece on the board
 * 
 * @param[in, out] game rotates the current piece on the board
 */
void game_rotate_piece(game_t *game);

/**
 * @brief check if the piece can fall
 * 
 * @param[in] game current state of the game
 * @return * uint8_t 1 if the piece doesn't collide with nothing in the fall
 */
uint8_t game_can_fall(const game_t *game);

/**
 * @brief move 1 row down the current piece on the board
 * 
 * @param[in, out] game current state of the game
 */
void game_fall_piece(game_t *game);

/**
 * @brief lock the piece on the board
 * 
 * @param[in, out] game current state of the game
 */
void game_lock_piece(game_t *game);

/**
 * @brief Returns the cuantity of lines cleared after locking a piece
 * 
 * @param[in] game current state of the game
 * @return * uint8_t number of rows cleares
 */
uint8_t game_last_cleared_lines(const game_t *game);

/**
 * @brief Ends the game
 * 
 * @param[in, out] game sets current game, game over
 */
void game_enter_game_over(game_t *game);

/**
 * @brief Check if game is over
 * 
 * @param[in] game current status of the game
 * @return * uint8_t return 1 if game is over, else 0
 */
uint8_t game_is_game_over(const game_t *game);

/**
 * @brief Returns what is on the board on the asked position
 * 
 * @param[in] game current state of the game
 * @param[in] x position x 
 * @param[in] y position y
 * @return * uint8_t returns 1 if there is a piece in that position
 */
uint8_t game_board_cell(const game_t *game, uint8_t x, uint8_t y);

/**
 * @brief Returns the 4x4 cell of the active piece on the column and row asked
 * 
 * @param[in] game current status of the game
 * @param[in] row row where the piece is going to be searched
 * @param[in] col column where the piece is going to be searched
 * @return * uint8_t returns the number of the piece shape
 */
uint8_t game_current_piece_cell(const game_t *game, uint8_t row, uint8_t col);

/**
 * @brief Returns the x position of the current active piece
 * 
 * @param[in] game current game state
 * @return * int8_t x position of the current piece
 */
int8_t game_current_piece_x(const game_t *game);


/**
 * @brief Returns the y position of the current active piece
 * 
 * @param[in] game current game state
 * @return * int8_t y position of the current piece
 */
int8_t game_current_piece_y(const game_t *game);

/**
 * @brief returns the next piece on the bag
 * 
 * @param[in] game current status of the game
 * @return * piece_type_t next piece type on the bag
 */
piece_type_t game_next_type(const game_t *game);

/**
 * @brief retuns the score of the game
 * 
 * @param[in] game current status of the game
 * @return * uint32_t current score
 */
uint32_t game_score(const game_t *game);

#endif