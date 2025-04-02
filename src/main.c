#include "hasty_graphic.h"
#include "hasty_shapes.h"

#define WND_TITLE "My application"

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;
    rect r = { 60, 30, 100, 180 };
    rect r2 = { 150, 200, 600, 180 };

    init_graphics(WND_TITLE, scr_w, scr_h);

    /* Draw here */
    draw_rect_fill(&r, CYAN);
    draw_line(50, 20, 150, 200, GREEN);
    draw_line_h(50, 150, 20, YELLOW);
    draw_line_h(50, 150, 200, RED);
    draw_line_v(50, 20, 200, BLACK);
    draw_line_v(150, 20, 200, BLACK);
    draw_line_h(650, 40, 300, MAGENTA);
    draw_line_v(100, 500, 20, MAGENTA);

    draw_rect(&r, WHITE);

    draw_rect_fill(&r2, BLUE);
    draw_rect(&r2, WHITE);
    draw_line_h(0, scr_w, scr_h / 2, RED);

    /* Clear and redraw screen */
    redraw_screen();

    wait_quit_event();

    shutdown_graphics();

    return EXIT_SUCCESS;
}
