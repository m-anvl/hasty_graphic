#include "hasty_graphic.h"
#include <math.h>

#define WND_TITLE "My application"
#define SCR_W 600
#define SCR_H 600

#define VIEWPORT_SIZE 1
#define PROJECTION_PLANE_Z 1
#define BG_COLOR WHITE

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
    vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

vec3 project_to_view(int32_t x, int32_t y)
{
    vec3 v;
    v.x = x;
    v.y = y;
    v.z = PROJECTION_PLANE_Z;

    return v;
}

int32_t dot_product(const vec3 a, const vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 sub_vec3(vec3 a, vec3 b)
{
    vec3 v;
    v.x = a.x - b.x;
    v.y = a.y - b.y;
    v.z = a.z = b.z;

    return v;
}

void intersect_ray_sphere(int32_t* t1, int32_t* t2,
    vec3 origin, vec3 direction, sphere* s)
{
    vec3 oc = sub_vec3(origin, s->center);

    float a = dot_product(direction, direction);
    float b = 2 * dot_product(oc, direction);
    float c = dot_product(oc, oc) - s->radius * s->radius;

    float discriminant = b * b - 4 * a * c;

    if (fabsf(discriminant) < 0.0f) {
        *t1 = INT32_MAX;
        *t2 = INT32_MAX;
        return;
    }

    *t1 = ((-b + sqrtf(discriminant)) / (2.0f * a));
    *t2 = ((-b - sqrtf(discriminant)) / (2.0f * a));
}

uint32_t trace_ray(sphere* spheres, size_t spheres_count,
    vec3 origin, vec3 direction, int32_t t_min, int32_t t_max)
{
    int32_t closest_t_ = INT32_MAX;
    sphere* closest_sphere = NULL;

    int32_t t1 = 0;
    int32_t t2 = 0;

    for (size_t i = 0; i < spheres_count; ++i) {
        intersect_ray_sphere(&t1, &t2, origin, direction, &spheres[i]);
        if (t1 < closest_t_ && t_min < t1 && t1 < t_max) {
            closest_t_ = t1;
            closest_sphere = &spheres[i];
        }
        if (t2 < closest_t_ && t_min < t2 && t2 < t_max) {
            closest_t_ = t2;
            closest_sphere = &spheres[i];
        }
    }

    if (closest_sphere == NULL) {
        return BG_COLOR;
    }

    return closest_sphere->color;
}

int main(int argc, char* argv[])
{
    init_graphics(WND_TITLE, SCR_W, SCR_H);

    /* Define scene */
    vec3 camera_pos = { 0, 0, 0 };
    sphere spheres[4];
    init_sphere(&spheres[0], vec3_pack(0, -100, 0), 400, RED);
    init_sphere(&spheres[1], vec3_pack(400, 0, 100), 400, BLUE);
    init_sphere(&spheres[2], vec3_pack(-400, 0, 100), 200, GREEN);
    init_sphere(&spheres[3], vec3_pack(0, -5001, 0), 5000, YELLOW);


    /* Draw */
    uint32_t color = BG_COLOR;
    for (int x = -SCR_W / 2; x < SCR_W / 2; ++x) {
        for (int y = -SCR_H / 2; y < SCR_H / 2; ++y) {
            vec3 direction = project_to_view(x, y);
            color = trace_ray(spheres, 4, camera_pos, direction, 1, INT32_MAX);
            set_pixel(x, y, color);
        }
    }

    /* Draw centers */
    for (size_t i = 0; i < 4; i++) {

        int sx = spheres[i].center.x;
        int sy = spheres[i].center.y;
        set_pixel(sx, sy, spheres[i].color);
    }

    /* Clear and redraw screen */
    redraw_screen();

    wait_quit_event();

    shutdown_graphics();

    return EXIT_SUCCESS;
}
