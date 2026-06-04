#include "fsm.h"

#include <stddef.h>

//mi máquina de estados finitos que ahora será privada
typedef struct {
    fsm_state_t next_state;
    //puntero a la función condición que me lleva al estado, o sea los punteros al guard
    uint8_t (*guard)(const game_t *game);
    //puntero a la función que determina qué acción ejecuto si se cumple?, o sea mi puntero al action
    void (*action)(game_t *game);
} fsm_transition_t;

//Como puedo tener varias transiciones dependiendo el resultado de mi guard. Tengo una celda de transiciones
typedef struct {
    const fsm_transition_t *transitions;
    uint8_t count;
} fsm_cell_t;

//inicialización de mi maquina de estados
void fsm_init(fsm_t *fsm) {
    fsm ->state = FSM_STATE_SPAWN;
}
//-------------------GUARDS-----------------

//spawn
static uint8_t guard_can_spawn(const game_t *game);
static uint8_t guard_cannot_spawn(const game_t *game);

//movimiento

//para abajo o sea también caer
static uint8_t guard_cannot_fall(const game_t *game);
static uint8_t guard_can_fall(const game_t *game);
//rotar
static uint8_t guard_can_rotate(const game_t *game);
//left
static uint8_t guard_can_move_left(const game_t *game);
//right
static uint8_t guard_can_move_right(const game_t *game);

//--------------------------ACCIONES--------------------
//spawn
static void act_spawn_piece(game_t *game);

//movimiento
// rota la pieza
static void act_rotate_piece(game_t *game);
//a la izquierda
static void act_move_piece_left(game_t *game);
//a la derecha
static void act_move_piece_right(game_t *game);
//para abajo
static void act_move_piece_down(game_t *game);
//fija la pieza
static void act_lock_piece(game_t *game);

//ciclo de vida del juego
static void act_game_over(game_t *game);

//------------------TABLA DE TRANSICIONES-----------------
static const fsm_transition_t spawn_tick_transitions[] = {
    { FSM_STATE_FALLING,  guard_can_spawn,    act_spawn_piece },
    { FSM_STATE_GAME_OVER, guard_cannot_spawn, act_game_over }
};

static const fsm_transition_t falling_tick_transitions[] = {
    { FSM_STATE_FALLING, guard_can_fall,    act_move_piece_down },
    { FSM_STATE_LOCK,    guard_cannot_fall, act_lock_piece}
};

static const fsm_transition_t move_left_transitions[] = {
    { FSM_STATE_FALLING, guard_can_move_left, act_move_piece_left }
};

static const fsm_transition_t move_right_transitions[] = {
    { FSM_STATE_FALLING, guard_can_move_right, act_move_piece_right }
};

static const fsm_transition_t rotate_transitions[] = {
    { FSM_STATE_FALLING, guard_can_rotate, act_rotate_piece }
};

static const fsm_transition_t lock_tick_transitions[] = {
    { FSM_STATE_SPAWN, NULL, NULL}
};

static const fsm_transition_t move_down_transitions[] = {
    {FSM_STATE_FALLING, guard_can_fall, act_move_piece_down}
};

static const fsm_cell_t fsm_table[FSM_STATE_COUNT][FSM_EVENT_COUNT] = {
    [FSM_STATE_SPAWN] = {
        [FSM_EVENT_TICK] = { spawn_tick_transitions, 2 }
    },
    [FSM_STATE_FALLING] = {
        [FSM_EVENT_TICK] = { falling_tick_transitions, 2 },
        [FSM_EVENT_MOVE_LEFT]  = { move_left_transitions, 1 },
        [FSM_EVENT_MOVE_RIGHT] = { move_right_transitions, 1 },
        [FSM_EVENT_ROTATE] = { rotate_transitions, 1 },
        [FSM_EVENT_DOWN] = {move_down_transitions,1}
    },
    [FSM_STATE_LOCK] = {
        [FSM_EVENT_TICK] = { lock_tick_transitions, 1 }
    },
    [FSM_STATE_GAME_OVER] = {
        [FSM_EVENT_INPUT] = {spawn_tick_transitions, 2}
    }
};


//Flujo de la maquina de estados
void fsm_dispatch(fsm_t *fsm, game_t *game, fsm_event_t event) {
    if (fsm == NULL || game == NULL) {
        return;
    }
    if (fsm->state >= FSM_STATE_COUNT || event >= FSM_EVENT_COUNT) {
        return;
    }
    const fsm_cell_t *cell = &fsm_table[fsm->state][event];

    if (cell->count == 0 || cell->transitions == NULL) {
        return;
    }

    for (uint8_t i = 0; i < cell->count; i++) {
        const fsm_transition_t *t = &cell->transitions[i];

        if (t->guard == NULL || t->guard(game)) {
            if (t->action != NULL) {
                t->action(game);
            }

            fsm->state = t->next_state;
            return;
        }
    }
}

static uint8_t guard_can_spawn(const game_t *game) {
    return game_can_spawn_piece(game);
}

static uint8_t guard_cannot_spawn(const game_t *game) {
    return !game_can_spawn_piece(game);
}

static uint8_t guard_can_move_left(const game_t *game) {
    return game_can_move(game, -1, 0);
}

static uint8_t guard_can_move_right(const game_t *game) {
    return game_can_move(game, 1, 0);
}

static uint8_t guard_can_rotate(const game_t *game) {
    return game_can_rotate(game);
}

static uint8_t guard_can_fall(const game_t *game) {
    return game_can_fall(game);
}

static uint8_t guard_cannot_fall(const game_t *game) {
    return !game_can_fall(game);
}
//hace aparecer la pieza
static void act_spawn_piece(game_t *game) {
    game_spawn_piece(game);
}
//mueve izquierda
static void act_move_piece_left(game_t *game) {
    game_move_piece(game, -1, 0);
}
//mueve derecha
static void act_move_piece_right(game_t *game) {
    game_move_piece(game, 1, 0);
}
//gira
static void act_rotate_piece(game_t *game) {
    game_rotate_piece(game);
}
//para abajo
static void act_move_piece_down(game_t *game) {
    game_fall_piece(game);
}
//fija
static void act_lock_piece(game_t *game) {
    game_lock_piece(game);
}
//acaba el juego
static void act_game_over(game_t *game) {
    game_enter_game_over(game);
}
