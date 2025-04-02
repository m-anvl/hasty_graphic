#include "hasty_graphic.h"
#include <SDL3/SDL.h>

/* Error handle macro */
#define ERR_SDL(str) do { fprintf(stderr, "[%s:%d] %s error: %s\n",\
                        __FILE__, __LINE__, #str, SDL_GetError()); } while(0)

SDL_Window* g_wnd;
SDL_Renderer* g_rndr;
SDL_Texture* g_back_buffer_texture;
uint32_t* g_back_buffer;
int g_pitch;
int g_width;
int g_height;

static void create_window(const char* wnd_title)
{
    SDL_PropertiesID wnd_props = SDL_CreateProperties();
    if (!wnd_props) {
        ERR_SDL(SDL_CreateProperties);
        exit(EXIT_FAILURE);
    }

    SDL_SetStringProperty(wnd_props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, wnd_title);
    SDL_SetNumberProperty(wnd_props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, g_width);
    SDL_SetNumberProperty(wnd_props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, g_height);
    SDL_SetNumberProperty(wnd_props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
    SDL_SetNumberProperty(wnd_props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);

    g_wnd = SDL_CreateWindowWithProperties(wnd_props);
    if (!g_wnd) {
        ERR_SDL(SDL_CreateWindowWithProperties);
        exit(EXIT_FAILURE);
    }
}

static void create_back_buffer(void)
{
    g_back_buffer_texture = SDL_CreateTexture(g_rndr, SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING, g_width, g_height);
    if (!g_back_buffer_texture) {
        ERR_SDL(SDL_CreateTexture);
        exit(EXIT_FAILURE);
    }

    g_back_buffer = malloc(sizeof(*g_back_buffer) * g_width * g_height);
    if (!g_back_buffer) {
        fprintf(stderr, "[%s:%d] Back buffer don\'t created\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    clear_back_buffer(BLACK);
}

void init_graphics(const char* wnd_title, int w, int h)
{
    g_width = w;
    g_height = h;

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        ERR_SDL(SDL_InitSubSystem);
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);

    create_window(wnd_title);

    g_rndr = SDL_CreateRenderer(g_wnd, NULL);
    if (!g_rndr) {
        ERR_SDL(SDL_CreateRenderer);
        exit(EXIT_FAILURE);
    }

    /* Set black color for renderer */
    SDL_SetRenderDrawColor(g_rndr, 0x00u, 0x00u, 0x00u, SDL_ALPHA_OPAQUE);

    create_back_buffer();

    g_pitch = sizeof(g_back_buffer[0]) * g_width;
}

void shutdown_graphics(void)
{
    if (g_back_buffer) {
        free(g_back_buffer);
        g_back_buffer = NULL;
    }

    SDL_DestroyTexture(g_back_buffer_texture);
    g_back_buffer_texture = NULL;

    SDL_DestroyRenderer(g_rndr);
    g_rndr = NULL;

    SDL_DestroyWindow(g_wnd);
    g_wnd = NULL;
}

void redraw_screen(void)
{
    SDL_UpdateTexture(g_back_buffer_texture, NULL, g_back_buffer, g_pitch);
    SDL_RenderClear(g_rndr);
    SDL_RenderTexture(g_rndr, g_back_buffer_texture, NULL, NULL);
    SDL_RenderPresent(g_rndr);
}

void wait_quit_event(void)
{
    SDL_Event event;
    for (;;) {
        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT)
                break;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    break;
                }
            }
        }
        SDL_Delay(20);
    }
}

void set_pixel(int x, int y, uint32_t color)
{
    /* Convert to screen coordinate system */
    x = g_width / 2 + x;
    y = g_height / 2 - y;

    if (x >= g_width || y >= g_height || x < 0 || y < 0) {
        return;
    }

    g_back_buffer[g_width * y + x] = color;
}

void clear_back_buffer(uint32_t color)
{
    for (int i = 0; i < g_width * g_height; i++)
        g_back_buffer[i] = color;
}

uint32_t pack_color_rgb(const uint8_t r, const uint8_t g,
    const uint8_t b /* const uint8_t a = 0xff */)
{
    /* alpha = 0xff */
    return 0xff000000 + (b << 16) + (g << 8) + r;
}

uint32_t pack_color_rgba(const uint8_t r, const uint8_t g,
    const uint8_t b, const uint8_t a)
{
    return (a << 24) + (b << 16) + (g << 8) + r;
}
