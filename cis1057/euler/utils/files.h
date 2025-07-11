#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

FILE *open_file(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "[Error] Cannot open file '%s': %s\n", path, strerror(errno));
        exit(1);
    }
    return file;
}