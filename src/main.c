#include "hasty_graphic.h"

#define WND_TITLE "My application"

extern uint32_t* g_back_buffer;

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;
    SDL_Event event;

    init_graphics(WND_TITLE, scr_w, scr_h);

    /* Fill back buffer one color */
    for (size_t i = 0; i < (size_t)scr_h * scr_w; i++) {
        g_back_buffer[i] = 0xff00ff00u; /* color pack - AA BB GG RR */
    }

    while (1) {

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

        redraw_screen();
    }

    shutdown_graphics();

    return EXIT_SUCCESS;
}
