#ifndef SDL_PROXY_H
#define SDL_PROXY_H

#include <SDL2/SDL.h>
#include <stdint.h>

/* ----------------------------------------------------------------
 * Hardware Proxy para SDL2.
 * Encapsula los detalles de SDL_Window, SDL_Renderer y la cola de
 * eventos.  El resto del código host nunca incluye <SDL2/SDL.h>
 * directamente — solo usa este proxy.
 * ---------------------------------------------------------------- */

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    uint8_t       should_quit;   /* SDL_QUIT o tecla ESC */
    SDL_Keycode   last_key;      /* última tecla presionada este frame */
} sdl_proxy_t;

/* Crea ventana y renderer.  Retorna 0 en éxito. */
int  sdl_proxy_init(sdl_proxy_t *p, const char *title, int w, int h);

/* Procesa todos los eventos pendientes de SDL en este frame.
 * Debe llamarse UNA vez al inicio de cada iteración del loop. */
void sdl_proxy_poll(sdl_proxy_t *p);

/* Destruye renderer, ventana y llama SDL_Quit. */
void sdl_proxy_shutdown(sdl_proxy_t *p);

#endif /* SDL_PROXY_H */