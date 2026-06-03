#ifndef INPUT_SDL_H
#define INPUT_SDL_H

#include "sdl_proxy.h"
#include "../../platform/input_port.h"

/* ----------------------------------------------------------------
 * El adapter de input SDL no necesita estado propio:
 * lee directamente del proxy compartido.
 * ---------------------------------------------------------------- */
typedef struct {
    sdl_proxy_t *proxy;
} sdl_input_t;

input_port_t sdl_input_create(sdl_input_t *in, sdl_proxy_t *proxy);

#endif /* INPUT_SDL_H */