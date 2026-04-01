#include "fsm.h"

#include <stdio.h>

//-------------------GUARDS-----------------

//spawn
static uint8_t guard_can_spawn(const game_t *game);
static uint8_t guard_out_of_range(const game_t *game);

//movimiento
static uint8_t guard_cannot_fall(const game_t *game);
static uint8_t guard_can_rotate(const game_t *game);
static uint8_t guard_cannot_move(const game_t *game);
static uint8_t guard_can_move_left(const game_t *game);
static uint8_t guard_can_move_right(const game_t *game);
static uint8_t guard_can_move_down(const game_t *game);

//limpiar
static uint8_t guard_fill_lines(const game_t *game) ;

//--------------------------ACCIONES--------------------
//spawn
static void act_spawn_piece(game_t *game);

//movimiento
static void act_rotate_piece(game_t *game);
static void act_move_piece_left(game_t *game);
static void act_move_piece_right(game_t *game);
static void act_move_piece_down(game_t *game);
static void act_lock_piece(game_t *game);

//limpieza
static void act_clear_line(game_t *game) ;

//ciclo de vida del juego
static void act_game_over(game_t *game);

//Prototipos extra
//Revisar si la pieza se puede colocar en el tablero
static uint8_t check_piece_after_transform (const game_t *game, int8_t dx, int8_t dy, int8_t rotate);

//------------------TABLA DE TRANSICIONES-----------------
static const fsm_transition_t fsm_spawn_transitions[]={
    {FSM_STATE_FALLING, guard_can_move_down, act_move_piece_down},
    {FSM_STATE_GAME_OVER, guard_out_of_range, act_game_over}
};

static const fsm_transition_t fsm_fall_transitions[]={
    {FSM_STATE_FALLING, guard_can_move_down, act_move_piece_down},
    {FSM_STATE_LOCK, guard_cannot_fall, act_lock_piece}
};

static const fsm_transition_t fsm_move_left_transitions[]={
    {FSM_STATE_FALLING, guard_can_move_left, act_move_piece_left},
    {FSM_STATE_LOCK, guard_cannot_move, act_lock_piece}
};

static const fsm_transition_t fsm_move_right_transitions[]={
    {FSM_STATE_FALLING, guard_can_move_left, act_move_piece_right},
    {FSM_STATE_LOCK, guard_cannot_move, act_lock_piece},
};

static const fsm_transition_t fsm_move_down_transitions[]={
    {FSM_STATE_FALLING, guard_can_move_down, act_move_piece_down},
    {FSM_STATE_LOCK, guard_cannot_move, act_lock_piece}
};

static const fsm_transition_t fsm_rotate_transitions[]={
    {FSM_STATE_FALLING, guard_can_rotate, act_rotate_piece}
};

static const fsm_transition_t fsm_start_transitions[]={
    {FSM_STATE_SPAWN, guard_can_spawn, act_spawn_piece}
};

static const fsm_transition_t fsm_clear_transitions[]={
    {FSM_STATE_CLEAR_LINES, guard_fill_lines, act_clear_line}
};

//celda de transiciones
static const fsm_cell_t fsm_spawn[8][8]={
    [FSM_STATE_SPAWN] = {
        [FSM_EVENT_TICK] = {fsm_spawn_transitions, 2}
    },
    [FSM_STATE_FALLING] = {
        [FSM_EVENT_TICK] = {fsm_fall_transitions, 2}
    },
    [FSM_STATE_FALLING] = {
        [FSM_EVENT_MOVE_LEFT] = {fsm_move_left_transitions, 2}
    },
    [FSM_STATE_FALLING] = {
        [FSM_EVENT_MOVE_RIGHT] = {fsm_move_right_transitions, 2}
    },
    [FSM_STATE_FALLING] = {
        [FSM_EVENT_DOWN] = {fsm_move_down_transitions, 2}
    },
    [FSM_STATE_FALLING] = {
        [FSM_EVENT_ROTATE] = {fsm_rotate_transitions, 1}
    },
    [FSM_STATE_LOCK] = {
        [FSM_EVENT_TICK] = {fsm_clear_transitions, 1}
    },
    [FSM_STATE_GAME_OVER] = {
        [FSM_EVENT_ROTATE] = {fsm_start_transitions, 1}
    }

};


//Flujo de la maquina de estados
void fsm_dispatch(fsm_t *fsm, game_t *game, fsm_event_t event) {
    const fsm_cell_t *cell = &fsm_spawn[fsm->state][event];
    for (uint8_t i = 0; i < cell->count; i++) {
        const fsm_transition_t *t = &cell->transitions[i];

        if (t->guard == NULL || t->guard(game)) {
            if (t->action != NULL) {
                t->action(game);
            }
            // si no se cumple ningún guard, entonces me quedo en el mismo estado
            fsm->state = t->next_state;
            return;
        }
    }
}

//spawn
static uint8_t guard_can_spawn(const game_t *game) {
    piece_t temp;
    piece_type_t type = game->next_type;   /* o bag_peek(), no bag_next() */
    piece_init(&temp, type, BOARD_WIDTH / 2, 0);

    return board_check_placement(&game->board, &temp) == BOARD_PLACE_OK;
}
static void act_spawn_piece(game_t *game) {
    game_spawn_piece(game);
    game->next_type = bag_next(&game->bag);
}

//caida
static uint8_t guard_cannot_fall(const game_t *game) {
    return !check_piece_after_transform(game, 0, 1, 0);
}
static void act_lock_piece(game_t *game) {
    game_fall(game);
}

//Movimiento
static uint8_t guard_can_move_left(const game_t *game) {
    return check_piece_after_transform(game, -1, 0, 0);
}

static uint8_t guard_can_move_right(const game_t *game) {
    return check_piece_after_transform(game, 1, 0, 0);
}

static uint8_t guard_can_move_down(const game_t *game) {
    return check_piece_after_transform(game, 0, -1, 0);
}

static uint8_t guard_can_rotate(const game_t *game) {
    return check_piece_after_transform(game, 0, 0, 1);
}
static void act_move_piece_left(game_t* game) {
    game_move_piece(game, -1,0);
}
static void act_move_piece_right(game_t* game) {
    game_move_piece(game, 1,0);
}
static void act_move_piece_down(game_t* game) {
    game_move_piece(game,0,-1);
}
static void act_rotate_piece(game_t* game) {
    game_rotate_piece(game);
}


//limpiar
static uint8_t guard_fill_lines(const game_t *game) {
    return game_line_filled(game);
}
static void act_clear_line(game_t *game) {
    game_clear_line(game);
}


//Ciclo de vida del programa
static void act_game_over(game_t *game) {
    game_over(game);
}

//Auxiliares
static uint8_t check_piece_after_transform (const game_t *game, int8_t dx, int8_t dy, int8_t rotate) {
    piece_t temp = game ->current_piece;
    if (rotate) {
        piece_rotate(&temp);
    }
    piece_move(&temp, dx, dy);
    return board_check_placement(&game->board, &temp);
}