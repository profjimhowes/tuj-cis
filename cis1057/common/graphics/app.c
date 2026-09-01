#include <stdint.h>
#include <stdlib.h>
#include "fenster.h"
#include "app.h"

static struct fenster f;
static int down_keys[256], up_keys[256];

static void update_keys(int64_t elapsed) {
    for (int i = 0; i < 256; i++) {
        up_keys[i] = -!f.keys[i] & down_keys[i];
        down_keys[i] += elapsed;
        down_keys[i] *= f.keys[i];
    }
}

uint32_t *app_open(const char *title, int w, int h) {
    uint32_t *buf = calloc(w * h, sizeof(uint32_t));
    if (!buf) return NULL;

    struct fenster temp = {
        .title = title, .buf = buf,
        .width = w,     .height = h
    };
    memcpy(&f, &temp, sizeof(struct fenster));
    fenster_open(&f);
    return buf;
}

void app_close() {
    fenster_close(&f);
    free(f.buf);
    f.buf = NULL;
}

int app_continue(int fps_limit) {
    static int64_t timestamp = 0;
    int status = !fenster_loop(&f);
    if (status) {
        int64_t prev_frame = timestamp;
        timestamp = fenster_time();
        if (fps_limit > 0) {
            int64_t next_frame = prev_frame + 1000 / fps_limit;
            if (timestamp < next_frame) {
                fenster_sleep((int)(next_frame - timestamp));
                timestamp = fenster_time();
            }
        }
        update_keys(timestamp - prev_frame);
    }
    return status;
}

int key_down(int keycode) {
    return down_keys[keycode & 0xff];
}

int key_up(int keycode) {
    return up_keys[keycode & 0xff];
}