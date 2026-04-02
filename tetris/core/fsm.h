#ifndef FSM_H
#define FSM_H
#include "game.h"

//estados que puede tomar mi juego
typedef enum {
    FSM_STATE_SPAWN=0,
    FSM_STATE_FALLING,
    FSM_STATE_LOCK,
    FSM_STATE_GAME_OVER,
    FSM_STATE_COUNT
} fsm_state_t;

//eventos a los que mi juego debe responder
typedef enum {
    FSM_EVENT_NONE=0,
    FSM_EVENT_TICK,
    FSM_EVENT_MOVE_LEFT,
    FSM_EVENT_MOVE_RIGHT,
    FSM_EVENT_ROTATE,
    FSM_EVENT_DOWN,
    FSM_EVENT_INPUT,
    FSM_EVENT_COUNT,
} fsm_event_t;

typedef struct {
    fsm_state_t state;
}fsm_t;

//Inicialización de mi maquina de estados
void fsm_init(fsm_t *fsm);

//Función que me mueve entre estados
void fsm_dispatch(fsm_t *fsm, game_t *game, fsm_event_t event);

#endif
