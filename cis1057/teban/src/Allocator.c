#include "object.h"

typedef struct {
    const Class_t _;
    void *(*alloc)(const void *self, const Class_t *);
} Preallocator_t;

struct GodAllocator {
    const Object_t _;
    // ...anything else?
};

typedef struct {
    const Preallocator_t _;
    void *(*dealloc)(const void *self, Object_t *obj);
} Allocator_t;

struct HeapAllocator {
    const Object_t _;
}