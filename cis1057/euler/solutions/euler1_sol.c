#include <stdlib.h>
#include <stdio.h>
#include "timer.h"

/**
 * PROJECT EULER #1
 * Multiples of 3 or 5
 *
 * If we list all the natural numbers below 10 that are multiples of 3 or 5,
 * we get 3, 5, 6 and 9. The sum of these multiples is 23.
 *
 * Find the sum of all the multiples of 3 or 5 below 1000.
 */

#define END 1000000000

#define TIMER(code) do { \
    double start = get_time(); \
    { code } \
    double elapsed = get_time() - start; \
    printf("Elapsed time: %9.6lf seconds\n", elapsed); \
} while(0)

long long linear_sum(int end, int skip);

int main(int argc, char *argv[]) {
    long long sum = 0;
    // Approach 1: count through all integers, test each
    TIMER(
        for (int i = 0; i < END; i++) {
            if (i % 3 == 0 || i % 5 == 0) {
                sum += i;
            }
        }
    );
    printf("Sum: %d\n", sum);

    // Approach 2: Skip count through multiples, but subtract duplicates
    sum = 0;
    TIMER(
        for (int i = 0; i < END; i += 3) {
            sum += i;
        }
        for (int i = 0; i < END; i += 5) {
            if (i % 3) {
                sum += i;
            }
        }
    );
    printf("Sum: %d\n", sum);

    // Approach 3: Use math
    TIMER(
        // Sum multiples of 3
        sum = linear_sum(END, 3);

        // Then add sum of multiples of 5
        sum += linear_sum(END, 5);

        // Then remove duplicated values... which are multiples of 15
        sum -= linear_sum(END, 15);
    );
    printf("Sum: %d\n", sum);

    return EXIT_SUCCESS;
}

long long linear_sum(int end, int skip) {
    int k = (end - 1) / skip;
    return (k * (k + 1) / 2) * skip;
}