#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * PROJECT EULER #77
 * Prime Summations
 *
 * It is possible to write ten as the sum of primes in exactly
 * five different ways:
 *
 * 7 + 3
 * 5 + 5
 * 5 + 3 + 2
 * 3 + 3 + 2 + 2
 * 2 + 2 + 2 + 2
 *
 * What is the first value which can be written as the sum of
 * primes in over five thousand different ways?
 */

#define LIMIT 5000

int main(int argc, char *argv[]) {
    int *primes = calloc(1000, sizeof(int)), pcount = 0;
    int **count = malloc(1000 * sizeof(int *));
    for (int n = 2; n < 1000; n++, count++) {
        int comp = 0, total = 0;
        *count = calloc(pcount, sizeof(int));
        for (int p = 0; p < pcount; p++) {
            if (n - primes[p] > 1) {
                for (int q = 0; q <= p; q++) {
                    if (n - primes[p] > primes[q]) (*count)[p] += (*(count - primes[p]))[q];
                    else if (n - primes[p] == primes[q]) (*count)[p]++;
                    else break;
                }
            } else (*count)[p] = 0;
            if (!comp && n % primes[p] == 0) comp = 1;
            total += (*count)[p];
        }
        if (!comp) primes[pcount++] = n;
        if (total > LIMIT) {
            printf("%d can be written %d ways\n", n, total);
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}