#include "core/game.h"

int main(){
    game_t game;
    game_init(&game);
    game_draw(&game);
    game_update(&game);
    game_input(&game);
    return 0;
}