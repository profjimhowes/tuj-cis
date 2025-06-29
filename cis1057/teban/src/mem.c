#include "object.h"

void *new(const Preallocator_t *mem, const void *class, ...) {
    const Class_t *cp = class;
    assert(cp);

    Object_t *object = mem->alloc(cp);
    assert(object);

    va_list ap;
    va_start(ap, class);
    object->class = cp;
    object = init(object, &ap);
    va_end(ap);
    return object;
}