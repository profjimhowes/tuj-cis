#include "object.h"

/* Representations */

typedef struct Object {
    const struct Class *class;
} Object_t;

typedef struct Type {
    const Object_t *_;
    const char *name;
    const struct Type *super;
    size_t size;
    void *(*init)(void *self, va_list *app);
} Type_t;

typedef struct Class {
    const Object_t *base;
    const Behavior_t *interface;
} Class_t

void *new(const Allocator_t *mem, const void *class, ...) {
    const Type_t *cp = class;
    Object_t *object = mem->alloc(cp->size);
    assert(object);

    va_list ap;
    va_start(ap, class);
    object->class = cp;
    object = init(object, &ap);
    va_end(ap);
    return object;
}

void *init(void *self, va_list *app) {
    const Type_t *type = typeof(self);
    return type->init(self, app);
}

// Equality protocol, has default (universal) implementation
bool _equals(const void *self, const void *other) {
    return self == other;
}

const void *getClass(const void *self) {
    const Object_t *obj = self;
    assert(obj && obj->class);
    return obj->class;
}

bool instanceOf(const void *self, const Class_t *class) {
    return self && getClass(self) == class;
}

bool extends(const void *self, const Class_t *class) {
    if (!self) return false;
    const Class_t *cp = getClass(self);
    do {
        if (cp == class) return true;
    } while (cp = cp->super);
    return false;
}

bool implements(const void *self, const Protocol_t *protocol) {
    if (!self) return false;
    const Class_t *cp = getClass(self);
    do {
        if (protocol->conforms(cp)) return true;
    } while (cp = cp->super);
    return false;
}

void *cast(const void *type, const void *self) {
    if (instanceOf(type, Class))
        assert(extends(self, type));
    else if (instanceOf(type, Protocol))
        assert(implements(self, type));
    return (void *)self;
}

/* Protocols define dynamically-linked polymorphic functions */
struct Protocol {
    const Class_t _; // extends Class
}

/* Default initializer */

static void *_init(void *self, va_list *app) {
    return self;
}

/* Class methods */

static void *Class_init(void *_self, va_list *app) {
    Class_t *self = _self;
    self->name = va_arg(*app, char *);
    assert(self->super = va_arg(*app, Class_t *));
    self->size = va_arg(*app, size_t);

    // Find starting location of constructor, then copy vtable from superclass
    const size_t offset = offsetof(Class_t, construct);
    memcpy((char *)self + offset, (char *)self->super + offset, sizeOf(self->super) - offset);

    va_list ap = *app; // Copy argument list so that protocols can be shared among constructors
    typedef void (*voidf)();
    voidf protocol;
    while (protocol = va_arg(ap, voidf)) {
        voidf method = va_arg(ap, voidf);
        if (protocol == (voidf) construct)
            *(voidf *)&self->construct = method;
        else if (protocol == (voidf) destruct)
            *(voidf *)&self->destruct = method;
        else if (protocol == (voidf) equals)
            *(voidf *)&self->equals = method;
    }
}

/* Static initialization of God and Class */
static const Class_t god[] = {
    {
        {god + 1},
        "God", god, sizeof(God_t),
        _construct, _equals
    },
    {
        {god + 1},
        "Class", god, sizeof(Class_t),
        Class_construct, _equals
    },

};
const void *God = god;          // description of God class
const void *Class = god + 1;    // description of Class class