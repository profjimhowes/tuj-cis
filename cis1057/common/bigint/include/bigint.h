typedef struct {
    size_t bytes;
    int value[];
} BigInt;

void BigInt_add(BigInt *x, BigInt *y);