#ifndef SIM_H
#define SIM_H

enum {W = 640, H = 480, N = 100};

int main_loop(void);

// Implemented in sim.c
void sim_init(void);
void sim_step(float dt);
void sim_render(uint32_t *fb);

#endif // SIM_H