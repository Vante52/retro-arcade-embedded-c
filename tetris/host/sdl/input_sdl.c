#include "input_sdl.h"
#include <SDL2/SDL.h>

static void sdl_input_init(void *ctx)     { (void)ctx; }
static void sdl_input_shutdown(void *ctx) { (void)ctx; }

static input_cmd_t sdl_input_poll(void *ctx) {
    sdl_input_t *in = (sdl_input_t *)ctx;
    SDL_Keycode  k  = in->proxy->last_key;

    switch (k) {
        case SDLK_LEFT:  case SDLK_a:  return INPUT_CMD_LEFT;
        case SDLK_RIGHT: case SDLK_d:  return INPUT_CMD_RIGHT;
        case SDLK_DOWN:  case SDLK_s:  return INPUT_CMD_DOWN;
        case SDLK_UP:    case SDLK_w:
        case SDLK_SPACE:               return INPUT_CMD_ROTATE;
        case SDLK_q:                   return INPUT_CMD_QUIT;
        default:                       return INPUT_CMD_NONE;
    }
}

input_port_t sdl_input_create(sdl_input_t *in, sdl_proxy_t *proxy) {
    in->proxy = proxy;

    input_port_t port;
    port.init     = sdl_input_init;
    port.poll     = sdl_input_poll;
    port.shutdown = sdl_input_shutdown;
    port.ctx      = in;
    return port;
}