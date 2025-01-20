#ifndef HASTY_GRAPHIC_H_SENTRY
#define HASTY_GRAPHIC_H_SENTRY

#include <SDL3/SDL.h>

#define ERR_SDL(str) do { fprintf(stderr, "[%s:%d] %s error: %s\n",\
                        __FILE__, __LINE__, #str, SDL_GetError()); } while(0)

#ifdef __cplusplus
extern "C" {
#endif

void init_app(const char* wnd_title, int w, int h);
void shutdown_app(void);

#ifdef __cplusplus
}
#endif

#endif // !HASTY_GRAPHIC_H_SENTRY
