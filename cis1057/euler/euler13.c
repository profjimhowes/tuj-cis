#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/**
 * PROJECT EULER #13
 * Large Sum
 *
 * Work out the first ten digits of the sum of the following one-hundred
 * 50-digit numbers.
 * (Input data available in data/euler13.txt)
 */

#define INPUT_FILE "data/euler13.txt"
#define COUNT 100
#define DIGITS 50

int load_data(char *path, int lines, int length, char data[lines][length + 1]);
void reverse(char *string, int length);
void add_into(char *sum, char *addend);

int main(int argc, char *argv[]) {
    char numbers[COUNT][DIGITS + 1];
    char sum[DIGITS * 2] = {0};
    if (load_data(INPUT_FILE, COUNT, DIGITS, numbers)) return EXIT_FAILURE;

    // Data loaded into numbers array, ready to work with
    for (int i = 0; i < COUNT; i++) {
        // reverse the row into little-endian
        reverse(numbers[i], DIGITS);
        // add it to running sum
        add_into(sum, numbers[i]);
    }
    reverse(sum, strlen(sum));
    printf("%.10s\n", sum); // print first 10 digits only

    return EXIT_SUCCESS;
}

int load_data(char *path, int lines, int length, char data[lines][length + 1]) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "[Error] Cannot open file '%s': %s\n", path, strerror(errno));
        return -1;
    }

    int count = 0;
    char format[16];
    sprintf(format, "%%%d[0-9]", length + 1);
    for (char buffer[100]; count < lines && fgets(buffer, sizeof(buffer), file); count++) {
        if (sscanf(buffer, format, data[count]) != 1) {
            fprintf(stderr, "[Error] Invalid data: %s\n", buffer);
            return -1;
        }
    }
    fclose(file);

    return 0;
}

void reverse(char *string, int length) {
    // TODO: implement this
}

void add_into(char *sum, char *addend) {
    // TODO: implement this
}