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
 * @brief Allocator block with fixed size
 */
struct Allocator {
    char *head;     /* First open address */
    char *tail;     /* End of memory block */
};

#define DEFAULT_CAPACITY (1 << 15)  /* 32KB default capacity */

Allocator *Allocator_new(void)
{
    /* Allocate memory for the structure plus storage */
    Allocator *alloc = calloc(1, DEFAULT_CAPACITY);
    if (alloc) {
        alloc->head = (char *)(alloc + 1);
        alloc->tail = (char *)alloc + DEFAULT_CAPACITY - sizeof(Allocator *);
    }
    return alloc;
}

void *Allocator_push(Allocator *alloc, size_t size)
{
    if (!(alloc && size)) return NULL;  // Handle NULL allocator or 0 size
    if (alloc->tail - alloc->head < size) return NULL;  // Ensure available capacity
    void *ptr = alloc->head;
    alloc->head += size;
    return ptr;
}

void Allocator_pop(Allocator *alloc, size_t size)
{
    if (!(alloc && size)) return;   // Handle NULL allocator or 0 size
    if (alloc->head - (char *)alloc + sizeof(Allocator) < size) return; // Ensure available capacity
}

void Allocator_clear(Allocator *alloc)
{
    if (alloc) alloc->head = (char *)(alloc + 1);
}

size_t Allocator_used(Allocator *alloc)
{
    if (!alloc) return 0;
    return alloc->head - (char *)alloc - sizeof(Allocator);
}