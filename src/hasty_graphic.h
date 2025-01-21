#ifndef HASTY_GRAPHIC_H_SENTRY
#define HASTY_GRAPHIC_H_SENTRY

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL3/SDL.h>

#define ERR_SDL(str) do { fprintf(stderr, "[%s:%d] %s error: %s\n",\
                        __FILE__, __LINE__, #str, SDL_GetError()); } while(0)

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

void redraw_screen(void);

void set_pixel(int x, int y, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif // !HASTY_GRAPHIC_H_SENTRY
