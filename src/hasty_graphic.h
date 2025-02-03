#ifndef HASTY_GRAPHIC_H_SENTRY
#define HASTY_GRAPHIC_H_SENTRY

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Colors defines */
#define BLACK   0xff000000u
#define WHITE   0xffffffffu
#define RED     0xff0000ffu
#define GREEN   0xff00ff00u
#define BLUE    0xffff0000u
#define CYAN    0xffffff00u
#define MAGENTA 0xffff00ffu
#define YELLOW  0xff00ffffu
#define GREY    0xff808080u
#define ORANGE  0xff0080ffu

#ifdef __cplusplus
extern "C" {
#endif

void init_graphics(const char* wnd_title, int w, int h);
void shutdown_graphics(void);

void clear_back_buffer(uint32_t color);
void redraw_screen(void);

void set_pixel(int x, int y, uint32_t color);

uint32_t pack_color_rgb(const uint8_t r, const uint8_t g, const uint8_t b);
uint32_t pack_color_rgba(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

#ifdef __cplusplus
}
#endif

#endif // !HASTY_GRAPHIC_H_SENTRY
