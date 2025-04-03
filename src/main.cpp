#include "hasty_graphic.h"

#define WND_TITLE "My application"

struct vec3 {
    float x = 0;
    float y = 0;
    float z = 0;

    constexpr float dot_product(const vec3& v) { return x * v.x + y * v.y + z * v.z; };
};

int main(int argc, char* argv[])
{
    const int scr_w = 640;
    const int scr_h = 480;

    init_graphics(WND_TITLE, scr_w, scr_h);

    /* Draw here */
    set_pixel(scr_w / 2, scr_h / 2, RED);

    /* Clear and redraw screen */
    redraw_screen();

    wait_quit_event();

    shutdown_graphics();

    return EXIT_SUCCESS;
}
