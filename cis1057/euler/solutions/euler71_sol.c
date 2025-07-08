#include <stdlib.h>
#include <stdio.h>

/**
 * PROJECT EULER #71
 * Ordered Fractions
 *
 * Consider the fraction, n/d, where n and d are positive integers. If n < d
 * and GCF(n,d) = 1, it is called a reduced proper fraction.
 *
 * If we list the set of reduced proper fractions for d ≤ 8 in ascending order
 * of size, we get:
 *
 * 1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, ... , 7/8
 *
 * It can be seen that 2/5 is the fraction immediately to the left of 3/7.
 *
 * By listing the set of reduced proper fractions for d ≤ 1000000 in ascending
 * order of size, find the numerator of the fraction immediately to the left of 3/7.
 */

#define D 1000000

typedef struct {
    int numer;
    int denom;
    double as_double;
} Rational;

void set_double(Rational *r);
void print_rat(Rational *r);

typedef struct {
    size_t size;
    Rational *data[D >> 1];
} RatHeap;

void heap_insert(RatHeap *heap, Rational *value);
Rational *heap_remove(RatHeap *heap);
int invalid_heap(RatHeap *heap);
void print_heap(RatHeap *heap);

int gcd(int a, int b);

int main(int argc, char *argv[]) {
    size_t length = D + 1 >> 1;
    Rational r = {1, length}, *p;
    set_double(&r);
    Rational *rats = calloc(length, sizeof(Rational));
    rats[0] = r;
    RatHeap *heap = calloc(1, sizeof(RatHeap));
    heap_insert(heap, rats);
    for (int n = 2, d; n < length; n++) {
        for (d = D; gcd(n, d) > 1; d--);
        rats[n - 1].numer = n;
        rats[n - 1].denom = d;
        set_double(&rats[n - 1]);
        heap_insert(heap, &rats[n - 1]);
    }
    p = heap_remove(heap);
    while (p->numer != 3 || p->denom != 7) {
        r = *p;
        if (p->as_double < 3 / 7.0) {
            while (gcd(p->numer, --p->denom) > 1);
            set_double(p);
            heap_insert(heap, p);
        }
        p = heap_remove(heap);
    }
    printf("Solution: ");
    print_rat(&r);
    puts("");
    return EXIT_SUCCESS;
}

void heap_insert(RatHeap *heap, Rational *value) {
    int i = heap->size++;
    for (int j = i - 1 >> 1; i > 0 && value->as_double < heap->data[j]->as_double; j = i - 1 >> 1) {
        heap->data[i] = heap->data[j];
        i = j;
    }
    heap->data[i] = value;
}

Rational *heap_remove(RatHeap *heap) {
    Rational *value = heap->data[--heap->size];
    heap->data[heap->size] = heap->data[0];
    int i = 0;
    for (int j; i < heap->size >> 1; i = j) {
        j = i + 1 << 1;
        if (j-- < heap->size)
            if (heap->data[j]->as_double < value->as_double) {
                if (heap->data[j + 1]->as_double < heap->data[j]->as_double) j++;
                heap->data[i] = heap->data[j];
            } else if (heap->data[++j]->as_double < value->as_double)
                heap->data[i] = heap->data[j];
            else break;
        else if (heap->data[j]->as_double < value->as_double)
            heap->data[i] = heap->data[j];
        else break;
    }
    heap->data[i] = value;
    return heap->data[heap->size];
}

void print_heap(RatHeap *heap) {
    if (!heap->size) {
        puts("Empty Heap");
        return;
    }
    printf("Heap (%lu): ", heap->size);
    print_rat(heap->data[0]);
    for (int i = 1; i < heap->size; i++) {
        printf(", ");
        print_rat(heap->data[i]);
    }
    puts("");
}

void set_double(Rational *r) {
    r->as_double = (double)r->numer / r->denom;
}

void print_rat(Rational *r) {
    printf("%d/%d", r->numer, r->denom);
}

int gcd(int a, int b) {
    if (a == 1 || b == 1) return 1;
    while (a != b)
        if (a < b) b -= a;
        else a -= b;
    return a;
}