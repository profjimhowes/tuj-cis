#include "fenster.h"
#include "turmite.h"
#include "keys.h"
#include "colors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WIN_SIZE 1024
#define SPEED 1 << 14
#define FPS 60

static Color palette[] = {
    CARBON_BLACK, INTENSE_CHERRY, SHAMROCK, OCEAN_DEEP, AMBER_GOLD
};

static inline void recolor() {
    Color c = palette[0];
    memmove(palette, palette + 1, 4 * sizeof(Color));
    palette[4] = c;
}

static void render(struct fenster *f, Turmite *t) {
    for (int gy = 0; gy < WIN_SIZE; gy++)
        for (int gx = 0; gx < WIN_SIZE; gx++)
            fenster_pixel(f, gx, gy) =
                palette[turmite_get_cell(t, gy, gx)];
}

int main(int argc, char *argv[]) {
    const int states = atoi(argv[1]), symbols = atoi(argv[2]);

    uint32_t buf[WIN_SIZE * WIN_SIZE] = {0};
    struct fenster f = {
        .title = "Turmite", .buf    = buf,
        .width = WIN_SIZE,  .height = WIN_SIZE
    };
    fenster_open(&f);

    Turmite *t = turmite_new(states, symbols, WIN_SIZE);

    const int64_t frame_ms = 1000 / FPS;
    int debounced_keys[256] = {0};
    while (fenster_loop(&f) == 0 && !f.keys[KEY_ESC]) {
        int64_t t0 = fenster_time();

        for (int i = 0; i < 256; i++) debounced_keys[i] &= !f.keys[i];
        if (debounced_keys[KEY_O]) { char *buffer = turmite_dump(t); puts(buffer); free(buffer); }
        if (debounced_keys[KEY_SP]) turmite_randomize(t), turmite_reset(t, 0);
        for (int k = KEY_0; k <= KEY_9; k++) if (debounced_keys[k]) turmite_reset(t, k - KEY_0);
        if (debounced_keys[KEY_C]) recolor();
        memcpy(debounced_keys, f.keys, sizeof(debounced_keys));

        for (int i = 0; i < SPEED; i++) turmite_step(t);
        render(&f, t);

        int64_t elapsed = fenster_time() - t0;
        if (elapsed < frame_ms)
            fenster_sleep((int)(frame_ms - elapsed));
    }

    turmite_free(t);
    fenster_close(&f);
    return 0;
}