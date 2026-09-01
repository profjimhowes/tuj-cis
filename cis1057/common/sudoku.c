/******************************************
 * CIS 1057 - Spring 2025                 *
 * ASSIGNMENT 4: Sudoku puzzle generator. *
 ******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GROUP       3
#define LINE        (GROUP * GROUP)
#define BOARD       (LINE * LINE)
#define MASKS       (LINE * 3)
#define BOARD_SZ    sizeof(int[BOARD + MASKS])

#define ROW(i)      (i / LINE)
#define COL(i)      (i % LINE)
#define GRP(i)      (ROW(i) / GROUP * GROUP + COL(i) / GROUP)
#define CELLDATA(i) row = ROW(i), col = COL(i), grp = GRP(i)

#define MASK_CLIP       (1 << LINE)
#define ROW_MASK(b, r)  b[BOARD + r]
#define COL_MASK(b, c)  b[BOARD + LINE + c]
#define GRP_MASK(b, g)  b[BOARD + LINE + LINE + g]
#define GET_MASK(b, i) \
    ((ROW_MASK(b, ROW(i)) | COL_MASK(b, COL(i)) | GRP_MASK(b, GRP(i))) >> 1)
#define INVERT_MASK(m)  (unsigned) ~m % MASK_CLIP
#define APPLY_MASK(b, i, m) \
    ROW_MASK(b, ROW(i)) ^= m, \
    COL_MASK(b, COL(i)) ^= m, \
    GRP_MASK(b, GRP(i)) ^= m

#define FILL_CELL(b, i, v) \
    b[i] = v, APPLY_MASK(b, i, 1 << (v))
#define CLEAR_CELL(b, i) \
    APPLY_MASK(b, i, 1 << b[i]), b[i] = 0

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void print_row(int row[]) {
    for (int i = 0; i < LINE; i++) {
        if (i && i % GROUP == 0) printf("  |");
        switch (row[i]) {
            case -1: printf("  X"); break;
            case  0: printf("  -"); break;
            default: printf("%3d", row[i]); break;
        }
    }
    printf("\n");
}

void print_board(int *board) {
    for (int r = 0; r < LINE; r++) {
        if (r && r % GROUP == 0) {
            for (int i = 0; i < LINE + GROUP - 1; i++)
                printf(i % (GROUP + 1) == GROUP ? "--+" : "---");
            printf("--\n");
        }
        print_row(board + r * LINE);
    }
}

void print_mask_row(int row[]) {
    for (int i = 0; i < LINE; i++) printf("%3x", row[i]);
    printf("\n");
}

void print_masks(int *board) {
    for (int i = 0; i < 3; i++) print_mask_row(board + BOARD + i * LINE);
}

void print_mask(int mask) {
    for (int i = 0; i < LINE; i++) {
        mask >>= 1;
        if (mask % 2) printf("  -");
        else printf("%3d", i + 1);
    }
}

/**
 *  Generates a random permutation of the given integer array using the
 *  Fisher-Yates shuffle algorithm.
 *
 *  @param arr array to store the resulting permutation
 *  @param size size of the array
 */
void shuffle(int arr[], int size) {
    for (int i = size; i > 1; i--) swap(&arr[i - 1], &arr[rand() % i]);
}

/**
 *  Initializes an array with the positive integers in order starting from
 *  the given offset.
 *
 *  @param arr array to initialize
 *  @param size size of the array
 *  @param offset starting value
 */
void indices(int arr[], int size, int offset) {
    for (int i = 0; i < size; i++) arr[i] = i + offset;
}

/**
 *  Initializes an array with a random permutation of integers, starting from
 *  the given offset.
 *
 *  @param arr array to initialize
 *  @param size size of the array
 *  @param offset lowest value
 */
void shuffled_indices(int arr[], int size, int offset) {
    for (int i = 0; i < size; i++) {
        int j = rand() % (i + 1);
        arr[i] = arr[j];
        arr[j] = i + offset;
    }
}

/**
 *  Shuffles the values across the sudoku board as a whole.
 *
 *  @param board references a valid sudoku board to shuffle
 */
int * shuffle_values(int board[]) {
    int values[LINE];
    shuffled_indices(values, LINE, 1);
    for (int i = 0; i < BOARD; i++) board[i] = values[board[i] - 1];
    return board;
}

void grouped_shuffle(int shuffled[]) {
    int groups[GROUP], lines[GROUP];
    shuffled_indices(groups, GROUP, 0);
    indices(lines, GROUP, 0);
    for (int i = 0; i < GROUP; i++) {
        shuffle(lines, GROUP);
        for (int j = 0; j < GROUP; j++)
            shuffled[i * GROUP + j] = groups[i] * GROUP + lines[j];
    }
}

/**
 *  Clears the sudoku board provided as input, or allocates a new board if the
 *  input is NULL.
 *
 *  @param p pointer to sudoku board previously returned by this function
 */
void * new_board(void *p) {
    return memset(p ? : malloc(BOARD_SZ), 0, BOARD_SZ);
}

void * copy_board(void *src) {
    return memcpy(malloc(BOARD_SZ), src, BOARD_SZ);
}

/**
 *  Shuffles the locations of the values from a given starting board in a way
 *  that respects the sudoku invariants. That is, each group of rows and
 *  columns are shuffled, and the groups are shuffled. However, a row or column
 *  may not change groups as a result of the shuffling or the board may become
 *  invalid.
 *
 *  @param board a complete sudoku board to shuffle
 */
int * shuffle_positions(int board[]) {
    int *copy = copy_board(board);
    int row[LINE], col[LINE];
    grouped_shuffle(row);
    grouped_shuffle(col);
    for (int i = 0; i < LINE; i++) {
        board[LINE][i] = copy[LINE][row[i]],
        board[LINE + 1][i] = copy[LINE + 1][col[i]];
        board[LINE + 2][i] = copy[LINE + 2][row[i] / 3 + col[i] % 3];
        for (int j = 0; j < LINE; j++)
            board[i] = copy[row[ROW(i)]][col[COL(i)]];
    }
    free(copy);
    return (int *) board;
}

/**
 *  Generates a valid sudoku solution by shuffling the positions and values of
 *  a starting reference solution.
 *
 *  @param ref starting reference solution which will be shuffled
 */
int * shuffle_board(int board[][LINE]) {
    return shuffle_values(shuffle_positions(board));
}

int * generate_solution(int board[]) {
    board = new_board(board);
    shuffled_indices(board, LINE, 1);
    for (int i = 0; i < LINE; i++) FILL_CELL(board, i, board[i]);
    int cells[BOARD - LINE];
    short int steps = 9;
    shuffled_indices(cells, BOARD - LINE, LINE);
    for (int i = 0; i < BOARD - LINE; steps++) {
        if (steps < 0) return generate_solution(board);
        int cell = cells[i],
            value = board[cell];
        CLEAR_CELL(board, cell);
        int mask = INVERT_MASK(GET_MASK(board, cell)) >> value++;
        if (mask) {
            while (mask % 2 == 0) mask >>= 1, value++;
            FILL_CELL(board, cell, value);
            for (int j = ++i, k = j; j < BOARD - LINE; j++) {
                cell = cells[j],
                mask = GET_MASK(board, cell) + 1;
                if (mask == MASK_CLIP) { i--; break; }
                for (int v = 1; v < MASK_CLIP; v <<= 1)
                    if (mask + v == MASK_CLIP)
                        swap(&cells[j], &cells[k++]);
            }
        } else i--;
    }
    return board;
}

int solve(int board[], int cell) {
    while (cell < BOARD && board[cell]) cell++;
    if (cell == BOARD) return 1;
    int solutions = 0,
        mask = INVERT_MASK(GET_MASK(board, cell));
    for (int v = 1; mask > 0; v++, mask >>= 1)
        if (mask % 2)
            FILL_CELL(board, cell, v),
            solutions += solve(board, cell + 1),
            CLEAR_CELL(board, cell);
    return solutions;
}

int * fill_fixed(int board[]) {
    for (int i = 0, limit = BOARD; limit > 0; i = ++i % BOARD, limit--) {
        int cell = i % BOARD;
        if (board[cell]) continue;
        int mask = GET_MASK(board, cell) + 1;
        for (int v = 0; v < LINE; v++)
            if (mask + (1 << v) == MASK_CLIP)
                FILL_CELL(board, cell, v + 1),
                limit = BOARD;
    }
    return board;
}

/**
 *  Generates a valid sudoku puzzle from a starting solution.
 *
 *  @param board references an array containing a solved sudoku
 */
int * generate_puzzle(int board[]) {
    int cells[BOARD];
    shuffled_indices(cells, BOARD, 0);
    for (int i = 0; i < BOARD; i++) {
        int cell = cells[i],
            value = board[cell];
        CLEAR_CELL(board, cell);
        if (solve(board, 0) > 1) FILL_CELL(board, cell, value);
    }
    return board;
}

int main() {
    int start = time(NULL);
    srand(start);
    int *board = NULL;
    for (int i = 0; i < 1000; i++) board = generate_solution(board);
    printf("Elapsed time: %d s\n", time(NULL) - start);
    free(board);
    return 0;
}