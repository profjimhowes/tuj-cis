#include "fenster.h"
#include "turmite.h"
#include "keys.h"
#include "colors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define WIN_SIZE 512
#define SPEED 1 << 13
#define FPS 60

#define DEFAULT_STATES 3
#define MAX_STATES 8

#define DEFAULT_SYMBOLS 5
#define MAX_SYMBOLS 8

static Color palette[] = {
    CARBON_BLACK, INTENSE_CHERRY, SHAMROCK, OCEAN_DEEP,
    AMBER_GOLD, RASPBERRY_PLUM, LIGHT_SEA_GREEN, COPPERWOOD
};

static void render(struct fenster *f, Turmite *t) {
    for (int gy = 0; gy < WIN_SIZE; gy++)
        for (int gx = 0; gx < WIN_SIZE; gx++)
            fenster_pixel(f, gx, gy) = palette[turmite_get_symbol(t, gy, gx)];
}

int main(int argc, char *argv[]) {
    int states = DEFAULT_STATES, symbols = DEFAULT_SYMBOLS;
    const char *filename = NULL;
    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-s") && i+1 < argc) states = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-m") && i+1 < argc) symbols = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-f") && i+1 < argc) filename = argv[++i];
        else { fprintf(stderr, "Unknown option: %s\n", argv[i]); return EXIT_FAILURE; }

    // Set up and open the fenster window
    uint32_t buf[WIN_SIZE * WIN_SIZE] = {0};
    struct fenster f = {
        .title  = "Turmite Visualizer",
        .buf    = buf,
        .width  = WIN_SIZE,
        .height = WIN_SIZE
    };
    fenster_open(&f);

    // Create and randomize the turmite
    Turmite *t = turmite_new(states, symbols, WIN_SIZE);
    assert(("Unable to allocate turmite", t != NULL));
    turmite_randomize(t);

    const int64_t frame_ms = 1000 / FPS;
    int debounced_keys[256] = {0};
    char buffer[256];
    while (!fenster_loop(&f) && !f.keys[KEY_ESC]) {
        int realloc = 0, reset = 0, randomize = 0;

        // Log current time for framerate management
        int64_t timer = fenster_time();

        // Update debounced keys to detect when keys are lifted
        for (int i = 0; i < 256; i++) debounced_keys[i] &= !f.keys[i];
        // All keypress events must happen BELOW this point

        if (debounced_keys[KEY_O]) puts(turmite_tostring(t, buffer));

        if (debounced_keys[KEY_UP] && states < MAX_STATES) realloc = states++;
        else if (debounced_keys[KEY_DOWN] && states > 2) realloc = states--;
        if (debounced_keys[KEY_RIGHT] && symbols < MAX_SYMBOLS) realloc = symbols++;
        else if (debounced_keys[KEY_LEFT] && symbols > 2) realloc = symbols--;
        randomize = debounced_keys[KEY_SP] || realloc;
        for (int k = KEY_0; k <= KEY_9; k++)
            if (debounced_keys[k]) reset = k;

        // All keypress events must happen ABOVE this point
        // Reset the debounced key tracker to prepare for next frame
        memcpy(debounced_keys, f.keys, sizeof(debounced_keys));

        // Reallocate, randomize, and reset the turmite if necessary
        if (realloc) {
            turmite_free(t);
            t = turmite_new(states, symbols, WIN_SIZE);
            assert(("Unable to allocate turmite", t != NULL));
        }
        if (randomize) turmite_randomize(t);
        if (reset || randomize) turmite_reset(t, reset >= KEY_0 ? reset - KEY_0 : 0);

        // Move the turmite, and then render the results to the window
        for (int i = 0; i < SPEED; i++) turmite_step(t);
        render(&f, t);

        // Check how much time has passed, and pause if necessary to maintain FPS
        timer = fenster_time() - timer;
        if (timer < frame_ms) fenster_sleep((int)(frame_ms - timer));
    }

    // Clean up by freeing the turmite and closing the fenster window
    turmite_free(t);
    fenster_close(&f);
    return EXIT_SUCCESS;
}
