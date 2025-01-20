#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "hasty_graphic.h"

#define WND_TITLE "My application"

extern SDL_Window* g_wnd;
extern SDL_Renderer* g_rndr;

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;
    int pitch = sizeof(uint32_t) * scr_w;

    init_app(WND_TITLE, scr_w, scr_h);

    SDL_Texture* back_buffer_texture = SDL_CreateTexture(g_rndr, SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING, scr_w, scr_h);
    if (!back_buffer_texture) {
        ERR_SDL(SDL_CreateTexture);
        exit(EXIT_FAILURE);
    }

    uint32_t* back_buffer = malloc(sizeof(*back_buffer) * scr_h * scr_w);
    if (!back_buffer) {
        fprintf(stderr, "[%s:%d] Back buffer don\'t created\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    /* Fill back buffer one color */
    for (size_t i = 0; i < (size_t)scr_h * scr_w; i++) {
        back_buffer[i] = 0xff00ff00u; /* color pack - AA BB GG RR */
    }

    SDL_Event event;

    while (true) {

        /* Event handling */
        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT)
                break;

            if (event.type == SDL_EVENT_KEY_DOWN) {

                if (event.key.key == SDLK_ESCAPE) {
                    break;
                }
            }
        } /* End of event handling */

        SDL_UpdateTexture(back_buffer_texture, NULL, back_buffer, pitch);
        SDL_RenderClear(g_rndr);
        SDL_RenderTexture(g_rndr, back_buffer_texture, NULL, NULL);
        SDL_RenderPresent(g_rndr);
    }

    if (back_buffer) {
        free(back_buffer);
        back_buffer = NULL;
    }

    SDL_DestroyTexture(back_buffer_texture);

    shutdown_app();

    return EXIT_SUCCESS;
}
