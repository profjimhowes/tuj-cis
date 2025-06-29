#include "object.h"

/* Representations */

typedef struct {
    const struct Class *class;
} Object_t;

typedef struct Class {
    const Object_t _;
    const char *name;
    const struct Class *super;
    size_t size;
    void *(*init)(void *self, va_list *app);
} Class_t;

// Invokes the construct protocol
void *new(const void *class, ...) {
    const Class_t *cp = class;
    assert(cp && cp->size);

    Object_t *object = calloc(1, cp->size);
    assert(object);

    va_list ap;
    va_start(ap, class);
    object->class = cp;
    object = construct(object, &ap);
    va_end(ap);
    return object;
}

// Invokes the destruct protocol
void delete(void *self) {
    if (self) free(destruct(self));
}

// Construction protocol, implemented by all classes automatically
void *construct(void *self, va_list *app) {
    const Class_t *class = getClass(self);
    assert(class->super);
    if (class != class->super) {
        (Object_t *)self->class = class->super;             // hard cast to super class
        self = class->construct(construct(self, app), app); // construct recursively
    } else self = class->construct(self, app);
    assert(self);
    (Object_t *)self->class = class; // restore class
    return self;
}

// Destruction protocol, implemented by all classes automatically
void *destruct(void *self) {
    const Class_t *class = getClass(self);
    assert(class->destruct);
    self = class->destruct(self);

    assert(class->super);
    if (class != class->super) {
        (Object_t *)self->class = class->super;
    }
}

// Universal protocol, implemented by all classes automatically
bool equals(const void *self, const void *other) {
    if (self == other) return true;
    const Class_t * const *cp = self;
    assert(self && *cp && (*cp)->equals);
    return (*cp)->equals(self, other);
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

/* Object methods */

static void *Object_construct(void *self, va_list *app) {
    return self;
}

/* Class methods */

static void *Class_construct(void *_self, va_list *app) {
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

/* Static initialization of Object and Class */
static const Class_t object[] = {
    {
        {object + 1},
        "Object", NULL, sizeof(Object_t),
        Object_construct, Object_destruct, Object_equals
    },
    {
        {object + 1},
        "Class", object, sizeof(Class_t),
        Class_construct, Class_destruct, Object_equals
    }
};
const void *Object = object;        // description of Object class
const void *Class = object + 1;     // description of Class class