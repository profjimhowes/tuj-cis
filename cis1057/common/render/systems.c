// systems.c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include "sim.h"
#include "color.h"

#define N 300

enum {X, VX, Y, VY, R};
static float particles[5][N];
static uint32_t color[N];

void spawn(int i) {
    particles[R][i] = rand() % 101 / 20.0f + 8.0f;

    particles[X][i] = (float)(rand() % W);
    particles[Y][i] = (float)(rand() % H);

    float mass = particles[R][i] * particles[R][i];
    float momentum = (rand() % 101) * (rand() % 101);

    float dx = W / 2.0f - particles[X][i];
    float dy = H / 2.0f - particles[Y][i];
    float dist = sqrtf(dx * dx + dy * dy);
    float speed = momentum / mass;

    if (dist > 0.0f) {
        particles[VX][i] = speed * dx / dist;
        particles[VY][i] = speed * dy / dist;
    } else {
        particles[VX][i] = 0.0f;
        particles[VY][i] = 0.0f;
    }

    color[i] = random_color();
}

// Initialize particle table
void sim_init(void) {
    srand(time(NULL));
    for (int i = 0; i < N; i++) spawn(i);
}

static void sys_integrate(float dt) {
    for (int i = 0; i < N; i++) {
        particles[X][i] += particles[VX][i] * dt;
        particles[Y][i] += particles[VY][i] * dt;
    }
}

static void sys_wrap() {
    for (int i = 0; i < N; i++) {
        if (particles[X][i] < 0) {
            particles[X][i] += W;
        } else if (particles[X][i] > W) {
            particles[X][i] -= W;
        }
        if (particles[Y][i] < 0) {
            particles[Y][i] += H;
        } else if (particles[Y][i] > H) {
            particles[Y][i] -= H;
        }
    }
}

static void sys_bounce(float e) {
    for (int i = 0; i < N; i++) {
        if (particles[X][i] < particles[R][i]) {
            if (particles[VX][i] < 0) particles[VX][i] *= -e;
        } else if (particles[X][i] > W - particles[R][i]) {
            if (particles[VX][i] > 0) particles[VX][i] *= -e;
        }
        if (particles[Y][i] < particles[R][i]) {
            if (particles[VY][i] < 0) particles[VY][i] *= -e;
        } else if (particles[Y][i] > H - particles[R][i]) {
            if (particles[VY][i] > 0) particles[VY][i] *= -e;
        }
    }
}

static void sys_collision() {
    for (int i = 0; i < N; i++) {
        float ri2 = particles[R][i] * particles[R][i];
        for (int j = i + 1; j < N; j++) {
            float dx = particles[X][j] - particles[X][i];
            float dy = particles[Y][j] - particles[Y][i];
            float d2 = dx*dx + dy*dy;
            float rj2 = particles[R][j] * particles[R][j];
            float dr2 = ri2 + 2 * particles[R][i] * particles[R][j] + rj2;

            if (d2 < dr2 && d2 > 0.0f) {
                float dist = sqrtf(d2);

                float nx = dx / dist;
                float ny = dy / dist;

                float dvx = particles[VX][i] - particles[VX][j];
                float dvy = particles[VY][i] - particles[VY][j];
                float dvn = dvx*nx + dvy*ny;

                if (dvn > 0) {
                    // Mass proportional to r² (area); π cancels
                    float mi = ri2;
                    float mj = rj2;
                    float inv_mass_sum = 1.0f / (mi + mj);

                    // Impulse scalar for elastic collision:
                    //   j = 2 * mi * mj / (mi + mj) * dvn
                    // Δvi = -j/mi = -2 * mj / (mi + mj) * dvn
                    // Δvj = +j/mj = +2 * mi / (mi + mj) * dvn
                    float fi = 2.0f * mj * inv_mass_sum * dvn;
                    float fj = 2.0f * mi * inv_mass_sum * dvn;

                    particles[VX][i] -= fi * nx;
                    particles[VY][i] -= fi * ny;
                    particles[VX][j] += fj * nx;
                    particles[VY][j] += fj * ny;
                }
            }
        }
    }
}

static void sys_repel(float strength) {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            float dx = particles[X][j] - particles[X][i];
            float dy = particles[Y][j] - particles[Y][i];
            float d2 = dx * dx + dy * dy;

            if (d2 > 0.0f && d2 < 65536.0f) {
                float dist = sqrtf(d2);
                float gap = dist - particles[R][i] - particles[R][j];
                if (gap < 0.0f) gap = 0.0f;
                float gap2 = gap * gap + 1.0f; // +1 to avoid singularity at contact

                float f = strength / gap2;
                float fx = (dx / dist) * f;
                float fy = (dy / dist) * f;

                float mi_inv = 1.0f / (particles[R][i] * particles[R][i]);
                float mj_inv = 1.0f / (particles[R][j] * particles[R][j]);

                particles[VX][i] -= fx * mi_inv;
                particles[VY][i] -= fy * mi_inv;
                particles[VX][j] += fx * mj_inv;
                particles[VY][j] += fy * mj_inv;
            }
        }
    }
}

// Step the simulation one frame
void sim_step(float dt) {
    // tweakable constants (students can play here)
    const float g    = 9.8f * 30.0f;   // pixels/sec^2
    const float e    = 0.95f;          // bounce restitution

    sys_integrate(dt);
    sys_bounce(e);
    sys_repel(0x3000);
}

static void put_particle(uint32_t *fb, float x, float y, float r, uint32_t c) {
    float r2 = r * r;
    for (int px = -(r + 1.0f); px < r + 1.0f; px++) {
        unsigned x2 = px * px;
        for (int py = -(r + 1.0f); py < r + 1.0f; py++) {
            unsigned d2 = x2 + py * py;
            int fx = x + px, fy = y + py;
            if (d2 < r2 && fx >= 0 && fx < W && fy >= 0 && fy < H)
                fb[fx + fy * W] = c;
        }
    }
}

// Render the simulation into framebuffer
void sim_render(uint32_t *fb) {
    for (int i = 0; i < W * H; i++) fb[i] = fade(fb[i], 0.75);
    for (int i = 0; i < N; i++)
        put_particle(fb, particles[X][i], H - particles[Y][i] - 1, particles[R][i], color[i]);
}