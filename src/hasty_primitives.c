#include "hasty_primitives.h"

extern uint32_t* g_back_buffer;
extern int g_width;
extern int g_height;

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    /* Check if the line is on the screen */
    if (x0 < 0 || x0 > g_width - 1
        || x1 < 0 || x1 > g_width - 1
        || y0 < 0 || y0 > g_height - 1
        || y1 < 0 || y1 > g_height - 1)
        return;

    /* Bresenham's line algorithm */

    int dx, dy; /* deltas */
    int dx2, dy2; /* (dx, dy)x2 */
    int x_dir, y_dir; /* steps */
    int err_flag;

    /* Calc start address in back buffer (for draw start point) */
    uint32_t* bbuf_start = g_back_buffer + x0 + y0 * g_width;
    /* Calc deltas */
    dx = x1 - x0;
    dy = y1 - y0;

    if (dx >= 0) {
        x_dir = 1; /* Line direction - right */
    }
    else {
        x_dir = -1; /* left */
        dx = -dx; /* need abs value */
    }

    if (dy >= 0) {
        y_dir = g_width; /* down */
    }
    else {
        y_dir = -g_width; /* up */
        dy = -dy;
    }

    /* (dx, dy)x2 */
    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy) {
        err_flag = dy2 - dx;

        /* Draw line */
        for (int i = 0; i <= dx; i++) {
            /* draw pixel */
            *bbuf_start = color;

            /* Check error overflow */
            if (err_flag >= 0) {
                err_flag -= dx2;
                /* next line */
                bbuf_start += y_dir;
            }

            /*correct error */
            err_flag += dy2;
            /* go to next pixel */
            bbuf_start += x_dir;
        }
    }
    else {
        err_flag = dx2 - dy;

        /* draw line */
        for (int i = 0; i <= dy; i++) {
            /* draw pixel */
            *bbuf_start = color;

            /* Check error overflow */
            if (err_flag >= 0) {
                err_flag -= dy2;
                /* next line */
                bbuf_start += x_dir;
            }

            /*correct error */
            err_flag += dx2;
            /* go to next pixel */
            bbuf_start += y_dir;
        }
    }
}

void draw_line_h(int x0, int x1, int y, uint32_t color)
{

    /* Swap x0 and x1 for set correct endpoint */
    if (x1 < x0) {
        x0 += x1;
        x1 = x0 - x1;
        x0 -= x1;
    }

    /* Check if the line is on the screen */
    if (x1 < 0 || x0 >= g_width || y < 0 || y >= g_height)
        return;

    /* Clip */
    if (x0 < 0)
        x0 = 0;

    if (x1 >= g_width)
        x1 = g_width - 1;

    /* Dummy method */
    for (int i = x0; i < x1; i++) {
        set_pixel(i, y, color);
    }
}

void draw_line_v(int x, int y0, int y1, uint32_t color)
{
    /* Swap y0 and y1 for set correct endpoint */
    if (y1 < y0) {
        y0 += y1;
        y1 = y0 - y1;
        y0 -= y1;
    }

    /* Check if the line is on the screen */
    if (x < 0 || x >= g_width || y1 < 0 || y0 >= g_height)
        return;

    /* Clip */
    if (y0 < 0)
        y0 = 0;

    if (y1 >= g_height)
        y1 = g_height - 1;

    /* Dummy method */
    for (int i = y0; i < y1; i++) {
        set_pixel(x, i, color);
    }
}
