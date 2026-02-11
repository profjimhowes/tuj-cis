// app.h
#ifndef APP_H
#define APP_H

#include <stdint.h>

typedef enum {
    KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
    KEY_SPACE, KEY_SHIFT,
    KEY_F, KEY_L,
    KEY_ESC,
    KEY_COUNT
} Key;

typedef struct {
    uint8_t down[KEY_COUNT];
    uint8_t pressed[KEY_COUNT];   // debounced: up->down this frame
    uint8_t released[KEY_COUNT];  // debounced: down->up this frame
} Input;

// 1) init: creates window + allocates framebuffer
int app_init(const char *title, int w, int h);

// 2) shutdown
void app_shutdown(void);

// 3) pump: polls OS events, updates debounced input, returns 0 if quit requested
int app_pump(Input *in);

// 4) present: blit framebuffer to the screen
void app_present(const uint32_t *fb);

// 5) time: monotonic seconds
double app_time(void);

// 6) framebuffer: returns pointer + dimensions (so sim can render into it)
uint32_t *app_framebuffer(int *out_w, int *out_h);

#endif // APP_H