/**
 * @file Allocator.c
 * @brief Basic linear stack-based allocator with fixed block size
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
    // Allocate memory for the structure plus storage
    Allocator *alloc = calloc(1, DEFAULT_CAPACITY);
    if (alloc) {
        alloc->head = (char *)(alloc + 1);
        alloc->tail = (char *)alloc + DEFAULT_CAPACITY;
    }
    return alloc;
}

void *Allocator_push(Allocator *alloc, ptrdiff_t size)
{
    // Handle NULL allocator or 0 size
    if (!(alloc && size)) return NULL;

    // Ensure available capacity
    if (alloc->tail - alloc->head < size) return NULL;

    void *ptr = alloc->head;
    alloc->head += size;
    return ptr;
}

void Allocator_pop(Allocator *alloc, ptrdiff_t size)
{
    // Handle NULL allocator or 0 size
    if (!(alloc && size)) return;

    // Ensure available capacity
    if (alloc->head - (char *)alloc < size + (ptrdiff_t)sizeof(Allocator)) return;

    alloc->head -= size;
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