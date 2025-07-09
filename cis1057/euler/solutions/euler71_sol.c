#include <stdlib.h>
#include <stdio.h>

/**
 * PROJECT EULER #71
 * Ordered Fractions
 *
 * Consider the fraction, n/d, where n and d are positive integers. If n < d
 * and GCF(n,d) = 1, it is called a reduced proper fraction.
 *
 * If we list the set of reduced proper fractions for d ≤ 8 in ascending order
 * of size, we get:
 *
 * 1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, ... , 7/8
 *
 * It can be seen that 2/5 is the fraction immediately to the left of 3/7.
 *
 * By listing the set of reduced proper fractions for d ≤ 1000000 in ascending
 * order of size, find the numerator of the fraction immediately to the left of 3/7.
 */

#define D 1000000
#define TARGET_NUMER 3
#define TARGET_DENOM 7

int gcd(int a, int b);

int main(int argc, char *argv[]) {
    long double target = (long double)TARGET_NUMER / TARGET_DENOM, nearest = 0;
    printf("%7d/%-7d = %.12Lf <--- TARGET\n", TARGET_NUMER, TARGET_DENOM, target);
    for (int denom = D; denom > 1; denom--) {
        if (denom == TARGET_DENOM) continue;
        int numer = denom * target;
        while (gcd(numer, denom) > 1) numer--;
        long double value = ((long double)numer) / denom;
        if (value > nearest) {
            nearest = value;
            printf("%7d/%-7d = %.12Lf, delta = %.12Lf\n", numer, denom, nearest, target - nearest);
        }
    }
    return EXIT_SUCCESS;
}

int gcd(int a, int b) {
    if (a <= 1 || b <= 1) return 1;
    while (a != b)
        if (a < b) b -= a;
        else a -= b;
    return a;
}