/**
 * @file Allocator.c
 * @brief Next TDD iteration - implement basic allocation functionality
 * 
 * This implementation adds basic allocation to pass the basic_allocation tests
 * while keeping the stack behavior and other advanced features unimplemented.
 */

#include "Allocator.h"
#include <stdlib.h>
#include <stddef.h>

/**
 * @brief Simple allocator structure with basic memory pool
 */
struct Allocator {
    size_t used;        /* Track bytes allocated */
    size_t capacity;    /* Total capacity of memory pool */
    char *memory;       /* Pointer to memory pool */
};

#define DEFAULT_CAPACITY 8192  /* 8KB default capacity */

Allocator *Allocator_new(void)
{
    /* Allocate memory for the allocator structure */
    Allocator* alloc = malloc(sizeof(Allocator));
    if (!alloc) {
        return NULL;
    }
    
    /* Allocate the memory pool */
    alloc->memory = malloc(DEFAULT_CAPACITY);
    if (!alloc->memory) {
        free(alloc);
        return NULL;
    }
    
    alloc->used = 0;
    alloc->capacity = DEFAULT_CAPACITY;
    return alloc;
}

void Allocator_free(Allocator *allocator)
{
    if (allocator) {
        free(allocator->memory);
        free(allocator);
    }
}

void *Allocator_push(Allocator *allocator, size_t size)
{
    /* Handle NULL allocator */
    if (!allocator) {
        return NULL;
    }
    
    /* Handle zero-size allocation - return NULL for consistency */
    if (size == 0) {
        return NULL;
    }
    
    /* Align size to pointer boundary for proper alignment */
    size_t aligned_size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
    
    /* Check if we have enough space */
    if (allocator->used + aligned_size > allocator->capacity) {
        return NULL;  /* Out of memory */
    }
    
    /* Get pointer to current position */
    void *ptr = allocator->memory + allocator->used;
    
    /* Update used amount */
    allocator->used += aligned_size;
    
    return ptr;
}

void Allocator_pop(Allocator *allocator, size_t size)
{
    /* Still unimplemented - will fail stack behavior tests */
    (void)allocator;
    (void)size;
}

void Allocator_clear(Allocator *allocator)
{
    /* Still unimplemented - will fail clear tests */
    (void)allocator;
}

size_t Allocator_used(Allocator *allocator)
{
    /* Return actual usage or 0 for NULL */
    if (!allocator) {
        return 0;
    }
    return allocator->used;
}