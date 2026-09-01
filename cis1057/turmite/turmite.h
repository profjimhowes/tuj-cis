#pragma once

typedef struct Turmite Turmite;

Turmite *turmite_new(int states, int symbols, int grid_size);
void turmite_free(Turmite *t);
char *turmite_tostring(const Turmite *t, char *buffer);
char turmite_get_symbol(const Turmite *t, int x, int y);
void turmite_reset(Turmite *t, char state);
void turmite_randomize(Turmite *t);
void turmite_step(Turmite *t);
