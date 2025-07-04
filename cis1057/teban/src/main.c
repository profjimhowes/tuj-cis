#include <stdio.h>
#include <stdlib.h>
#include "Allocator.h"

Allocator *alloc;

int main(void) {
    alloc = Allocator_new();
    int *num = Allocator_push(alloc, sizeof(int));
    int *num2 = Allocator_push(alloc, sizeof(int));
    *num = 5;
    *num2 = 10;
    printf("num = %d, num2 = %d\n", *num, *num2);
    free(alloc);
    return 0;
}