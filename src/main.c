#include "hasty_graphic.h"

#define WND_TITLE "My application"

typedef struct vector3_tag {
    int32_t x, y, z;
} vec3;

typedef struct sphere_tag {
    vec3 center;
    int32_t radius;
    uint32_t color;
} sphere;

void init_sphere(sphere* s, vec3 pos, int32_t radius, uint32_t color)
{
    s->center = pos;
    s->radius = radius;
    s->color = color;
}

vec3 vec3_pack(int32_t x, int32_t y, int32_t z)
{
    vec3 v = { x, y, z };
    return v;
}

int32_t dot_product(const vec3 a, const vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;

    init_graphics(WND_TITLE, scr_w, scr_h);

    /* Define scene */
    vec3 camera_pos = { 0, 0, 0 };
    const int32_t viewport_size = 1;
    const int32_t projection_plane_z = 1;
    uint32_t bg_color = WHITE;
    sphere s1, s2, s3;
    init_sphere(&s1, vec3_pack(0, -1, 3), 1, RED);
    init_sphere(&s2, vec3_pack(2, 0, 4), 1, BLUE);
    init_sphere(&s3, vec3_pack(-2, 0, 4), 1, GREEN);

    /* Draw */
    for (int x = -scr_w / 2; x < scr_w / 2; ++x) {
        for (int y = -scr_h / 2; y < scr_h / 2; ++y) {

            set_pixel(x, y, RED);
        }
    }

    /* Clear and redraw screen */
    redraw_screen();

    wait_quit_event();

    shutdown_graphics();

    return EXIT_SUCCESS;
}
