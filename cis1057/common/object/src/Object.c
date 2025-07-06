#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Object.h"
#include "Object_r.h"

/**
 *	Object
 */

/* Statically-linked public methods */
#define castp(class, obj) ((class##_t *)cast(class(), obj))
#define castcp(class, obj) ((const class##_t *)cast(class(), obj))
#define getclassp(obj) ((const Class_t *)getclass(obj))
#define superp(class) ((const Class_t *)super(class))

void *cast(const void *class, const void *self) {
    assert(class && self);
    const Class_t *cp = ((const Object_t *)self)->class;
    while (cp != (const Class_t *)class) {
        assert(cp != Object());
        cp = cp->super;
    }
    return (void *)self;
}

const void *getclass(const void *self) {
	return castcp(Object, self)->class;
}

size_t getsize(const void *self) {
	return getclassp(self)->size;
}

bool instanceof(const void *self, const void *class) {
    return getclass(self) == cast(Class(), class);
}

bool extends(const void *self, const void *class) {
    const Class_t *cp = getclass(self);
    const Class_t *sup = cast(Class(), class);
    if (sup != Object())
        while (cp != sup)
            if (cp != Object()) cp = super(cp);
            else return false;
    return true;
}

bool implements(const void *self, const void *protocol) {
    return callback(self, protocol) != NULL;
}

void (*)(void) callback(const void *self, const void *protocol) {
    cast(Protocol(), protocol);
    const Class_t *class = getclass(self);
    const method_t *p = &class->init;
    for (int count = class->methods; count-- > 0; ++p)
        if (p->implements == protocol)
            return p->method ? p->selector : NULL;
    return NULL;
}

/* Dynamically-linked methods */

static void *_init(void *self, va_list *app) {
	return self;
}

static bool _equals(const void *self, const void *other) {
    if (self && other) return self == other;
    return false;
}

static int _fput(const void *self, FILE *fp) {
	return fprintf(fp, "<%s at %p>\n", getclassp(self)->name, self);
}

/*
 *	Class
 */

void *new(Allocator_t *mem, const void *class, ...) {
    Object_t *obj;
	const Class_t *cp = cast(Class(), class);
	assert(obj = mem->alloc(cp->size));
	obj->class = cp;

    va_list ap;
	va_start(ap, class);
	obj = init(obj, &ap);
	va_end(ap);

	return obj;
}

const void *super(const void *self) {
	return castcp(Class, self)->super;
}

/*
 * Selectors
 */

void *init(void *self, va_list *app) {
	return getclassp(self)->init(self, app);
}

void *super_init(const void *class, void *self, va_list *app) {
	return superp(class)->init->method(self, app);
}

bool equals(const void *self, const void *other) {
	return getclassp(self)->equals->method(self, other);
}

bool super_equals(const void *class, const void *self, const void *other) {
	return superp(class)->equals->method(self, other);
}

int fput(const void *self, FILE *fp) {
	return getclassp(self)->fput->method(self, fp);
}

int super_fput(const void *class, const void *self, FILE *fp) {
	return superp(class)->fput->method(self, fp);
}

static void *Class_init(void *self, va_list *app) {
    const Class_t *super = va_arg(*app, Class_t *);
    assert(super);

    Object_t obj = *(Object_t *)self;
    size_t supersize = getsize(super);
    memcpy(self, super, supersize);
    ((Class_t *)self)->_ = obj;
    ((Class_t *)self)->super = super;
	assert(((Class_t *)self)->name = va_arg(*app, char *));
    size_t size = va_arg(*app, size_t);
	((Class_t *)self)->size = size;
    ((Class_t *)self)->methods += (size - supersize) / sizeof(method_t);

    va_list ap = *app;
	INIT_METHODS(va_arg(ap, void (*)()), ((Class_t *)self), init, equals, fput);

	return self;
}

/**
 * Protocol
 */

static void *Protocol_init(void *self, va_list *app) {
    self = super_init(Protocol(), self, app);
    PROTOCOL(self)->selector = va_arg(*app, void (*)(void));
}

/*
 * Static initialization
 */

static const Class_t object[] = {
	{
        {object + 1},
	    "Object", object, getsize(Object_t), 3,
	    {init, NULL, _init},
        {equals, NULL, _equals},
        {fput, NULL, _fput}
	},
	{
        {object + 1},
	    "Class", object, getsize(Class_t), 3,
	    {init, NULL, Class_init},
        {equals, NULL, _equals},
        {fput, NULL, _fput}
	},
    {
        {object + 1},
	    "Protocol", object, getsize(Protocol_t), 3,
	    {init, NULL, Protocol_init},
        {equals, NULL, _equals},
        {fput, NULL, _fput}
	}
};

const void *const Object(void) {
    return object;
};

const void *const Class(void) {
    return object + 1;
}

const void *const Protocol(void) {
    return object + 2;
}