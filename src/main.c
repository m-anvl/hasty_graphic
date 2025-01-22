#include "hasty_graphic.h"

#define WND_TITLE "My application"

extern uint32_t* g_back_buffer;

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;
    SDL_Event event;

    init_graphics(WND_TITLE, scr_w, scr_h);

    uint32_t v_color;

    /* Pack the color A8B8G8*/
    uint8_t red = 0xff;
    uint8_t green = 0x00;
    uint8_t blue = 0xff;
    uint8_t alpha = 0xff;

    v_color = (alpha << 24) + (blue << 16) + (green << 8) + red;

    clear_back_buffer(v_color);

    set_pixel(scr_w / 2, scr_h / 2, BLACK);

    /* Draw line */
    for (int i = 30; i < 80; i++) {
        set_pixel(i, 26, YELLOW);
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
