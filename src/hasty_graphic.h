#ifndef HASTY_GRAPHIC_H_SENTRY
#define HASTY_GRAPHIC_H_SENTRY

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL3/SDL.h>

#define ERR_SDL(str) do { fprintf(stderr, "[%s:%d] %s error: %s\n",\
                        __FILE__, __LINE__, #str, SDL_GetError()); } while(0)

#ifdef __cplusplus
extern "C" {
#endif

void init_graphics(const char* wnd_title, int w, int h);
void shutdown_graphics(void);

void redraw_screen(void);

#ifdef __cplusplus
}
#endif

#endif // !HASTY_GRAPHIC_H_SENTRY
