#include <stdio.h>
#include <stdlib.h>
#include "hasty_graphic.h"

SDL_Window* g_wnd;
SDL_Renderer* g_rndr;

void init_app(const char* wnd_title, int w, int h)
{
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        ERR_SDL(SDL_InitSubSystem);
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    SDL_PropertiesID wnd_props = SDL_CreateProperties();
    if (!wnd_props) {
        ERR_SDL(SDL_CreateProperties);
        exit(EXIT_FAILURE);
    }

    SDL_SetStringProperty(wnd_props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, wnd_title);
    SDL_SetNumberProperty(wnd_props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, w);
    SDL_SetNumberProperty(wnd_props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, h);
    SDL_SetNumberProperty(wnd_props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(wnd_props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);

    g_wnd = SDL_CreateWindowWithProperties(wnd_props);

    if (!g_wnd) {
        ERR_SDL(SDL_CreateWindowWithProperties);
        exit(EXIT_FAILURE);
    }

    g_rndr = SDL_CreateRenderer(g_wnd, NULL);

    if (!g_rndr) {
        ERR_SDL(SDL_CreateRenderer);
        exit(EXIT_FAILURE);
    }

    /* Set black color for renderer */
    SDL_SetRenderDrawColor(g_rndr, 0x00u, 0x00u, 0x00u, SDL_ALPHA_OPAQUE);
}

void shutdown_app(void)
{
    SDL_DestroyRenderer(g_rndr);
    SDL_DestroyWindow(g_wnd);
}
