// systems.c
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include "sim.h"

static struct {
    float *x, *y, *vx, *vy, *r;
    uint32_t *color;
} particles;

static void spawn(int i) {
    particles.r[i] = sqrt((float)(rand() % 8001) / 25 + 1.0f);
    particles.x[i] = (float)(rand() % W);
    particles.y[i] = (float)(rand() % H);
    particles.vx[i] = (float)(rand() % 401 - 200);
    particles.vy[i] = (float)(rand() % 401 - 200);
    particles.color[i] = rand();
}

// Initialize particle table
void sim_init(void) {
    particles.x = calloc(N * 5, sizeof(float));
    particles.y = particles.x + N;
    particles.vx = particles.y + N;
    particles.vy = particles.vx + N;
    particles.r = particles.vy + N;
    particles.color = calloc(N, sizeof(uint32_t));
    srand(time(NULL));
    for (int i = 0; i < N; i++) spawn(i);
}

// --- Systems ---
// You can keep these as separate passes for clarity (recommended for class),
// or fuse them later as an optimization exercise.

static void sys_integrate(float dt) {
    for (int i = 0; i < N; i++) {
        particles.x[i] += particles.vx[i] * dt;
        particles.y[i] += particles.vy[i] * dt;
    }
}

static void sys_wrap() {
    for (int i = 0; i < N; i++) {
        if (particles.x[i] < 0) { // off the left side of the screen
            particles.x[i] += W;
        } else if (particles.x[i] > W) {
            particles.x[i] -= W;
        }
        if (particles.y[i] < 0) {
            particles.y[i] += H;
        } else if (particles.y[i] > H) {
            particles.y[i] -= H;
        }
    }
}

static void sys_bounce() {
    for (int i = 0; i < N; i++) {
        if (particles.x[i] < 0) {
            if (particles.vx[i] < 0) particles.vx[i] = -particles.vx[i];
        } else if (particles.x[i] > W) {
            if (particles.vx[i] > 0) particles.vx[i] = -particles.vx[i];
        }
        if (particles.y[i] < 0) {
            if (particles.vy[i] < 0) particles.vy[i] = -particles.vy[i];
        } else if (particles.y[i] > H) {
            if (particles.vy[i] > 0) particles.vy[i] = -particles.vy[i];
        }
    }
}

static void sys_collision() {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            float dx = particles.x[j] - particles.x[i];
            float dy = particles.y[j] - particles.y[i];
            float dist_sq = dx*dx + dy*dy;
            float r2 = particles.r[i] + particles.r[j];
            r2 *= r2;
            
            // Check if particles overlap (distance < 1)
            if (dist_sq < r2 && dist_sq > 0.0f) {
                float dist = sqrtf(dist_sq);

                // Normalized collision normal (direction from i to j)
                float nx = dx / dist;
                float ny = dy / dist;
                
                // Relative velocity
                float dvx = particles.vx[i] - particles.vx[j];
                float dvy = particles.vy[i] - particles.vy[j];
                
                // Relative velocity along collision normal
                float dvn = dvx*nx + dvy*ny;
                
                // Only resolve if particles are moving toward each other
                if (dvn > 0) {
                    // For equal mass particles, exchange velocity along normal
                    // (for unequal masses you'd need the full momentum calculation)
                    particles.vx[i] -= dvn * nx;
                    particles.vy[i] -= dvn * ny;
                    particles.vx[j] += dvn * nx;
                    particles.vy[j] += dvn * ny;
                }
            }
        }
    }
}

void sim_step(float dt) {
    sys_integrate(dt);
    sys_bounce();
    sys_collision();
}

static void draw_circle(uint32_t *raster, float x, float y, float r, uint32_t c) {
    for (float dx = -r; dx <= r; dx++)
        for (float dy = -r; dy <= r; dy++)
            if (dx * dx + dy * dy <= r * r) {
                int px = x + dx + 0.5f, py = y + dy + 0.5f;
                if (0 <= px && px < W && 0 <= py && py < H)
                    raster[px + py * W] = c;
            }
}

// Render the simulation into framebuffer
void sim_render(uint32_t *raster) {
    for (int i = 0; i < W * H; i++) raster[i] = 0;
    for (int i = 0; i < N; i++)
        draw_circle(raster,
            particles.x[i],
            H - 1 - particles.y[i],
            particles.r[i],
            particles.color[i]
        );
}
