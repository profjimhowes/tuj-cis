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

#define INPUT_FILE "../data/euler13.txt"
#define COUNT 100
#define DIGITS 50

int load_data(char *path, int lines, int length, char data[lines][length + 1]);

int main(int argc, char *argv[]) {
    char numbers[COUNT][DIGITS + 1];
    if (load_data(INPUT_FILE, COUNT, DIGITS, numbers)) return EXIT_FAILURE;

    char sum[DIGITS * 2] = {0};
    int place;
    for (int row = 0; row < COUNT; row++) {
        for (place = 0; place < DIGITS; place++) {
            sum[place] += numbers[row][DIGITS - place - 1] - '0';
            sum[place + 1] += sum[place] / 10;
            sum[place] %= 10;
        }
        while (sum[place] > 9) {
            sum[place + 1] += sum[place] / 10;
            sum[place++] %= 10;
        }
    }
    for (place = sizeof(sum); !sum[--place];);
    for (int i = 0; i < place; i++, place--) {
        int temp = sum[i] + '0';
        sum[i] = sum[place];
        sum[place] = temp;
    }
    while (place >= 0) sum[place--] += '0';

    puts(sum);

    return EXIT_SUCCESS;
}

int load_data(char *path, int lines, int length, char data[lines][length + 1]) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "[Error] Cannot open file '%s': %s\n", path, strerror(errno));
        return -1;
    }

    int count = 0;
    char format[32];
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