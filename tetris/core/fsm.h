#ifndef FSM_H
#define FSM_H
#include "game.h"

//estados que puede tomar mi juego
typedef enum {
    FSM_STATE_SPAWN=0,
    FSM_STATE_FALLING,
    FSM_STATE_LOCK,
    FSM_STATE_CLEAR_LINES,
    FSM_STATE_GAME_OVER,
} fsm_state_t;

//eventos a los que mi juego debe responder
typedef enum {
    FSM_EVENT_NONE=0,
    FSM_EVENT_TICK,
    FSM_EVENT_MOVE_LEFT,
    FSM_EVENT_MOVE_RIGHT,
    FSM_EVENT_ROTATE,
    FSM_EVENT_DOWN,
} fsm_event_t;

//mi máquina de estados finitos
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

typedef struct {
    fsm_state_t state;
}fsm_t;

//Inicialización de mi maquina de estados
void fsm_init(fsm_t *fsm);

//Función que me mueve entre estados
void fsm_dispach(fsm_t *fsm, game_t *game, fsm_event_t event);

#endif
