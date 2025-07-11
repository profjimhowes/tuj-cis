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
#define LENGTH 13

int main(int argc, char *argv[]) {
    // Open input file
    FILE *file = open_file(INPUT_FILE);

    // Read digits one character at a time and accumulate product
    // Non-digit characters are ignored
    int number[LENGTH];
    long long max = 0, product = 1;
    for (int i = 0, len = 0; (number[i] = fgetc(file)) != EOF;)
        // Reset the product after a zero
        if (number[i] == '0')
            len = 0, product = 1;

        // Otherwise convert to integer and multiply into product
        else if (number[i] > '0' && number[i] <= '9') {
            product *= number[i++] -= '0';
            i %= LENGTH;

            // Once target length is reached, update maximum
            // and divide out last number from the sequence
            if (++len >= LENGTH) {
                max = product > max ? product : max;
                product /= number[i];
            }
        }

    // Close input file
    fclose(file);

    // Output answer and exit
    printf("Maximum product: %lld\n", max);
    return EXIT_SUCCESS;
}