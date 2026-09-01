
/**
 * @file coroutine_example.c
 * @author John Franks
 * @date Jan 1, 2023
 * @copyright Copyright (c) 2023 John Franks. All rights reserved.
 * @license MIT License
 *
 * @brief Coroutine Example using Duff's Device and Macros
 *
 * C coroutines are an analogy to generators. Both C coroutines generators are
 * used to generate a sequence of results while maintaining their internal state
 * between calls. They are helpful for producing a series of values, particularly
 * in situations where it would be inefficient or unnecessary to generate all the
 * values at once.
 *
 * The C coroutines demonstrated in this example are implemented using macros,
 * which involve defining the coroutine structure and using switch statements to
 * resume execution at the last point where it was suspended The internal state
 * of the coroutine is manually managed, including setting the starting point,
 * the resume point, and updating the internal variables.
 *
 * Although the C coroutine approach shown here is more challenging to work with
 * compared to Python generators, it can offer more fine-grained control in
 * certain situations.
 *
 * This file demonstrates the implementation of coroutines in C using
 * Duff's Device and a set of macros, including COROUTINE_INIT, COROUTINE_BEGIN,
 * COROUTINE_YIELD, and COROUTINE_END. The concepts embodied in this code include:
 *
 * 1. Coroutines: A programming concept that generalizes subroutines for
 *    non-preemptive multitasking by allowing multiple entry points for
 *    suspending and resuming execution at certain locations.
 *
 * 2. Duff's Device: A technique that allows multiple iterations of a loop to
 *    be unrolled and executed in parallel, which can potentially improve
 *    performance.
 *
 * 3. Macros: The C preprocessor provides a mechanism for defining macros,
 *    which are textual substitutions that occur during preprocessing, before
 *    the actual compilation of the code. In this case, macros are used to
 *    implement the coroutine functionality.
 *
 * The example provided is a simple coroutine that calculates and yields doubled
 * values of numbers from 0 to 9. The main function repeatedly calls the
 * coroutine and prints the yielded values until the coroutine is done.
 *
 */

#include <stdio.h>
#include <stdint.h>

/**
 * CoroutineState structure holds the state of the coroutine.
 * @count: A counter used inside the coroutine.
 * @resume_point: A line number used to resume the coroutine from its last yield point.
 */
typedef struct CoroutineState {
    int    resume_point;    // Internal State Data
    void * data;            // User Data
} CoroutineState_t;

/**
 * Sequence sentinels
 */
#define START_OF_SEQUENCE  0
#define END_OF_SEQUENCE    -1

/**
 * Macro to initialize the CoroutineState_t structure.
 * @s: Pointer to CoroutineState_t structure.
 */
#define COROUTINE_INIT(s) do { (s)->resume_point = START_OF_SEQUENCE; } while (0)

/**
 * Macro to mark the beginning of the coroutine.
 * @s: Pointer to CoroutineState_t structure.
 */
#define COROUTINE_BEGIN(s) switch((s)->resume_point) { case 0 :

/**
 * Macro to yield from the coroutine.
 * @s: Pointer to CoroutineState_t structure.
 * @z: Value to return.
 */
#define COROUTINE_YIELD(s, z) (s)->resume_point = __LINE__; return (z); case __LINE__ :

/**
 * Macro to mark the end of the coroutine.
 * @s: Pointer to CoroutineState_t structure.
 */
#define COROUTINE_END(s) default: (s)->resume_point = END_OF_SEQUENCE; break; }

/**
 * Return value to indicate end of generation sequence
 */
#define COROUTINE_SEQUENCE_COMPLETE(s)  ((s)->resume_point == END_OF_SEQUENCE)

/**
 * coroutine_example - A simple example of a coroutine.
 * @state: Pointer to CoroutineState_t structure.
 *
 * This coroutine calculates and yields doubled values of numbers from 0 to 9.
 */
int32_t coroutine_example(CoroutineState_t *state) {
    COROUTINE_BEGIN(state)
        for (*(int32_t *)state->data = 0; *(int32_t *)state->data < 100;) {
            COROUTINE_YIELD(state, *(int32_t *)state->data + 1); /* Coroutine yields here */
            (*(int32_t *)state->data)++;
        }
    COROUTINE_END(state)
}

/**
 * main - The main function that demonstrates the coroutine mechanism.
 *
 * Calls the coroutine_example function repeatedly and prints the yielded values
 * until the coroutine is done.
 */
int main() {
    CoroutineState_t state;
    int32_t state_data;
    int32_t value;

    state_data = 0;
    COROUTINE_INIT(&state);
    state.data = (void *)&state_data;
    printf("Output: %d\n", coroutine_example(&state));
    printf("Output: %d\n", coroutine_example(&state));
    printf("Output: %d\n", coroutine_example(&state));
    return 0;
}