#include <stdlib.h>
#include <stdio.h>
#include "../../common/timer.h"

/**
 * PROJECT EULER #1
 * Multiples of 3 or 5
 *
 * If we list all the natural numbers below 10 that are multiples of 3 or 5,
 * we get 3, 5, 6 and 9. The sum of these multiples is 23.
 *
 * Find the sum of all the multiples of 3 or 5 below 1000.
 */

#define LIMIT 1000
#define A 3
#define B 5

long long linear_sum(long long end, int skip);

int main(int argc, char *argv[]) {
    // Approach 1: count through all integers, test each
    long long sum = 0;
    PRINT_ELAPSED(
        for (int i = 0; i < LIMIT; i++) {
            if (i % A == 0 || i % B == 0) {
                sum += i;
            }
        }
    );
    printf("Sum: %lld\n", sum);

    // Approach 2: Skip count through multiples, but subtract duplicates
    sum = 0;
    PRINT_ELAPSED(
        for (int i = 0; i < LIMIT; i += A)
            sum += i;
        for (int i = 0; i < LIMIT; i += B)
            if (i % A)
                sum += i;
    );
    printf("Sum: %lld\n", sum);

    // Approach 3: Use math
    sum = 0;
    PRINT_ELAPSED(
        sum += linear_sum(LIMIT, A);  // Add multiples of 3
        sum += linear_sum(LIMIT, B);  // Then add multiples of 5
        sum -= linear_sum(LIMIT, A * B); // Finally remove multiples of 15 (= 3 * 5)
    );
    printf("Sum: %lld\n", sum);

    return EXIT_SUCCESS;
}

long long linear_sum(long long end, int skip) {
    long long k = (end - 1) / skip;
    return (k * (k + 1) / 2) * skip;
}