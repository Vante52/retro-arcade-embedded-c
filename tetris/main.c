#include "fsm.h"
#include "core/game.h"

int main(){
    game_t *game = game_get_instance();
    fsm_t fsm;
    game_init(game);
    fsm_init(&fsm);

    return 0;
}