#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Object.h"
#include "Object_r.h"

/**
 *	Object
 */

/* Statically-linked public methods */

const void *getclass(const void *self) {
	assert(self && C_OBJECT(self)->class);
	return C_OBJECT(self)->class;
}

size_t getsize(const void *self) {
	return C_CLASS(getclass(self))->size;
}

bool instanceof(const void *self, const void *class) {
    return self && getclass(self) == class;
}

bool extends(const void *self, const void *class)

bool implements(const void *self, const void *protocol) {
    return callback(self, cast(Protocol(), protocol)) != NULL;
}

void (*)(void) callback(const void *self, const void *protocol) {
    if (!protocol) return false;
    const Class_t *class = getclass(self);
    const method_t *p = &class->init;
    for (int count = class->methods; count-- > 0; ++p)
        if (p->implements == protocol)
            return p->method ? p->selector | NULL;
}

void *cast(const void *class, const void *self) {
    // not implemented yet
}

/* Dynamically-linked methods */

static void *_init(void *self, va_list *app) {
	return self;
}

static int _equals(const void *self, const void *other) {
	return self == other;
}

static int _fput(const void *self, FILE *fp) {
	return fprintf(fp, "<%s at %p>\n", C_CLASS(getclass(self))->name, self);
}

/*
 *	Class
 */

void *new(Allocator_t *mem, const void *class, ...) {
	assert(class && C_CLASS(class)->size);
	Object_t *object = mem->alloc(C_CLASS(class)->size);
	assert(object);
	object->class = C_CLASS(class);

    va_list ap;
	va_start(ap, class);
	object = init(object, &ap);
	va_end(ap);

	return object;
}

const void *super(const void *self) {
	assert(self && C_CLASS(self)->super);
	return C_CLASS(self)->super;
}

/*
 * Selectors
 */

void *init(void *self, va_list *app) {
    const Class_t *class = getclass(self);
	assert(class->init);
	return class->init(self, app);
}

void *super_init(const void *class, void *self, va_list *app) {
    const Class_t *superclass = super(class);
	assert(self && superclass->init);
	return superclass->init(self, app);
}

bool equals(const void *self, const void *other) {
    const Class_t *class = getclass(self);
	assert(class->equals);
	return class->equals(self, other);
}

bool super_equals(const void *class, const void *self, const void *other) {
    const Class_t *superclass = super(class);
	assert(self && superclass->equals);
	return superclass->equals(self, other);
}

int fput(const void *self, FILE *fp) {
    const Class_t *class = getclass(self);
	assert(class->fput);
	return class->fput(self, fp);
}

int super_fput(const void *class, const void *self, FILE *fp) {
    const Class_t *superclass = super(class);
	assert(self && superclass->fput);
	return superclass->fput(self, fp);
}

static void *Class_init(void *self, va_list *app) {
    const Class_t *super = va_arg(*app, Class_t *);
    assert(super);

    Object_t obj = *OBJECT(SELF);
    size_t supersize = getsize(super);
    memcpy(self, super, supersize);
    CLASS(self)->_ = obj;
    CLASS(self)->super = super;
	CLASS(self)->name = va_arg(*app, char *);
    size_t size = va_arg(*app, size_t);
	CLASS(self)->size = size;
    CLASS(self)->methods += (size - supersize) / sizeof(method_t);

    va_list ap = *app;
	INIT_METHODS(va_arg(ap, void (*)()), CLASS(self), init, equals, fput);

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
	    {NULL, NULL, _init},
        {NULL, NULL, _equals},
        {NULL, NULL, _fput}
	},
	{
        {object + 1},
	    "Class", object, getsize(Class_t), 3,
	    {NULL, NULL, Class_init},
        {NULL, NULL, _equals},
        {NULL, NULL, _fput}
	},
    {
        {object + 1},
	    "Protocol", object, getsize(Protocol_t), 3,
	    {NULL, NULL, Protocol_init},
        {NULL, NULL, _equals},
        {NULL, NULL, _fput}
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