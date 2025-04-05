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
    vec3 operator*(const float s)   const { return { x * s, y * s, z * s }; }
    vec3 operator+(const vec3& v)   const { return { x + v.x, y + v.y, z + v.z }; }
    vec3 operator-(const vec3& v)   const { return { x - v.x, y - v.y, z - v.z }; }
    vec3 operator-()                const { return { -x, -y, -z }; }
    float operator*(const vec3& v)  const { return x * v.x + y * v.y + z * v.z; }

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
    vec3 diffuse_color_ = { 0.0f, 0.0f, 0.0f };
    float albedo_[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    float specular_exponent_ = 0.0f;
    float refractive_index_ = 1.0f;
};

struct light {
    vec3 position_;
    float intensity_;
    light(const vec3& p, const float& i) : position_(p), intensity_(i) {}
};

struct sphere {
    vec3 center_;
    float radius_;
    material material_;

    sphere(const vec3& c, const float& r, const material& m) :
        center_(c), radius_(r), material_(m) {}
};

vec3 reflect(const vec3& I, const vec3& N)
{
    return I - N * 2.0f * (I * N);
}

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

vec3 refract(const vec3& I, const vec3& N, const float& refractive_index)
{
    float cosi = -std::max(-1.0f, std::min(1.0f, I * N));
    float etai = 1.0f;
    float etat = refractive_index;
    vec3 n = N;

    if (cosi < 0.0f) {
        cosi = -cosi;
        std::swap(etai, etat);
        n = -N;
    }

    float eta = etai / etat;
    float k = 1 - eta * eta * (1.0f - cosi * cosi);

    return k < 0 ? vec3{ 0.0f, 0.0f, 0.0f } : I * eta + n * (eta * cosi - sqrtf(k));
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

    float checkerboard_dist = std::numeric_limits<float>::max();
    if (fabs(static_cast<double>(dir.y)) > 1e-3) {
        float d = -(origin.y + 4.0f) / dir.y;
        vec3 pt = origin + dir * d;

        if (d > 0.0f
            && fabsf(pt.x) < 10.f
            && pt.z < -10.0f
            && pt.z > -30.0f
            && d < spheres_dist) {
            checkerboard_dist = d;
            hit = pt;
            N = vec3{ 0.0f, 1.0f, 0.0f };
            mat.diffuse_color_ = (int(0.5f * hit.x + 1000) + int(0.5f * hit.z)) & 1
                ? vec3{ 1.0f, 1.0f, 1.0f } : vec3{ 1.0f, 0.7f, 0.3f };
            mat.diffuse_color_ = mat.diffuse_color_ * 0.3f;
        }
    }

    return std::min(spheres_dist, checkerboard_dist) < 1000.0f;
}

vec3 ray_cast(const vec3& origin, const vec3& dir, const std::vector<sphere>& spheres,
    const std::vector<light> lights, size_t depth = 0)
{
    vec3 point, N;
    material mat;
    if (depth > 4 || !scene_intresect(origin, dir, spheres, point, N, mat)) {
        return vec3{ 0.2f, 0.7f, 0.8f };
    }

    vec3 reflect_dir = reflect(dir, N).normalised();
    vec3 reflect_origin = reflect_dir * N < 0.0f ? point - N * 1e-3 : point + N * 1e-3;
    vec3 reflect_color = ray_cast(reflect_origin, reflect_dir, spheres, lights, depth + 1);

    vec3 refract_dir = refract(dir, N, mat.refractive_index_).normalised();
    vec3 refract_origin = refract_dir * N < 0.0f ? point - N * 1e-3 : point + N * 1e-3;
    vec3 refract_color = ray_cast(refract_origin, refract_dir, spheres, lights, depth + 1);

    float diffuse_light_intensity = 0.0f;
    float specular_light_intensity = 0.0f;

    for (light l : lights) {
        vec3 light_dir = (l.position_ - point).normalised();
        float light_dist = (l.position_ - point).norm();
        vec3 shadow_origin = light_dir * N < 0.0f ? point - N * 1e-3 : point + N * 1e-3;
        vec3 shadow_pt, shadow_N;
        material tmp_mat;

        if (scene_intresect(shadow_origin, light_dir, spheres, shadow_pt, shadow_N, tmp_mat)
            && (shadow_pt - shadow_origin).norm() < light_dist) {
            continue;
        }

        diffuse_light_intensity += l.intensity_ * std::max(0.0f, light_dir * N);
        specular_light_intensity += powf(
            std::max(0.0f, -(dir * (reflect(-light_dir, N)))),
            mat.specular_exponent_) * l.intensity_;
    }

    return mat.diffuse_color_ * diffuse_light_intensity * mat.albedo_[0]
        + vec3{ 1.0f, 1.0f, 1.0f } *specular_light_intensity * mat.albedo_[1]
        + reflect_color * mat.albedo_[2] + refract_color * mat.albedo_[3];
}

void render(const std::vector<sphere>& spheres, const std::vector<light> lights,
    const int w, const int h)
{
    constexpr int fov = M_PI / 2.0;

    /* Make ray trace */
    std::vector<vec3> framebuffer(w * h);
    for (size_t j = 0; j < h; ++j) {
        for (size_t i = 0; i < w; ++i) {
            float x = (2 * (i + 0.5f) / (float)w - 1) * tanf(fov * 0.5f) * w / (float)h;
            float y = -(2 * (j + 0.5f) / (float)h - 1) * tanf(fov * 0.5f);
            vec3 dir = vec3{ x, y, -1 }.normalised();
            framebuffer[i + j * w] = ray_cast(vec3{ 0,0,0 }, dir, spheres, lights);
        }
    }

    /* Copy from framebuffer to g_back_buffer */
    for (size_t i = 0; i < w; ++i) {
        for (size_t j = 0; j < h; ++j) {
            g_back_buffer[i + j * w] = framebuffer[i + j * w].pack_color_rgb();
        }
    }
}

int main(int argc, char* argv[])
{
    constexpr int scr_w = 640;
    constexpr int scr_h = 480;

    init_graphics(WND_TITLE, scr_w, scr_h);

    constexpr material red_rubber = { { 0.3f, 0.1f, 0.1f }, { 0.9f, 0.1f, 0.1f, 0.0f }, 50.0f, 1.0f };
    constexpr material ivory = { { 0.4f, 0.4f, 0.3f }, { 0.6f, 0.3f, 0.0f, 0.0f }, 10.0f, 1.0f };
    constexpr material mirror = { { 1.0f, 1.0f, 1.0f }, { 0.0f, 10.0f, 0.8f, 0.0f }, 1425.0f, 1.0f };
    constexpr material glass = { { 0.6f, 0.7f, 0.8f }, { 0.0f, 0.5f, 0.1f, 0.8f }, 125.0f, 1.5f };

    std::vector<sphere> spheres;
    spheres.push_back(sphere(vec3{ -3.0f,  0.0f, -16.0f }, 2.0f, ivory));
    spheres.push_back(sphere(vec3{ -1.0f, -1.5f, -12.0f }, 2.0f, glass));
    spheres.push_back(sphere(vec3{ 1.5f, -0.5f, -18.0f }, 3.0f, red_rubber));
    spheres.push_back(sphere(vec3{ 7.0f,  5.0f, -18.0f }, 4.0f, mirror));

    std::vector<light> lights;
    lights.push_back(light(vec3{ -20.0f, 20.0f, 20.0f }, 1.5f));
    lights.push_back(light(vec3{ 30.0f, 50.0f, -25.0f }, 1.8f));
    lights.push_back(light(vec3{ 30.0f, 20.0f, 30.0f }, 1.7f));

    render(spheres, lights, scr_w, scr_h);

    /* Clear and redraw screen */
    redraw_screen();

    wait_quit_event();

    shutdown_graphics();

    return EXIT_SUCCESS;
}
