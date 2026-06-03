/* Entry point para el build SDL2.
 * CMake compila este archivo en lugar de main.c para el target tetris_sdl. */
#include "host/sdl/main_sdl.h"

int main(void) {
    return main_sdl_init();
}