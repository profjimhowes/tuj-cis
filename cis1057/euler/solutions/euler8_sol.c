#include <stdlib.h>
#include <stdio.h>
#include "../utils/files.h"

/**
 * PROJECT EULER #8
 * Largest Product in a Series
 *
 * The four adjacent digits in the 1000-digit number that have the greatest
 * product are 9 x 9 x 8 x 9 = 5832.
 *
 * (data available in data/euler8.txt)
 *
 * Find the thirteen adjacent digits in the 1000-digit number that have the
 * greatest product. What is the value of this product?
 */

#define INPUT_FILE "../data/euler8.txt"
#define DIGITS 1000
#define PRODUCT_LENGTH 13

int main(int argc, char *argv[]) {
    FILE *file = open_file(INPUT_FILE);
    int number[DIGITS];
    int *digit = number;

    while ((*digit = fgetc(file)) != EOF) {
        if (*digit >= '0' && *digit <= '9') {
            *digit++ -= '0';
        }
    }
    fclose(file);

    long long product = 1, max = 0;
    for (int i = 0, j = 0; i < DIGITS; i++, j++) {
        if (number[i] == 0) {
            j = 0, product = 1;
        } else {
            product *= number[i];
            if (j >= 13) {
                if (j > 13) product /= number[i - 13];
                if (product > max) max = product;
            }
        }
    }
    printf("Maximum product: %lld\n", max);

    return EXIT_SUCCESS;
}