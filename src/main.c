#include "hasty_graphic.h"

#define WND_TITLE "My application"

extern uint32_t* g_back_buffer;

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;
    SDL_Event event;

    init_graphics(WND_TITLE, scr_w, scr_h);

    /* Bresenham's line algorithm */
    int x0 = 50, y0 = 20; /* start point of line */
    int x1 = 200, y1 = 400; /* end point of line */
    uint32_t color_line = pack_color_rgb(0x00u, 0x00u, 0xffu);
    int dx, dy; /* deltas */
    int dx2, dy2; /* (dx, dy)x2 */
    int x_dir, y_dir; /* steps */
    int err_flag;

    /* Calc start address in back buffer (for draw start point) */
    uint32_t* bbuf_start = g_back_buffer + x0 + y0 * scr_w;
    /* Calc deltas */
    dx = x1 - x0;
    dy = y1 - y0;

    if (dx >= 0) {
        x_dir = 1; /* Line direction - right */
    }
    else {
        x_dir = -1; /* left */
        dx = -dx; /* need abs value */
    }

    if (dy >= 0) {
        y_dir = scr_w; /* down */
    }
    else {
        y_dir = -scr_w; /* up */
        dy = -dy;
    }

    /* (dx, dy)x2 */
    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy) {
        err_flag = dy2 - dx;

        /* Draw line */
        for (int i = 0; i <= dx; i++) {
            /* draw pixel */
            *bbuf_start = color_line;

            /* Check error overflow */
            if (err_flag >= 0) {
                err_flag -= dx2;
                /* next line */
                bbuf_start += y_dir;
            }

            /*correct error */
            err_flag += dy2;
            /* go to next pixel */
            bbuf_start += x_dir;
        }
    }
    else {
        err_flag = dx2 - dy;

        /* draw line */
        for (int i = 0; i <= dy; i++) {
            /* draw pixel */
            *bbuf_start = color_line;

            /* Check error overflow */
            if (err_flag >= 0) {
                err_flag -= dy2;
                /* next line */
                bbuf_start += x_dir;
            }

            /*correct error */
            err_flag += dx2;
            /* go to next pixel */
            bbuf_start += y_dir;
        }
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
