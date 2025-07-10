#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * PROJECT EULER #72
 * Counting Fractions
 *
 * Consider the fraction, n/d, where n and d are positive integers. If n < d
 * and GCF(n,d) = 1, it is called a reduced proper fraction.
 *
 * If we list the set of reduced proper fractions for d ≤ 8 in ascending order
 * of size, we get:
 *
 * 1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, ... , 7/8
 *
 * It can be seen that there are 21 elements in this set.
 *
 * How many elements would be contained in the set of reduced proper fractions
 * for d ≤ 1000000?
 */

#define D 1000000

int gcd(int a, int b);

int main(int argc, char *argv[]) {
    long long count = 0;
    size_t max_prime = sqrt(D) + 1;
    int *primes = calloc(1000, sizeof(int));
    for (int d = 2; d <= D; d++) {
        int k = d, wheel = 1, coprimes = 1, *prime = primes;
        while (k > 1) {
            *prime = *prime ?: k;
            if (k % *prime == 0) {
                wheel *= *prime;
                coprimes *= *prime - 1;
                while (k % *prime == 0) k /= *prime;
            }
            if (*prime > max_prime) *prime = 0;
            prime++;
        }
        count += d / wheel * coprimes;
    }
    printf("Count: %lld\n", count);
    return EXIT_SUCCESS;
}