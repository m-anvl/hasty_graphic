#include "hasty_graphic.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <limits>
#define _USE_MATH_DEFINES
#include <math.h>

#define WND_TITLE "My application"

extern "C" {
    extern uint32_t* g_back_buffer;
}

struct vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    constexpr float& operator[](const size_t i) { assert(i < 3); return i == 0 ? x : (i == 1 ? y : z); }
    vec3 operator*(const float s) const { return { x * s, y * s, z * s }; }
    vec3 operator-(const vec3& v) const { return { x - v.x, y - v.y, z - v.z }; }
    float operator*(const vec3& v) const { return x * v.x + y * v.y + z * v.z; }

    float norm() const { return sqrtf(x * x + y * y + z * z); }
    vec3 normalised() const { return (*this) * (1.0f / norm()); }

    uint32_t pack_color_rgb()
    {
        float max_s = std::max(x, std::max(y, z));
        if (max_s > 1) { *this = *this * (1.0f / max_s); }
        return ::pack_color_rgb(
            static_cast<uint8_t>(255 * x),
            static_cast<uint8_t>(255 * y),
            static_cast<uint8_t>(255 * z)
        );
    }
};


struct sphere {
    vec3 center;
    float radius;
};

bool ray_intresect_sphere(const sphere& s, const vec3& origin, const vec3& dir, float& t0)
{
    vec3 L = s.center - origin;
    float tca = L * dir;
    float d2 = L * L - tca * tca;

    if (d2 > s.radius * s.radius) {
        return false;
    }

    float thc = sqrtf(s.radius * s.radius - d2);
    t0 = tca - thc;
    float t1 = tca + thc;
    if (t0 < 0) {
        t0 = t1;
    }
    if (t0 < 0) {
        return false;
    }

    return true;
}

vec3 ray_cast(const vec3& origin, const vec3& dir, const sphere& s)
{
    float sphere_dist = std::numeric_limits<float>::max();
    if (!ray_intresect_sphere(s, origin, dir, sphere_dist)) {
        return vec3{ 0.2f, 0.7f, 0.8f };
    }

    return vec3{ 0.4f, 0.4f, 0.3f };
}

int main(int argc, char* argv[])
{
    constexpr int scr_w = 640;
    constexpr int scr_h = 480;
    constexpr int fov = M_PI / 2.0;

    init_graphics(WND_TITLE, scr_w, scr_h);

    sphere s = { vec3{-3.0f, 0.0f, -16.0f}, 2.0f };

    /* Make ray trace */
    std::vector<vec3> framebuffer(scr_w * scr_h);
    for (size_t i = 0; i < scr_w; ++i) {
        for (size_t j = 0; j < scr_h; ++j) {
            float x = (2 * (i + 0.5f) / (float)scr_w - 1) * tanf(fov * 0.5f) * scr_w / (float)scr_h;
            float y = -(2 * (j + 0.5f) / (float)scr_h - 1) * tanf(fov * 0.5f);
            vec3 dir = vec3{ x, y, -1 }.normalised();
            framebuffer[i + j * scr_w] = ray_cast(vec3{ 0,0,0 }, dir, s);
        }
    }

    /* Copy from framebuffer to g_back_buffer */
    for (size_t i = 0; i < scr_w; ++i) {
        for (size_t j = 0; j < scr_h; ++j) {
            g_back_buffer[i + j * scr_w] = framebuffer[i + j * scr_w].pack_color_rgb();
        }
    }

    /* Clear and redraw screen */
    redraw_screen();

    wait_quit_event();

    shutdown_graphics();

    return EXIT_SUCCESS;
}
