#include "hasty_graphic.h"

#define WND_TITLE "My application"

extern uint32_t* g_back_buffer;

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;
    SDL_Event event;

    init_graphics(WND_TITLE, scr_w, scr_h);

    /* Draw line */
    int x0 = 50, y0 = 20; /* start point of line */
    int x1 = 200, y1 = 300; /* end point of line */

    /* Calculate slope of line (m) */
    int dx = x1 - x0;
    int dy = y1 - y0;
    int m = dy/(float)dx; /* m = dy / dx */
    
    /* Start draw by first point */
    int x = x0; int y = y0;
    /* For every x - calculate y, and draw */
    for (int i = 0; i < x1 - x0; i++) {
        set_pixel(x, y, BLUE);
        x++;
        y += m;
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
                if (event.key.key == SDLK_B) {
                    clear_back_buffer(BLUE);
                }
                if (event.key.key == SDLK_G) {
                    clear_back_buffer(GREEN);
                }
                if (event.key.key == SDLK_R) {
                    uint32_t color_red = pack_color_rgb(0xff, 0x00, 0x00);
                    clear_back_buffer(color_red);
                }
            }
        } /* End of event handling */

        redraw_screen();
    }

    shutdown_graphics();

    return EXIT_SUCCESS;
}
