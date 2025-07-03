/**
 * @file test.h
 * @brief Simple testing framework for CIS 1057 projects
 * 
 * This header provides basic testing macros for unit testing.
 * It's designed to be lightweight and easy to understand for
 * introductory C programming students.
 */

#ifndef TEST_H
#define TEST_H

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
#define COLOR_TEXT(color, message) color message COLOR_RESET
#define GREEN_TEXT(message) COLOR_TEXT(COLOR_GREEN, message)
#define RED_TEXT(message) COLOR_TEXT(COLOR_RED, message)
#define PASS(message) printf(GREEN_TEXT("PASS") ": %s\n", message)
#define FAIL(message) printf(RED_TEXT("FAIL") ": %s\n", message)

/**
 * @brief Initialize test framework
 */
#define TEST_INIT() do { \
    tests_run = 0; \
    tests_passed = 0; \
    tests_failed = 0; \
    puts("Starting tests...\n"); \
} while(0)

/**
 * @brief Print final test results
 */
#define TEST_SUMMARY() do { \
    puts("\n=== Test Summary ==="); \
    printf("Tests run: %d\n", tests_run); \
    printf(GREEN_TEXT("Passed: %d") "\n", tests_passed); \
    printf(RED_TEXT("Failed: %d") "\n", tests_failed); \
    if (tests_failed == 0) { \
        puts(GREEN_TEXT("All tests passed!")); \
    } else { \
        puts(RED_TEXT("Some tests failed.")); \
    } \
} while(0)

/**
 * @brief Return appropriate exit code based on test results
 */
#define TEST_EXIT() (tests_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE)

/* ASSERTIONS */

#define ASSERT(condition, message, diff) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        PASS(message); \
    } else { \
        tests_failed++; \
        FAIL(message); \
        diff; \
    } \
} while(0)

#define LITERAL_DIFF(expected, actual) puts("  Expected: " expected ", Actual: " actual)
#define STRING_DIFF(expected, actual) printf("  Expected: \"%s\", Actual: \"%s\"", expected, actual)
#define VALUE_DIFF(format, type, expected, actual) printf("  Expected: " format ", Actual: " format "\n", (type)(expected), (type)(actual))
#define INT_DIFF(expected, actual) VALUE_DIFF("%d", int, expected, actual)

/**
 * @brief Assert that a condition is true
 */
#define ASSERT_TRUE(condition, message) ASSERT(condition, message, LITERAL_DIFF("true", "false"))

/**
 * @brief Assert that a condition is false
 */
#define ASSERT_FALSE(condition, message) ASSERT(!(condition), message, LITERAL_DIFF("false", "true"))

/**
 * @brief Assert that a pointer is not NULL
 */
#define ASSERT_NOT_NULL(pointer, message) ASSERT((pointer) != NULL, message, LITERAL_DIFF("non-NULL pointer", "NULL"))

/**
 * @brief Assert that a pointer is NULL
 */
#define ASSERT_NULL(pointer, message) ASSERT((pointer) == NULL, message, LITERAL_DIFF("NULL pointer", "non-NULL"))

/**
 * @brief Assert that two integers are equal
 */
#define ASSERT_EQ(expected, actual, message) ASSERT((expected) == (actual), message, INT_DIFF(expected, actual))

/**
 * @brief Assert that two integers are not equal
 */
#define ASSERT_NE(not_expected, actual, message) do { \
    tests_run++; \
    if ((not_expected) != (actual)) { \
        tests_passed++; \
        PASS(message); \
    } else { \
        tests_failed++; \
        FAIL(message); \
        printf("  Expected NOT: %d, But got: %d\n", (int)(not_expected), (int)(actual)); \
    } \
} while(0)

/**
 * @brief Assert that two strings are equal
 */
#define ASSERT_STR_EQ(expected, actual, message) ASSERT(!strcmp((expected), (actual)), message, STRING_DIFF(expected, actual))

#endif /* TEST_H */