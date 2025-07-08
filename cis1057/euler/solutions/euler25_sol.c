#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * PROJECT EULER #25
 * 1000-digit Fibonacci Number
 *
 * The Fibonacci sequence is defined by the recurrence relation:
 *
 * F(n) = F(n-1) + F(n-2), where F(1) = 1 and F(2) = 1.
 *
 * Hence the first 12 terms will be:
 *     F(1)  = 1
 *     F(2)  = 1
 *     F(3)  = 2
 *     F(4)  = 3
 *     F(5)  = 5
 *     F(6)  = 8
 *     F(7)  = 13
 *     F(8)  = 21
 *     F(9)  = 34
 *     F(10) = 55
 *     F(11) = 89
 *     F(12) = 144
 *
 * The 12th term, F(12), is the first term to contain 3 digits.
 *
 * What is the index of the first term in the Fibonacci sequence to contain 1000 digits?
 */

void strint_puts(char *strint);
void strint_add(char *sum, char *addend);

#define TARGET_LENGTH 1000

int main(int argc, char *argv[]) {
    int n = 2;
    char a[TARGET_LENGTH + 1] = {'1'}, b[TARGET_LENGTH + 1] = {'1'};
    for (char *F_n = a, *F_prev = b; F_n[TARGET_LENGTH - 1] == 0; n++) {
        strint_add(F_prev, F_n);
        char *temp = F_n;
        F_n = F_prev;
        F_prev = temp;
    }
    printf("F(%d) = ", n);
    strint_puts(a);
    return EXIT_SUCCESS;
}

void strint_add(char *sum, char *addend) {
    int place = 0, carry = 0;
    while (sum[place] && addend[place]) {
        sum[place] += addend[place] + carry - '0';
        carry = sum[place] > '9';
        sum[place++] -= carry * 10;
    }
    while (addend[place] == '9' && carry) sum[place++] = '0';
    if (addend[place]) {
        if (carry) sum[place] += carry--;
        do sum[place] += addend[place]; while (addend[++place]);
    } else while (sum[place] == '9' && carry) sum[place++] = '0';
    if (carry) sum[place] = (sum[place] ?: '0') + carry;
}

void strint_puts(char *strint) {
    for (int i = strlen(strint); i > 0; putchar(strint[--i]));
    putchar('\n');
}