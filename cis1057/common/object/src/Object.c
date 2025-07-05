#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Object.h"
#include "Object_r.h"

#define OBJECT(o) ((Object_t *)o)
#define C_OBJECT(o) ((const Object_t *)o)
#define CLASS(c) ((Class_t *)c)
#define C_CLASS(c) ((const Class_t *)c)

/*
 *	Object
 */

static void *_init(void *self, va_list *app) {
	return self;
}

static int _equals(const void *self, const void *other) {
	return self == other;
}

static int _fput(const void *self, FILE *fp) {
	return fprintf(fp, "<%s> at %p\n", C_CLASS(classOf(self))->name, self);
}

const void *classOf(const void *self) {
	assert(self && C_OBJECT(self)->class);
	return C_OBJECT(self)->class;
}

size_t sizeOf(const void *self) {
	return C_CLASS(classOf(self))->size;
}

/*
 *	Class
 */

#define MATCH_METHODS8(sel, obj, fun, mem, ...) MATCH_METHOD(sel, obj, fun, mem) else MATCH_METHODS7(sel, obj, fun, __VA_ARGS__)
#define MATCH_METHODS7(sel, obj, fun, mem, ...) MATCH_METHOD(sel, obj, fun, mem) else MATCH_METHODS6(sel, obj, fun, __VA_ARGS__)
#define MATCH_METHODS6(sel, obj, fun, mem, ...) MATCH_METHOD(sel, obj, fun, mem) else MATCH_METHODS5(sel, obj, fun, __VA_ARGS__)
#define MATCH_METHODS5(sel, obj, fun, mem, ...) MATCH_METHOD(sel, obj, fun, mem) else MATCH_METHODS4(sel, obj, fun, __VA_ARGS__)
#define MATCH_METHODS4(sel, obj, fun, mem, ...) MATCH_METHOD(sel, obj, fun, mem) else MATCH_METHODS3(sel, obj, fun, __VA_ARGS__)
#define MATCH_METHODS3(sel, obj, fun, mem, ...) MATCH_METHOD(sel, obj, fun, mem) else MATCH_METHODS2(sel, obj, fun, __VA_ARGS__)
#define MATCH_METHODS2(sel, obj, fun, mem, ...) MATCH_METHOD(sel, obj, fun, mem) else MATCH_METHOD(sel, obj, fun, __VA_ARGS__)
#define MATCH_METHOD(sel, obj, fun, mem) if (sel == (void (*)(void))mem) { \
    *(void (**)(void))&obj->mem = fun; \
}
#define MATCH_SELECTOR(a, b, c, d, e, f, g, h, i, ...) i
#define MATCH_METHODS(sel, obj, fun, ...) MATCH_SELECTOR(__VA_ARGS__, \
    MATCH_METHODS8, MATCH_METHODS7, MATCH_METHODS6, MATCH_METHODS5, \
    MATCH_METHODS4, MATCH_METHODS3, MATCH_METHODS2, MATCH_METHOD, \
)(sel, obj, fun, __VA_ARGS__)
#define INIT_METHODS(next_arg, obj, ...) do { \
    void (*selector)(); \
    while (selector = next_arg) { \
        void (*)() fun = next_arg; \
        MATCH_METHODS(selector, obj, fun, __VA_ARGS__); \
    } \
} while (0)

static void *Class_init(void *self, va_list *app) {
    const Class_t *super = va_arg(*app, Class_t *);
    assert(super);

    Object_t obj = *OBJECT(SELF);
    memcpy(self, super, sizeOf(super));
    CLASS(self)->_ = obj;
    CLASS(self)->super = super;
	CLASS(self)->name = va_arg(*app, char *);
	CLASS(self)->size = va_arg(*app, size_t);

    va_list ap = *app;
	INIT_METHODS(va_arg(ap, void (*)()), self, init, equals, fput);

	return self;
}

const void *super(const void *self) {
	assert(self && C_CLASS(self)->super);
	return C_CLASS(self)->super;
}

/*
 *	initialization
 */

static const Class_t object[] = {
	{
        {object + 1},
	    "Object", object, sizeof(Object_t),
	    _init, _equals, _fput
	},
	{
        {object + 1},
	    "Class", object, sizeof(Class_t),
	    Class_init, _equals, _fput
	}
};

const void *Object = object;
const void *Class = object + 1;

/*
 *	object management and selectors
 */

void *new(Allocator *mem, const void *class, ...) {
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

void *init(void *self, va_list *app) {
    const Class_t *class = classOf(self);
	assert(class->init);
	return class->init(self, app);
}

void *super_init(const void *class, void *self, va_list *app) {
    const Class_t *superclass = super(class);
	assert(self && superclass->init);
	return superclass->init(self, app);
}

bool equals(const void *self, const void *other) {
    const Class_t *class = classOf(self);
	assert(class->equals);
	return class->equals(self, other);
}

bool super_equals(const void *class, const void *self, const void *other) {
    const Class_t *superclass = super(class);
	assert(self && superclass->equals);
	return superclass->equals(self, other);
}

int fput(const void *self, FILE *fp) {
    const Class_t *class = classOf(self);
	assert(class->fput);
	return class->fput(self, fp);
}

int super_fput(const void *class, const void *self, FILE *fp) {
    const Class_t *superclass = super(class);
	assert(self && superclass->fput);
	return superclass->fput(self, fp);
}