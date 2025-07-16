#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/files.h"

/**
 * PROJECT EULER #22
 * Names Scores
 *
 * Using data/euler22.txt, a 46K text file containing over five-thousand
 * first names, begin by sorting it into alphabetical order. Then working
 * out the alphabetical value for each name, multiply this value by its
 * alphabetical position in the list to obtain a name score.
 *
 * For example, when the list is sorted into alphabetical order, COLIN,
 * which is worth 3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the
 * list. So, COLIN would obtain a score of 938 x 53 = 49714.
 *
 * What is the total of all the name scores in the file?
 */

#define INPUT_FILE "../data/euler22.txt"
#define BUFFER_SIZE 1 << 16
#define NAME_LIMIT 6000

void heap_float(char **heap, int index);
void heap_sink(char **heap, int index, int limit);
void swap(char **a, char **b);

int main(int argc, char *argv[]) {
    // Open input file
    FILE *file = open_file(INPUT_FILE);

    // Pack names into buffer and assign pointers to first letter of each
    // Heapify the name pointers in reverse order to prep for heapsort
    char buffer[BUFFER_SIZE] = {0};
    char *names[NAME_LIMIT] = {0};
    int b = 0, n = 0;
    for (int c; (c = fgetc(file)) != EOF;)
        if (names[n])
            if (c == '"') {
                buffer[b++] = '\0';
                heap_float(names, n++);
            } else buffer[b++] = c;
        else if (c == '"') names[n] = &buffer[b];

    // Close input file
    fclose(file);

    // Heap sort
    while (n > 0) {
        swap(&names[0], &names[--n]);
        heap_sink(names, 0, n);
    }

    long long total = 0;
    for (int i = 0; i < NAME_LIMIT && names[i]; i++)
        printf("%s ", names[i]);
    puts("");
    return EXIT_SUCCESS;
}

void heap_float(char **heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (strcmp(heap[index], heap[parent]) > 0) {
            swap(&heap[index], &heap[parent]);
            index = parent;
        } else break;
    }
}

void heap_sink(char **heap, int index, int limit) {
    for (int child; (child = index * 2 + 1) < limit; index = child)
        if (strcmp(heap[child++], heap[index]) > 0)
            if (child < limit && strcmp(heap[child], heap[child - 1]) > 0)
                swap(&heap[index], &heap[child]);
            else swap(&heap[index], &heap[--child]);
        else if (child < limit && strcmp(heap[child], heap[index]) > 0)
            swap(&heap[index], &heap[child]);
        else break;
}

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}