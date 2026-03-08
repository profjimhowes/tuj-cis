/* Project Euler
 * Problem 514: Geoboard Shapes
 * A geoboard (of order N) is a square board with equally-spaced pins
 * protruding from the surface, representing an integer point lattice for
 * coordinates 0 ≤ x,y ≤ N.
 *
 * John begins with a pinless geoboard. Each position on the board is a hole
 * that can be filled with a pin. John decides to generate a random integer
 * between 1 and N+1 (inclusive) for each hole in the geoboard. If the random
 * integer is equal to  for a given hole, then a pin is placed in that hole.
 *
 * After John is finished generating numbers for all (N+1)² holes and placing
 * any/all corresponding pins, he wraps a tight rubberband around the entire
 * group of pins protruding from the board. Let S represent the shape that is
 * formed. S can also be defined as the smallest convex shape that contains all
 * the pins. If there are fewer than three pins on the board (or if all pins
 * are collinear), S can be assumed to have zero area.
 *
 * Let E(N) be the expected area of S given a geoboard of order N. For example,
 * E(1) = 0.1875, E(2) = 0.94335, and E(10) = 55.03013 when rounded to five
 * decimal places.
 *
 * Calculate E(100) rounded to five decimal places.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef struct { int n, d; } Rational;
typedef struct { int x, y, rank; } Point;

static inline void swap(Point *p1, Point *p2) { Point t = *p1; *p1 = *p2; *p2 = t; }

static inline void insort(Point *pts, int size, int dx, int dy) {
    pts->rank = pts->x * dx + pts->y * dy; pts++;
    for (int i = 1; i < size; i++, pts++) {
        pts->rank = pts->x * dx + pts->y * dy;
        for (Point *p = pts;; p--) {
            int diff = (p->rank < p[-1].rank) - (p->rank > p[-1].rank);
            if (diff < 0) break;
            if (!diff && p[-1].y < p->y) break;
            if (!diff && p[-1].y == p->y && p[-1].x > p->x) break;
            swap(p - 1, p);
        }
    }
}

static inline void reflect_tail(Point *last) {
    Point *first = last;
    for (; first->x || first->y; first--)
        first->x *= -1, first->y *= -1;
    while (last - first > 0) swap(first++, last--);
}

static inline void next_farey(int n, Rational *frac) {
    static int c, d = 0;
    if (!d) c = n - 1, d = n;
    int a = frac->n, b = frac->d, k = (n + b) / d;
    frac->n = c, frac->d = d;
    c = k * c - a, d = k * d - b;
}

static inline int signed_area(Point *p, Point *q) { return p->x * q->y - q->x * p->y; }

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    int total = (N + 1) * (N + 1);
    double sum = 0.0, rho = (double)N / (N + 1);

    // Initialize area accumulation bins
    int **acc = malloc(sizeof(int *) * total);
    acc[0] = calloc(total / 2 + N, sizeof(int) * total);
    for (int i = 1; i < total; i++) acc[i] = acc[i-1] + total / 2 + N;

    // Initialize points along diagonals
    int size = total + 1 >> 1;
    Point *pts = calloc(size + 1, sizeof(Point));
    *pts++ = (Point){-N / 2, -N / 2, INT_MIN};
    for (int c = 0, i = 0; i < size; c++)
        for (int d = 0; d <= c && i < size; d++, i++)
            pts[i].x = c - d - N / 2, pts[i].y = d - N / 2;

    for (Rational m = {1, 1}; m.n >= 0; next_farey(N, &m)) {
        int j = size - 1;
        insort(pts, size, m.n, m.d);
        reflect_tail(pts + j);
        while (pts[--j].rank == pts[size - 1].rank);
        for (int i = ++j; j >= (m.d + 1) * (m.n + 1) / 2 - 1; j = --i) {
            int mult = (m.n * m.d > 1) + 1;
            while (pts[--i].rank == pts[j].rank);
            for (int p = ++i; p < j; p++) for (int q = p + 1; q <= j; q++)
                acc[total - j - i - 1][j + p - q] -= signed_area(pts + p, pts + q) * mult;
        }
    }
    for (int i = 0; i < total; i++) {
        double sub = 0.0;
        for (int j = total / 2 + N - 1; j >= 0; j--)
            sub += acc[i][j] * pow(rho, j);
        sum += sub * (1 - pow(rho, i));
    }
    sum *= 2.0 / (N + 1) / (N + 1);
    printf("Expected area: %.5lf\n", sum);

    return 0;
}