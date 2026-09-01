#include <stdio.h>
#include <stdlib.h>

int sum(int *array, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += *array++; // <-- what's going on here?
    return sum;
}

int * make_int() {
    int *p = malloc(sizeof(int));
    *p = 0;
    return p;
}

int global = 13;
static int hidden = 27;

int main(int argc, char *argv[]) {
    int array[10]; // arrays act like pointers
    for (int i = 0; i < 10; i++) array[i] = i * 2;
    int result = sum(array, 10);
    printf("sum = %d\n", result);

    double d = 1.0;
    double *d_ptr = &d; // address of operator provides pointers

    // Pointers also come from dynamic allocation
    int *dyn_x = make_int();
    *dyn_x = 255;
    printf("dyn_x    = %p\n", dyn_x);
    printf("d_ptr    = %p\n", d_ptr);
    printf("array    = %p\n", array);
    printf("&global  = %p\n", &global);

    printf("*dyn_x   = %d\n", *dyn_x);
    free(dyn_x);
    printf("*dyn_x   = %d\n", *dyn_x);

    return 0;
}