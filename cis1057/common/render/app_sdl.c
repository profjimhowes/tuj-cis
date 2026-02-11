// app_sdl.c
#include "app.h"

#include <SDL2/SDL.h>
#include <string.h>

// App-owned SDL objects
static SDL_Window   *g_win = NULL;
static SDL_Renderer *g_ren = NULL;
static SDL_Texture  *g_tex = NULL;

// App-owned framebuffer
static uint32_t *g_fb = NULL;
static int g_w = 0, g_h = 0;

// Quit flag set by pump()
static int g_quit = 0;

// Previous key states for debouncing
static uint8_t g_prev_down[KEY_COUNT];

// Map SDL scancode -> our Key enum index, or -1 if ignored
static int map_scancode(SDL_Scancode sc) {
    switch (sc) {
        case SDL_SCANCODE_LEFT:   return KEY_LEFT;
        case SDL_SCANCODE_RIGHT:  return KEY_RIGHT;
        case SDL_SCANCODE_UP:     return KEY_UP;
        case SDL_SCANCODE_DOWN:   return KEY_DOWN;

        case SDL_SCANCODE_SPACE:  return KEY_SPACE;

        case SDL_SCANCODE_LSHIFT:
        case SDL_SCANCODE_RSHIFT: return KEY_SHIFT;

        case SDL_SCANCODE_F:      return KEY_F;
        case SDL_SCANCODE_L:      return KEY_L;

        case SDL_SCANCODE_ESCAPE: return KEY_ESC;

        default: return -1;
    }
}

int app_init(const char *title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) return 0;

    g_w = w;
    g_h = h;

    g_win = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
        0
    );
    if (!g_win) return 0;

    // Renderer: accelerated is fine; we still upload a texture each frame
    g_ren = SDL_CreateRenderer(g_win, -1, SDL_RENDERER_ACCELERATED);
    if (!g_ren) return 0;

    // Our framebuffer will be interpreted as 0xAARRGGBB (ARGB8888)
    g_tex = SDL_CreateTexture(
        g_ren,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );
    if (!g_tex) return 0;

    g_fb = (uint32_t*)SDL_malloc((size_t)w * (size_t)h * sizeof(uint32_t));
    if (!g_fb) return 0;

    memset(g_fb, 0, (size_t)w * (size_t)h * sizeof(uint32_t));
    memset(g_prev_down, 0, sizeof(g_prev_down));
    g_quit = 0;

    return 1;
}

void app_shutdown(void) {
    if (g_fb) { SDL_free(g_fb); g_fb = NULL; }
    if (g_tex) { SDL_DestroyTexture(g_tex); g_tex = NULL; }
    if (g_ren) { SDL_DestroyRenderer(g_ren); g_ren = NULL; }
    if (g_win) { SDL_DestroyWindow(g_win); g_win = NULL; }

    SDL_Quit();

    g_quit = g_w = g_h = 0;
}

uint32_t *app_framebuffer(int *out_w, int *out_h) {
    if (out_w) *out_w = g_w;
    if (out_h) *out_h = g_h;
    return g_fb;
}

double app_time(void) {
    static uint64_t freq = 0;
    if (!freq) freq = SDL_GetPerformanceFrequency();
    uint64_t t = SDL_GetPerformanceCounter();
    return (double)t / (double)freq;
}

int app_pump(Input *in) {
    if (g_quit) return 0;

    // Clear edges each frame
    memset(in, 0, sizeof(*in));

    // Pump events (this keeps the window responsive)
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) g_quit = 1;
    }
    if (g_quit) return 0;

    // SDL keyboard snapshot
    const uint8_t *keys = SDL_GetKeyboardState(NULL);

    // Build current down[] states only for our keys
    uint8_t cur_down[KEY_COUNT];
    memset(cur_down, 0, sizeof(cur_down));

    // We only care about a handful of scancodes; scanning all is fine but we can do it directly:
    // (Direct checks keep it simple and predictable.)
    cur_down[KEY_LEFT]  = keys[SDL_SCANCODE_LEFT]  || keys[SDL_SCANCODE_A];
    cur_down[KEY_RIGHT] = keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D];
    cur_down[KEY_UP]    = keys[SDL_SCANCODE_UP]    || keys[SDL_SCANCODE_W];
    cur_down[KEY_DOWN]  = keys[SDL_SCANCODE_DOWN]  || keys[SDL_SCANCODE_S];

    cur_down[KEY_SPACE] = keys[SDL_SCANCODE_SPACE];
    cur_down[KEY_SHIFT] = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];

    cur_down[KEY_F]     = keys[SDL_SCANCODE_F];
    cur_down[KEY_L]     = keys[SDL_SCANCODE_L];
    cur_down[KEY_ESC]   = keys[SDL_SCANCODE_ESCAPE];

    // Debounce
    for (int k = 0; k < KEY_COUNT; k++) {
        in->down[k]     = cur_down[k];
        in->pressed[k]  = (uint8_t)(cur_down[k] && !g_prev_down[k]);
        in->released[k] = (uint8_t)(!cur_down[k] && g_prev_down[k]);
        g_prev_down[k]  = cur_down[k];
    }

    return 1;
}

void app_present(const uint32_t *fb) {
    // Upload framebuffer into texture
    SDL_UpdateTexture(g_tex, NULL, fb, g_w * (int)sizeof(uint32_t));

    SDL_RenderClear(g_ren);
    SDL_RenderCopy(g_ren, g_tex, NULL, NULL);
    SDL_RenderPresent(g_ren);
}
