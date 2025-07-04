/**
 * @file Allocator.h
 * @brief Linear stack-based memory allocator for hierarchical object lifetimes
 * 
 * This allocator provides a simple stack-based memory management system that
 * eliminates the need for individual object destructors by managing memory
 * in hierarchical lifetime scopes (program, session, turn, temporary).
 */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

/**
 * @brief Opaque allocator structure
 * 
 * The Allocator maintains a contiguous memory region and tracks the current
 * allocation position, providing fast O(1) allocation and deallocation.
 */
typedef struct Allocator Allocator;

/**
 * @brief Create a new allocator instance
 * 
 * Creates a new linear allocator with an initial memory pool. The allocator
 * uses a simple bump-pointer strategy for fast allocation.
 * 
 * @return Pointer to new allocator instance, or NULL on failure
 */
Allocator *Allocator_new(void);

/**
 * @brief Allocate memory from the allocator
 * 
 * Allocates a contiguous block of memory from the allocator's memory pool.
 * Memory is allocated using a simple bump-pointer strategy for O(1) performance.
 * The returned memory is uninitialized.
 * 
 * @param allocator Allocator to allocate from
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL if allocation fails
 */
void *Allocator_push(Allocator *alloc, ptrdiff_t size);

/**
 * @brief Deallocate the most recently allocated memory
 * 
 * Releases the specified number of bytes from the top of the allocation stack.
 * This operation must match the size of the most recent allocation(s) in LIFO order.
 * 
 * @param allocator Allocator to deallocate from
 * @param size Number of bytes to deallocate (must match recent allocations)
 */
void Allocator_pop(Allocator *alloc, size_t size);

/**
 * @brief Reset allocator to empty state
 * 
 * Deallocates all memory allocated from this allocator, effectively resetting
 * it to its initial empty state. This provides fast bulk deallocation for
 * entire lifetime scopes.
 * 
 * @param allocator Allocator to clear
 */
void Allocator_clear(Allocator *alloc);

/**
 * @brief Get current memory usage
 * 
 * Returns the total number of bytes currently allocated from this allocator.
 * This can be used for memory profiling and debugging.
 * 
 * @param allocator Allocator to query
 * @return Number of bytes currently allocated
 */
size_t Allocator_used(Allocator *alloc);

#endif /* ALLOCATOR_H */