#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

#define ERR_SDL(str) do { fprintf(stderr, "[%s:%d] %s error: %s\n",\
                        __FILE__, __LINE__, #str, SDL_GetError()); } while(0)

#define WND_TITLE "My application"

SDL_Window* g_wnd;
SDL_Renderer* g_rndr;

void init_screen(const char* wnd_title, int w, int h)
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

int main(int argc, char* argv[])
{
    int scr_w = 640;
    int scr_h = 480;
    int pitch = sizeof(uint32_t) * scr_w;

    init_screen(WND_TITLE, scr_w, scr_h);

    SDL_Texture* back_buffer_texture = SDL_CreateTexture(g_rndr, SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING, scr_w, scr_h);
    if (!back_buffer_texture) {
        ERR_SDL(SDL_CreateTexture);
        exit(EXIT_FAILURE);
    }

    uint32_t* back_buffer = malloc(sizeof(*back_buffer) * scr_h * scr_w);
    if (!back_buffer) {
        fprintf(stderr, "[%s:%d] Back buffer don\'t created\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    /* Fill back buffer one color */
    for (size_t i = 0; i < (size_t)scr_h * scr_w; i++) {
        back_buffer[i] = 0xff000000u; /* color pack - AA BB GG RR */
    }

    SDL_Event event;

    while (true) {

        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT)
                break;

            if (event.type == SDL_EVENT_KEY_DOWN) {

                if (event.key.key == SDLK_ESCAPE) {
                    break;
                }
            }
        } /* End of event handling */

        SDL_UpdateTexture(back_buffer_texture, NULL, back_buffer, pitch);
        SDL_RenderClear(g_rndr);
        SDL_RenderTexture(g_rndr, back_buffer_texture, NULL, NULL);
        SDL_RenderPresent(g_rndr);
    }

    if (back_buffer) {
        free(back_buffer);
        back_buffer = NULL;
    }

    SDL_DestroyTexture(back_buffer_texture);
    SDL_DestroyRenderer(g_rndr);
    SDL_DestroyWindow(g_wnd);

    return EXIT_SUCCESS;
}
