#include <string.h>
#include <stdio.h>

const char *singles[] = {
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
};

const char *teens[] = {
    "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};

const char *tens[] = {
    "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
};

int main(void) {
    int num = 0;
    printf("Please enter an integer 0-99: ");
    scanf_s("%d", &num);
    printf("You entered %d.\n", num);

    if (num < 10) puts(singles[num]);
    else if (num < 20) puts(teens[num % 10]);
    else {
        printf("%s", tens[num / 10 - 2]);
        if (num % 10) printf("-%s", singles[num % 10]);
    }

    return 0;
}