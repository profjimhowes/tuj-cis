/**
 * @file test.h
 * @brief Simple testing framework for Teban project
 * 
 * This header provides basic testing macros for unit testing.
 * It's designed to be lightweight and easy to understand for
 * introductory C programming students.
 */

#ifndef TEBAN_TEST_H
#define TEBAN_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Global test counters */
extern int tests_run;
extern int tests_passed;
extern int tests_failed;

/** Colors for output (if terminal supports them) */
#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_RESET "\033[0m"

/**
 * @brief Initialize test framework
 */
#define TEST_INIT() do { \
    tests_run = 0; \
    tests_passed = 0; \
    tests_failed = 0; \
    printf("Starting tests...\n\n"); \
} while(0)

/**
 * @brief Print final test results
 */
#define TEST_SUMMARY() do { \
    printf("\n=== Test Summary ===\n"); \
    printf("Tests run: %d\n", tests_run); \
    printf(COLOR_GREEN "Passed: %d" COLOR_RESET "\n", tests_passed); \
    printf(COLOR_RED "Failed: %d" COLOR_RESET "\n", tests_failed); \
    if (tests_failed == 0) { \
        printf(COLOR_GREEN "All tests passed!" COLOR_RESET "\n"); \
    } else { \
        printf(COLOR_RED "Some tests failed." COLOR_RESET "\n"); \
    } \
} while(0)

/**
 * @brief Return appropriate exit code based on test results
 */
#define TEST_EXIT() (tests_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE)

/**
 * @brief Assert that a condition is true
 */
#define ASSERT_TRUE(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "FAIL" COLOR_RESET ": %s\n", message); \
        printf("  Expected: true, Got: false\n"); \
    } \
} while(0)

/**
 * @brief Assert that a condition is false
 */
#define ASSERT_FALSE(condition, message) do { \
    tests_run++; \
    if (!(condition)) { \
        tests_passed++; \
        printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "FAIL" COLOR_RESET ": %s\n", message); \
        printf("  Expected: false, Got: true\n"); \
    } \
} while(0)

/**
 * @brief Assert that two integers are equal
 */
#define ASSERT_EQ(expected, actual, message) do { \
    tests_run++; \
    if ((expected) == (actual)) { \
        tests_passed++; \
        printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "FAIL" COLOR_RESET ": %s\n", message); \
        printf("  Expected: %d, Got: %d\n", (int)(expected), (int)(actual)); \
    } \
} while(0)

/**
 * @brief Assert that two integers are not equal
 */
#define ASSERT_NE(not_expected, actual, message) do { \
    tests_run++; \
    if ((not_expected) != (actual)) { \
        tests_passed++; \
        printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "FAIL" COLOR_RESET ": %s\n", message); \
        printf("  Expected NOT: %d, But got: %d\n", (int)(not_expected), (int)(actual)); \
    } \
} while(0)

/**
 * @brief Assert that two strings are equal
 */
#define ASSERT_STR_EQ(expected, actual, message) do { \
    tests_run++; \
    if (strcmp((expected), (actual)) == 0) { \
        tests_passed++; \
        printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "FAIL" COLOR_RESET ": %s\n", message); \
        printf("  Expected: \"%s\", Got: \"%s\"\n", (expected), (actual)); \
    } \
} while(0)

/**
 * @brief Assert that a pointer is not NULL
 */
#define ASSERT_NOT_NULL(pointer, message) do { \
    tests_run++; \
    if ((pointer) != NULL) { \
        tests_passed++; \
        printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "FAIL" COLOR_RESET ": %s\n", message); \
        printf("  Expected: non-NULL pointer, Got: NULL\n"); \
    } \
} while(0)

/**
 * @brief Assert that a pointer is NULL
 */
#define ASSERT_NULL(pointer, message) do { \
    tests_run++; \
    if ((pointer) == NULL) { \
        tests_passed++; \
        printf(COLOR_GREEN "PASS" COLOR_RESET ": %s\n", message); \
    } else { \
        tests_failed++; \
        printf(COLOR_RED "FAIL" COLOR_RESET ": %s\n", message); \
        printf("  Expected: NULL pointer, Got: non-NULL\n"); \
    } \
} while(0)

#endif /* TEBAN_TEST_H */