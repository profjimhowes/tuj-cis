#pragma once

typedef struct Turmite Turmite;

void turmite_randomize(Turmite *t);
void turmite_reset(Turmite *t, char state);
void turmite_step(Turmite *t);
char turmite_get_cell(Turmite *t, int x, int y);
char *turmite_dump(Turmite *t);
Turmite *turmite_new(int states, int symbols, int grid_size);
void turmite_free(Turmite *t);