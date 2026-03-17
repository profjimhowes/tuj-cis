#include "turmite.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char symbol, dir, state;
} Transition;

struct Turmite {
    Transition *transitions;
    char *grid;
    int grid_size, head_x, head_y;
    char states, symbols, state;
};

// Movement directions
enum { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT, NUM_DIRS };
static const int DX[NUM_DIRS] = { 0, 1, 0,-1};
static const int DY[NUM_DIRS] = {-1, 0, 1, 0};

Turmite *turmite_new(int states, int symbols, int grid_size) {
    Turmite *t = malloc(sizeof(Turmite));
    if (!t) return NULL;
    t->transitions = calloc(states * symbols, sizeof(Transition));
    t->grid = calloc(grid_size * grid_size, sizeof(char));
    if (!t->grid || !t->transitions) { turmite_free(t); return NULL; }
    t->states = states, t->symbols = symbols, t->grid_size = grid_size;
    turmite_reset(t, 0);
    turmite_randomize(t);
    return t;
}

void turmite_free(Turmite *t) {
    if (t && t->grid) free(t->grid);
    if (t && t->transitions) free(t->transitions);
    if (t) free(t);
}

char *turmite_dump(Turmite *t) {
    char *buffer = calloc(3, t->states * t->symbols + 1);
    buffer[0] = t->states + '0', buffer[1] = t->symbols + '0';
    char *p = buffer + 2;
    for (int i = 0; i < t->states * t->symbols; i++, p += 3)
        p[0] = t->transitions[i].symbol + '0',
        p[1] = t->transitions[i].dir + '0',
        p[2] = t->transitions[i].state + '0';
    return buffer;
}

char turmite_get_cell(Turmite *t, int r, int c) {
    return t->grid[r * t->grid_size + c];
}

void turmite_reset(Turmite *t, char state) {
    memset(t->grid, 0, t->grid_size * t->grid_size * sizeof(char));
    t->head_x = t->head_y = 0;
    t->state = state % t->states;
}

void turmite_randomize(Turmite *t) {
    for (int i = 0; i < t->states * t->symbols; i++)
        t->transitions[i].symbol = rand() % t->symbols,
        t->transitions[i].dir    = rand() % NUM_DIRS,
        t->transitions[i].state  = rand() % t->states;
}

void turmite_step(Turmite *t) {
    int i = t->symbols * t->state + t->grid[t->head_y * t->grid_size + t->head_x];
    t->grid[t->head_y * t->grid_size + t->head_x] = t->transitions[i].symbol;
    t->head_x += DX[t->transitions[i].dir] + t->grid_size; t->head_x %= t->grid_size;
    t->head_y += DY[t->transitions[i].dir] + t->grid_size; t->head_y %= t->grid_size;
    t->state = t->transitions[i].state;
}
