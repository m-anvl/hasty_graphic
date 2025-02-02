#ifndef HASTY_SHAPES_H_SENTRY
#define HASTY_SHAPES_H_SENTRY

#include "hasty_graphic.h"

#ifdef __cplusplus
extern "C" {
#endif

void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_line_h(int x0, int x1, int y, uint32_t color);
void draw_line_v(int x, int y0, int y1, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif // !HASTY_SHAPES_H_SENTRY
