#include "hasty_graphic.h"
#include <vector>
#include <cassert>
#include <algorithm>

#define WND_TITLE "My application"

extern "C" {
    extern uint32_t* g_back_buffer;
}

struct vec3 {
    float x = 0;
    float y = 0;
    float z = 0;

    constexpr float& operator[](const size_t i) { assert(i < 3); return i == 0 ? x : (i == 1 ? y : z); }
    constexpr vec3 operator*(const float s) { return { x * s, y * s, z * s }; }
    constexpr float dot_product(const vec3& v) { return x * v.x + y * v.y + z * v.z; }
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

int main(int argc, char* argv[])
{
    const int scr_w = 640;
    const int scr_h = 480;

    init_graphics(WND_TITLE, scr_w, scr_h);

    /* Make gradient */
    std::vector<vec3> framebuffer(scr_w * scr_h);
    for (size_t i = 0; i < scr_w; ++i) {
        for (size_t j = 0; j < scr_h; ++j) {
            framebuffer[i + j * scr_w] = vec3{ j / float(scr_h), i / (float)scr_w, .0f };
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
