#include "app/app.h"
#include "sim.h"

int main_loop(void) {
    FrameBuffer *fb = app_init("Particles", W, H);
    if (!fb) return 1;

    sim_init();

    Input in;
    while (app_pump(&in)) {
        sim_step(1.0f/600.0f);
        sim_render(fb->raster);
        app_present(fb);
    }

    app_shutdown();
    return 0;
}