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
    vec3 operator+(const vec3& v) const { return { x + v.x, y + v.y, z + v.z }; }
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

struct material {
    vec3 diffuse_color_;
    material(const vec3& color) : diffuse_color_(color) {}
    material() : diffuse_color_() { vec3 diffuse_color; }
};

struct sphere {
    vec3 center_;
    float radius_;
    material material_;

    sphere(const vec3& c, const float& r, const material& m) :
        center_(c), radius_(r), material_(m) {}
};

bool ray_intresect_sphere(const sphere& s, const vec3& origin, const vec3& dir, float& t0)
{
    vec3 L = s.center_ - origin;
    float tca = L * dir;
    float d2 = L * L - tca * tca;

    if (d2 > s.radius_ * s.radius_) {
        return false;
    }

    float thc = sqrtf(s.radius_ * s.radius_ - d2);
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

bool scene_intresect(const vec3& origin, const vec3& dir,
    const std::vector<sphere>& spheres, vec3& hit, vec3& N, material& mat)
{
    float spheres_dist = std::numeric_limits<float>::max();

    for (sphere s : spheres) {
        float dist_i = 0.0f;

        if (ray_intresect_sphere(s, origin, dir, dist_i) && dist_i < spheres_dist) {
            spheres_dist = dist_i;
            hit = origin + dir * dist_i;
            N = (hit - s.center_).normalised();
            mat = s.material_;
        }
    }

    return spheres_dist < 1000.0f;
}

vec3 ray_cast(const vec3& origin, const vec3& dir, const std::vector<sphere>& spheres)
{
    vec3 point, N;
    material mat;
    if (!scene_intresect(origin, dir, spheres, point, N, mat)) {
        return vec3{ 0.2f, 0.7f, 0.8f };
    }

    return mat.diffuse_color_;
}

int main(int argc, char* argv[])
{
    constexpr int scr_w = 640;
    constexpr int scr_h = 480;
    constexpr int fov = M_PI / 2.0;

    init_graphics(WND_TITLE, scr_w, scr_h);

    material red_rubber(vec3{ 0.3f, 0.1f, 0.1f });
    material ivory(vec3{ 0.4f, 0.4f, 0.3f });

    std::vector<sphere> spheres;
    spheres.push_back(sphere(vec3{ -3.0f,  0.0f, -16.0f }, 2.0f, ivory));
    spheres.push_back(sphere(vec3{ -1.0f, -1.5f, -12.0f }, 2.0f, red_rubber));
    spheres.push_back(sphere(vec3{ 1.5f, -0.5f, -18.0f }, 3.0f, red_rubber));
    spheres.push_back(sphere(vec3{ 7.0f,  5.0f, -18.0f }, 4.0f, ivory));


    /* Make ray trace */
    std::vector<vec3> framebuffer(scr_w * scr_h);
    for (size_t i = 0; i < scr_w; ++i) {
        for (size_t j = 0; j < scr_h; ++j) {
            float x = (2 * (i + 0.5f) / (float)scr_w - 1) * tanf(fov * 0.5f) * scr_w / (float)scr_h;
            float y = -(2 * (j + 0.5f) / (float)scr_h - 1) * tanf(fov * 0.5f);
            vec3 dir = vec3{ x, y, -1 }.normalised();
            framebuffer[i + j * scr_w] = ray_cast(vec3{ 0,0,0 }, dir, spheres);
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
