#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * PROJECT EULER #205
 * Dice Game
 *
 * Peter has nine four-sided (pyramidal) dice, each with faces
 * numbered 1, 2, 3, 4.
 *
 * Colin has six six-sided (cubic) dice, each with faces
 * numbered 1, 2, 3, 4, 5, 6.
 *
 * Peter and Colin roll their dice and compare totals: the highest total
 * wins. The result is a draw if the totals are equal.
 *
 * What is the probability that Pyramidal Peter beats Cubic Colin? Give
 * your answer rounded to seven decimal places in the form 0.abcdefg.
 */

void dice_totals(int dice, int faces, int total_freq[]);

/**
 * Expects four integers as command-line arguments:
 *    1. Number of dice for Peter
 *    2. Faces per die for Peter
 *    3. Number of dice for Colin
 *    4. Faces per die for Colin
 */
int main(int argc, char *argv[]) {
    if (argc < 5) return EXIT_FAILURE;

    int     p_dice = atoi(argv[1]),
            p_faces = atoi(argv[2]),
            p_freqs[p_dice * p_faces + 1],
            c_dice = atoi(argv[3]),
            c_faces = atoi(argv[4]),
            c_freqs[c_dice * c_faces + 1];
    
    double  p_total = pow(p_faces, p_dice),
            c_total = pow(c_faces, c_dice);
    
    dice_totals(p_dice, p_faces, p_freqs);
    dice_totals(c_dice, c_faces, c_freqs);

    // For Colin, we use cumulative frequency: c_freqs[n] is the frequency
    // of rolling AT MOST n on the dice.
    for (int i = 1; i <= c_dice * c_faces; i++)
        c_freqs[i] += c_freqs[i - 1];

    double p_wins = 0;
    // If Peter rolls p, then he wins when Colin rolls p-1 or less.
    // Since we turned Colin's frequencies into cumulative, we have the values
    // we need already and only need to divide by the total outcomes for each.
    for (int p = p_dice; p <= p_dice * p_faces; p++)
        p_wins += p_freqs[p] / p_total * c_freqs[p - 1] / c_total;
    printf("Peter's win rate: %.7lf\n", p_wins);

    return EXIT_SUCCESS;
}

void dice_totals(int dice, int faces, int total_freq[]) {
    // Initialize frequency array
    for (int i = dice * faces; i > 0; i--) total_freq[i] = 0;
    total_freq[0] = 1;

    // Build frequency results one die at a time
    for (int d = 0; d < dice; d++) {
        for (int k = dice * faces; k > d; k--) {
            total_freq[k] = 0;
            for (int j = 1; j <= faces; j++) {
                if (k - j < d) break;
                total_freq[k] += total_freq[k - j];
            }
        }
        total_freq[d] = 0;
    }
}