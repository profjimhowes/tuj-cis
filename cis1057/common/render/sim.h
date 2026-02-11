#ifndef SIM_H
#define SIM_H

enum {W = 2400, H = 1350};

// Implemented in systems.c
void sim_init(void);
void sim_step(float dt);
void sim_render(uint32_t *fb);

#endif // SIM_H