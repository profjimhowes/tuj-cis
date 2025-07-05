#include "Object.h"

typedef struct {
    const Class_t *class;
} Object_t;

typedef struct {
    const Object_t _;
    const char *name;
    const Class_t *super;
    ptrdiff_t size;
    void *(*init)(void *self, va_list *app);
} Class_t;