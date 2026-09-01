#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "data/euler54.txt"
#define HAND_SIZE 5

typedef struct {
    int value; // 1-13 (1-9, J=10, Q=11, K=12, A=13)
    char suit; // 'S', 'H', 'D', 'C'
} Card;

enum Rank {
    PAIR        = 1 << 24,
    TWOPAIR     = 2 << 24,
    TRIPS       = 3 << 24,
    STRAIGHT    = 4 << 24,
    FLUSH       = 5 << 24,
    FULLHOUSE   = 6 << 24,
    QUADS       = 7 << 24,
    STRTFLUSH   = 8 << 24
};

// Convert card character to value
int card_value(char c) {
    switch (c) {
        case 'T': return 9;
        case 'J': return 10;
        case 'Q': return 11;
        case 'K': return 12;
        case 'A': return 13;
        default:  return c - '0';
    }
}

// Compare function for qsort (descending)
int cmp(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

// Parse 5 cards from tokens into a hand
void parse_hand(char **tokens, Card hand[HAND_SIZE]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        hand[i].value = card_value(tokens[i][0]);
        hand[i].suit = tokens[i][1];
    }
    qsort(hand, HAND_SIZE, sizeof(Card), cmp);
}

// Evaluate hand: returns a 64-bit integer encoding hand rank and tiebreakers
unsigned int eval_hand(Card hand[HAND_SIZE]) {
    int flush = 1, straight = 1;
    int quad = 0, set = 0, pair = 0, pair2 = 0;
    int kickers[HAND_SIZE] = {hand[0].value}, k = 1;
    for (int i = 1; i < 5; i++) {
        straight &= hand[i].value % 13 == hand[i - 1].value - 1;
        flush &= hand[i].suit == hand[i - 1].suit;
        if (hand[i].value == hand[i - 1].value) {
            if (hand[i].value == set)
                quad = hand[i].value, set = 0;
            else if (hand[i].value == pair2)
                set = hand[i].value, pair2 = 0;
            else if (hand[i].value == pair)
                set = hand[i].value, pair = 0;
            else if (pair)
                pair2 = hand[i].value, kickers[--k] = 0;
            else pair = hand[i].value, kickers[--k] = 0;
        } else kickers[k++] = hand[i].value;
    }
    
    if (straight && flush)  return STRTFLUSH
        | kickers[0];
    else if (quad)          return QUADS
        | quad << 4
        | kickers[0];
    else if (set && pair)   return FULLHOUSE
        | set << 4
        | pair;
    else if (flush)         return FLUSH
        | kickers[0] << 16
        | kickers[1] << 12
        | kickers[2] << 8
        | kickers[3] << 4
        | kickers[4];
    else if (straight)      return STRAIGHT
        | kickers[0];
    else if (set)           return TRIPS
        | set << 8
        | kickers[0] << 4
        | kickers[1];
    else if (pair2)         return TWOPAIR
        | pair << 8
        | pair2 << 4
        | kickers[0];
    else if (pair)          return PAIR
        | pair << 12
        | kickers[0] << 8
        | kickers[1] << 4
        | kickers[2];
    else return kickers[0] << 16
        | kickers[1] << 12
        | kickers[2] << 8
        | kickers[3] << 4
        | kickers[4];
}

int main(int argc, char *argv[]) {
    FILE *f = fopen(INPUT_FILE, "r");
    if (!f) { perror("File open failed"); return 1; }
    char line[64], *tokens[10];
    int p1wins = 0;
    while (fgets(line, sizeof(line), f)) {
        int t = 0;
        tokens[t++] = strtok(line, " \n");
        while (tokens[t-1] && t < 10) tokens[t++] = strtok(NULL, " \n");
        if (t < 10) continue;
        Hand h1, h2;
        parse_hand(tokens, &h1);
        parse_hand(tokens+5, &h2);
        unsigned long long s1 = eval_hand(&h1), s2 = eval_hand(&h2);
        if (s1 > s2) p1wins++;
    }
    fclose(f);
    printf("%d\n", p1wins);
    return EXIT_SUCCESS;
}