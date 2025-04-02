#include "hasty_graphic.h"

#define WND_TITLE "My application"

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;

    init_graphics(WND_TITLE, scr_w, scr_h);

    /* Draw here */
    set_pixel(0, 0, RED);

    /* Clear and redraw screen */
    redraw_screen();

    wait_quit_event();

    shutdown_graphics();

    return EXIT_SUCCESS;
}
