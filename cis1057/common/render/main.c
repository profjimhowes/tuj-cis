#include "app.h"
#include "sim.h"
#include <stdio.h>

int main(void) {
    if (!app_init("Particles", W, H)) return 1;

    int fbw, fbh;
    uint32_t *fb = app_framebuffer(&fbw, &fbh);
    (void)fbw; (void)fbh;

    sim_init();

    Input in = {0};
    int show_fps = 1;
    int limit_fps = 1;

    for (double last = app_time(); app_pump(&in) && !in.pressed[KEY_ESC]; last = app_time()) {
        if (in.pressed[KEY_L]) limit_fps = !limit_fps;
        if (in.pressed[KEY_F]) show_fps = !show_fps;

        // fixed step for teaching
        sim_step(1.0f / 60.0f);
        sim_render(fb);

        app_present(fb);

        // FPS limiter can be implemented in main (sleep) or in app_present (SDL_Delay)
        (void)show_fps;
        (void)limit_fps;
    }

    app_shutdown();
    return 0;
}
