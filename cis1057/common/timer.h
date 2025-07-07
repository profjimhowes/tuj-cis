/**
 * @file timer.h
 * @brief Simple high-precision timing utilities for performance measurement
 * 
 * This header provides lightweight timing functions and macros for measuring
 * code execution time with microsecond precision using gettimeofday().
 */

#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <stdio.h>

/**
 * @brief Get current time as a floating-point number of seconds
 * 
 * Returns the current time as seconds since the Unix epoch (January 1, 1970)
 * with microsecond precision. This function is suitable for calculating
 * elapsed time by taking the difference between two calls.
 * 
 * @return Current time in seconds (floating-point with microsecond precision)
 * 
 * @note This function uses gettimeofday() internally, which provides
 *       microsecond resolution on most Unix-like systems including macOS.
 */
double get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

/**
 * @brief Macro to time a code block and store the elapsed time
 * 
 * This macro executes the given code block and stores the elapsed execution
 * time in the specified variable. The elapsed time is measured in seconds
 * with microsecond precision.
 * 
 * @param elapsed Variable to store the elapsed time (should be type double)
 * @param code Code block to time (can be a single statement or compound statement)
 * 
 * @note The elapsed variable will be overwritten with the timing result.
 *       Make sure it's declared as a double before using this macro.
 * 
 * @example
 * @code
 * double time_taken;
 * TIMER(time_taken,
 *     for (int i = 0; i < 1000000; i++) {
 *         // some computation
 *     }
 * );
 * printf("Loop took %.6f seconds\n", time_taken);
 * @endcode
 */
#define TIMER(elapsed, code) do { \
    elapsed = get_time(); \
    { code } \
    elapsed = get_time() - elapsed; \
} while(0)

/**
 * @brief Macro to time a code block and automatically print the elapsed time
 * 
 * This macro executes the given code block and immediately prints the
 * elapsed execution time to stdout. The time is displayed in seconds
 * with 6 decimal places of precision.
 * 
 * @param code Code block to time and print (can be a single statement or compound statement)
 * 
 * @note Uses an internal variable __time__ to avoid naming conflicts.
 *       The output format is: "Elapsed time: X.XXXXXX seconds"
 * 
 * @example
 * @code
 * PRINT_ELAPSED({
 *     bubble_sort(array, size);
 * });
 * // Output: Elapsed time: 0.001234 seconds
 * 
 * PRINT_ELAPSED(
 *     result = fibonacci(40);
 * );
 * // Output: Elapsed time: 0.567890 seconds
 * @endcode
 */
#define PRINT_ELAPSED(code) do { \
    double __time__; \
    TIMER(__time__, code); \
    printf("Elapsed time: %9.6lf seconds\n", __time__); \
} while(0)

#endif /* TIMER_H */