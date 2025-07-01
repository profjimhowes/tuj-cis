/**
 * @file Allocator.c
 * @brief Minimal implementation of Allocator to pass creation tests
 * 
 * This implementation creates a valid allocator but still fails
 * allocation tests to demonstrate incremental TDD progress.
 */

#include "Allocator.h"
#include <stdlib.h>
#include <stddef.h>

/**
 * @brief Minimal allocator structure with basic state
 */
struct Allocator {
    size_t used;  /* Track bytes allocated */
};

Allocator *Allocator_new(void)
{
    /* Allocate memory for the allocator structure */
    Allocator* alloc = malloc(sizeof(Allocator));
    if (alloc) {
        alloc->used = 0;  /* Initialize to zero usage */
    }
    return alloc;
}

void Allocator_free(Allocator *allocator)
{
    /* Free the allocator structure (handles NULL gracefully) */
    free(allocator);
}

void *Allocator_push(Allocator *allocator, size_t size)
{
    /* Return NULL to fail all allocation tests */
    (void)allocator;
    (void)size;
    return NULL;
}

void Allocator_pop(Allocator *allocator, size_t size)
{
    /* Do nothing to fail stack behavior tests */
    (void)allocator;
    (void)size;
}

void Allocator_clear(Allocator *allocator)
{
    /* Do nothing to fail clear tests */
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