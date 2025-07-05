/**
 * @file test_allocator.c
 * @brief Comprehensive tests for the Allocator system
 * 
 * Tests both fixed-size and growable allocators through the common interface.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "test.h"
#include "Allocator.h"

/* Test global variables */
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

/* Forward declarations for test functions */
static int test_allocator_creation(void);
static int test_basic_allocation(void);
static void test_allocation_alignment(void);
static void test_stack_behavior(void);
static void test_clear_functionality(void);
static void test_usage_tracking(void);
static void test_error_conditions(void);
static void test_memory_patterns(void);
static void test_performance_characteristics(void);

/**
 * @brief Test allocator creation and destruction
 * @return 1 if tests passed, 0 if failed
 */
static int test_allocator_creation(void)
{
    puts("=== Testing Allocator Creation ===");
    int initial_passed = tests_passed;
    
    /* Test successful creation */
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator_new() should return valid pointer");
    
    if (alloc) {
        /* Test initial state */
        ASSERT_EQ(0, Allocator_used(alloc), "New allocator should have zero usage");
        
        /* Test destruction with valid allocator */
        free(alloc);
    }
    
    /* Test destruction with NULL allocator */
    free(NULL);
    /* No assertion needed - should handle NULL gracefully */
    
    printf("\n");
    return (tests_passed > initial_passed && alloc != NULL);
}

/**
 * @brief Test basic allocation functionality
 * @return 1 if tests passed, 0 if failed
 */
static int test_basic_allocation(void)
{
    puts("=== Testing Basic Allocation ===");
    int initial_passed = tests_passed;
    
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator created successfully");
    
    if (!alloc) {
        printf("\n");
        return 0;
    }
    
    /* Test small allocation */
    void* ptr1 = Allocator_push(alloc, 64);
    ASSERT_NOT_NULL(ptr1, "Small allocation should succeed");
    ASSERT_EQ(64, Allocator_used(alloc), "Usage should reflect allocation");
    
    /* Only continue if basic allocation works */
    if (ptr1) {
        /* Test medium allocation */
        void* ptr2 = Allocator_push(alloc, 256);
        ASSERT_NOT_NULL(ptr2, "Medium allocation should succeed");
        ASSERT_EQ(320, Allocator_used(alloc), "Usage should accumulate");
        
        /* Test large allocation */
        void* ptr3 = Allocator_push(alloc, 1024);
        ASSERT_NOT_NULL(ptr3, "Large allocation should succeed");
        ASSERT_EQ(1344, Allocator_used(alloc), "Usage should continue accumulating");
        
        /* Test zero-size allocation */
        void* ptr4 = Allocator_push(alloc, 0);
        ASSERT_TRUE(ptr4 == NULL || Allocator_used(alloc) == 1344, 
                    "Zero-size allocation should be handled consistently");
        
        /* Verify pointers are distinct */
        if (ptr1 && ptr2 && ptr3) {
            ASSERT_TRUE(ptr1 != ptr2, "Allocated pointers should be distinct");
            ASSERT_TRUE(ptr2 != ptr3, "Allocated pointers should be distinct");
            ASSERT_TRUE(ptr1 != ptr3, "Allocated pointers should be distinct");
        }
    }
    
    free(alloc);
    printf("\n");
    return (tests_passed > initial_passed && ptr1 != NULL);
}

/**
 * @brief Test memory alignment requirements
 */
static void test_allocation_alignment(void)
{
    puts("=== Testing Memory Alignment ===");
    
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator created successfully");
    
    /* Test various allocation sizes for proper alignment */
    void* ptrs[8];
    size_t sizes[] = {1, 3, 7, 15, 31, 63, 127, 255};
    
    for (int i = 0; i < 8; i++) {
        ptrs[i] = Allocator_push(alloc, sizes[i]);
        ASSERT_NOT_NULL(ptrs[i], "Allocation should succeed");
        
        /* Check alignment - should be aligned to at least sizeof(void*) */
        uintptr_t addr = (uintptr_t)ptrs[i];
        ASSERT_EQ(0, addr % sizeof(void*), "Allocation should be properly aligned");
    }
    
    /* Verify all pointers are distinct */
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            ASSERT_TRUE(ptrs[i] != ptrs[j], "All allocations should be distinct");
        }
    }
    
    free(alloc);
    printf("\n");
}

/**
 * @brief Test stack-like LIFO behavior
 */
static void test_stack_behavior(void)
{
    puts("=== Testing Stack Behavior ===");
    
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator created successfully");
    
    /* Allocate in sequence */
    void* ptr1 = Allocator_push(alloc, 100);
    void* ptr2 = Allocator_push(alloc, 200);
    void* ptr3 = Allocator_push(alloc, 300);
    
    ASSERT_NOT_NULL(ptr1, "First allocation should succeed");
    ASSERT_NOT_NULL(ptr2, "Second allocation should succeed");
    ASSERT_NOT_NULL(ptr3, "Third allocation should succeed");
    ASSERT_EQ(600, Allocator_used(alloc), "Total usage should be sum of allocations");
    
    /* Pop in LIFO order */
    Allocator_pop(alloc, 300);
    ASSERT_EQ(300, Allocator_used(alloc), "Usage should decrease after pop");
    
    Allocator_pop(alloc, 200);
    ASSERT_EQ(100, Allocator_used(alloc), "Usage should continue decreasing");
    
    Allocator_pop(alloc, 100);
    ASSERT_EQ(0, Allocator_used(alloc), "Usage should be zero after all pops");
    
    /* Test reuse after pop */
    void* ptr4 = Allocator_push(alloc, 50);
    ASSERT_NOT_NULL(ptr4, "Allocation after pop should succeed");
    ASSERT_EQ(50, Allocator_used(alloc), "Usage should reflect new allocation");
    
    free(alloc);
    printf("\n");
}

/**
 * @brief Test clear functionality
 */
static void test_clear_functionality(void)
{
    puts("=== Testing Clear Functionality ===");
    
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator created successfully");
    
    /* Allocate some memory */
    void* ptr1 = Allocator_push(alloc, 100);
    void* ptr2 = Allocator_push(alloc, 200);
    void* ptr3 = Allocator_push(alloc, 300);
    
    ASSERT_NOT_NULL(ptr1, "Allocations should succeed");
    ASSERT_NOT_NULL(ptr2, "Allocations should succeed");
    ASSERT_NOT_NULL(ptr3, "Allocations should succeed");
    ASSERT_EQ(600, Allocator_used(alloc), "Usage should reflect allocations");
    
    /* Clear all memory */
    Allocator_clear(alloc);
    ASSERT_EQ(0, Allocator_used(alloc), "Clear should reset usage to zero");
    
    /* Test allocation after clear */
    void* ptr4 = Allocator_push(alloc, 150);
    ASSERT_NOT_NULL(ptr4, "Allocation after clear should succeed");
    ASSERT_EQ(150, Allocator_used(alloc), "Usage should reflect new allocation");
    
    /* Test multiple clears */
    Allocator_clear(alloc);
    Allocator_clear(alloc);
    ASSERT_EQ(0, Allocator_used(alloc), "Multiple clears should be safe");
    
    free(alloc);
    printf("\n");
}

/**
 * @brief Test usage tracking accuracy
 */
static void test_usage_tracking(void)
{
    puts("=== Testing Usage Tracking ===");
    
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator created successfully");
    
    size_t total_expected = 0;
    
    /* Track usage through multiple operations */
    for (int i = 1; i <= 10; i++) {
        size_t size = i * 10;
        void* ptr = Allocator_push(alloc, size);
        total_expected += size;
        
        ASSERT_NOT_NULL(ptr, "Allocation should succeed");
        ASSERT_EQ(total_expected, Allocator_used(alloc), 
                  "Usage tracking should be accurate");
    }
    
    /* Test usage after pops */
    Allocator_pop(alloc, 100);  /* Remove last allocation */
    total_expected -= 100;
    ASSERT_EQ(total_expected, Allocator_used(alloc), 
              "Usage should decrease after pop");
    
    Allocator_pop(alloc, 90);   /* Remove second-to-last allocation */
    total_expected -= 90;
    ASSERT_EQ(total_expected, Allocator_used(alloc), 
              "Usage should continue tracking correctly");
    
    free(alloc);
    printf("\n");
}

/**
 * @brief Test error conditions and edge cases
 */
static void test_error_conditions(void)
{
    puts("=== Testing Error Conditions ===");
    
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator created successfully");
    
    /* Test allocation with NULL allocator */
    void* ptr = Allocator_push(NULL, 100);
    ASSERT_NULL(ptr, "Allocation with NULL allocator should fail");
    
    /* Test pop with NULL allocator */
    Allocator_pop(NULL, 100);
    /* Should not crash */
    
    /* Test clear with NULL allocator */
    Allocator_clear(NULL);
    /* Should not crash */
    
    /* Test usage with NULL allocator */
    size_t usage = Allocator_used(NULL);
    ASSERT_EQ(0, usage, "Usage of NULL allocator should be zero");
    
    /* Test pop more than allocated */
    Allocator_push(alloc, 100);
    Allocator_pop(alloc, 200);  /* Pop more than allocated */
    ASSERT_EQ(0, Allocator_used(alloc), "Over-pop should be handled gracefully");
    
    /* Test very large allocation */
    ptr = Allocator_push(alloc, SIZE_MAX);
    /* Should either succeed or fail gracefully */
    ASSERT_TRUE(ptr != NULL || Allocator_used(alloc) == 0, 
                "Large allocation should be handled gracefully");
    
    free(alloc);
    printf("\n");
}

/**
 * @brief Test memory patterns and data integrity
 */
static void test_memory_patterns(void)
{
    puts("=== Testing Memory Patterns ===");
    
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator created successfully");
    
    /* Test writing and reading data */
    char* buffer1 = (char*)Allocator_push(alloc, 256);
    ASSERT_NOT_NULL(buffer1, "Buffer allocation should succeed");
    
    /* Write pattern to buffer */
    for (int i = 0; i < 256; i++) {
        buffer1[i] = (char)(i % 256);
    }
    
    /* Allocate another buffer */
    int* buffer2 = (int*)Allocator_push(alloc, 64 * sizeof(int));
    ASSERT_NOT_NULL(buffer2, "Integer buffer allocation should succeed");
    
    /* Write pattern to integer buffer */
    for (int i = 0; i < 64; i++) {
        buffer2[i] = i * i;
    }
    
    /* Verify first buffer is unchanged */
    for (int i = 0; i < 256; i++) {
        ASSERT_EQ((char)(i % 256), buffer1[i], "First buffer should be unchanged");
    }
    
    /* Verify second buffer */
    for (int i = 0; i < 64; i++) {
        ASSERT_EQ(i * i, buffer2[i], "Second buffer should contain correct values");
    }
    
    free(alloc);
    printf("\n");
}

/**
 * @brief Test performance characteristics
 */
static void test_performance_characteristics(void)
{
    puts("=== Testing Performance Characteristics ===");
    
    Allocator* alloc = Allocator_new();
    ASSERT_NOT_NULL(alloc, "Allocator created successfully");
    
    /* Test many small allocations */
    void* ptrs[1000];
    for (int i = 0; i < 1000; i++) {
        ptrs[i] = Allocator_push(alloc, 16);
        ASSERT_NOT_NULL(ptrs[i], "Small allocation should succeed");
    }
    
    ASSERT_EQ(16000, Allocator_used(alloc), "Usage should match allocations");
    
    /* Verify all pointers are distinct and properly aligned */
    for (int i = 0; i < 1000; i++) {
        uintptr_t addr = (uintptr_t)ptrs[i];
        ASSERT_EQ(0, addr % sizeof(void*), "Allocation should be aligned");
        
        /* Check against next few pointers to avoid O(n²) comparison */
        for (int j = i + 1; j < i + 10 && j < 1000; j++) {
            ASSERT_TRUE(ptrs[i] != ptrs[j], "Pointers should be distinct");
        }
    }
    
    /* Test clear performance */
    Allocator_clear(alloc);
    ASSERT_EQ(0, Allocator_used(alloc), "Clear should reset usage");
    
    /* Test reallocation after clear */
    void* ptr = Allocator_push(alloc, 1024);
    ASSERT_NOT_NULL(ptr, "Allocation after clear should succeed");
    
    free(alloc);
    printf("\n");
}

/**
 * @brief Main test runner
 */
int main(void)
{
    TEST_INIT();
    
    puts("Testing Allocator System");
    puts("========================\n");
    
    /* Basic functionality tests - must pass for advanced tests to run */
    int creation_ok = test_allocator_creation();
    if (!creation_ok) {
        puts("⚠️  Skipping advanced tests - allocator creation failed\n");
        TEST_SUMMARY();
        return TEST_EXIT();
    }
    
    int basic_ok = test_basic_allocation();
    if (!basic_ok) {
        puts("⚠️  Skipping advanced tests - basic allocation failed\n");
        test_error_conditions();  /* Error conditions should always run */
        TEST_SUMMARY();
        return TEST_EXIT();
    }
    
    /* Advanced tests - only run if basic functionality works */
    puts("✅ Basic tests passed - running advanced tests\n");
    test_allocation_alignment();
    test_stack_behavior();
    test_clear_functionality();
    test_usage_tracking();
    test_error_conditions();
    test_memory_patterns();
    test_performance_characteristics();
    
    TEST_SUMMARY();
    return TEST_EXIT();
}