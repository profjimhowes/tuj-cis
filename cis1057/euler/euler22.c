#include <stdlib.h>
#include <stdio.h>
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

#define INPUT_FILE "data/euler22.txt"

int main(int argc, char *argv[]) {
    // Open input file
    FILE *file = open_file(INPUT_FILE);

    // Close input file
    fclose(file);
    
    return EXIT_SUCCESS;
}