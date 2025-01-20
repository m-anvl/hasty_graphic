#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "hasty_graphic.h"

#define WND_TITLE "My application"

extern SDL_Renderer* g_rndr;
extern SDL_Texture* g_back_buffer_texture;
extern uint32_t* g_back_buffer;

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;
    int pitch = sizeof(uint32_t) * scr_w;

    init_app(WND_TITLE, scr_w, scr_h);

    /* Fill back buffer one color */
    for (size_t i = 0; i < (size_t)scr_h * scr_w; i++) {
        g_back_buffer[i] = 0xff00ff00u; /* color pack - AA BB GG RR */
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

        SDL_UpdateTexture(g_back_buffer_texture, NULL, g_back_buffer, pitch);
        SDL_RenderClear(g_rndr);
        SDL_RenderTexture(g_rndr, g_back_buffer_texture, NULL, NULL);
        SDL_RenderPresent(g_rndr);
    }

    shutdown_app();

    return EXIT_SUCCESS;
}
