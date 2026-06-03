#include "sdl_proxy.h"
#include <stdio.h>

int sdl_proxy_init(sdl_proxy_t *p, const char *title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return -1;
    }

    p->window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_SHOWN
    );
    if (!p->window) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    /* SDL_RENDERER_PRESENTVSYNC → sincroniza con el monitor, limita a ~60fps */
    p->renderer = SDL_CreateRenderer(
        p->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!p->renderer) {
        SDL_DestroyWindow(p->window);
        SDL_Quit();
        return -1;
    }

    /* Escalado de píxeles nearest-neighbor para look pixel-art */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    p->should_quit = 0;
    p->last_key    = SDLK_UNKNOWN;
    return 0;
}

void sdl_proxy_poll(sdl_proxy_t *p) {
    SDL_Event e;
    p->last_key = SDLK_UNKNOWN;   /* reset cada frame */

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                p->should_quit = 1;
                break;
            case SDL_KEYDOWN:
                if (e.key.repeat) break;
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    p->should_quit = 1;
                else
                    p->last_key = e.key.keysym.sym;
                break;
            default:
                break;
        }
    }
}

void sdl_proxy_shutdown(sdl_proxy_t *p) {
    if (p->renderer) SDL_DestroyRenderer(p->renderer);
    if (p->window)   SDL_DestroyWindow(p->window);
    SDL_Quit();
}