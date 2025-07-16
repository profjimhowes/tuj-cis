#include <stdio.h>
#include <stdlib.h>

/**
 * PROJECT EULER #31
 * Coin Sums
 *
 * In the United Kingdom the currency is made up of pound (£) and pence (p).
 * There are eight coins in general circulation:
 *
 * 1p, 2p, 5p, 10p, 20p, 50p, £1 (100p), and £2 (200p).
 *
 * It is possible to make £2 in the following way:
 *
 * 1×£1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p
 *
 * How many different ways can £2 be made using any number of coins?
 */

// Coin denominations in pence
int coins[] = {1, 2, 5, 10, 20, 50, 100, 200};

int main(void) {
    int ways[201] = {1}; // ways[i] = number of ways to make i pence; initialize ways[0]=1
    int n = sizeof(coins) / sizeof(coins[0]);

    // For each coin, update the ways to make each amount
    for (int c = 0; c < n; c++)
        for (int i = coins[c]; i <= 200; i++)
            ways[i] += ways[i - coins[c]];
            // For each amount i, add ways to make (i - coin) using current and previous coins

    printf("%d\n", ways[200]); // Print number of ways to make £2 (200p)
    return EXIT_SUCCESS;
}

//coins[] holds the coin values in pence.
//ways[201] is an array where ways[i] is the number of ways to make i pence.
//Initialize ways[0]=1 because there's one way to make 0p (using no coins).
//For each coin, loop through all amounts from that coin's value up to 200p, updating the number of ways to make each amount.
//The answer is ways[200], printed at the end