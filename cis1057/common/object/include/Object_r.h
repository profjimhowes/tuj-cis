#ifndef OBJECT_R
#define OBJECT_R

/**
 * Class description initializers
 */
#define INIT_CLASS(mem, name, class, super, ...) static const void *_##name; \
const void *const class(void) { \
    return _##name ? _##name : _##name = new(mem, \
        class(), #name, super(), sizeof(name##_t), \
        init, name##_init, \
        __VA_ARGS__); \
}

#define INIT_METACLASS(mem, name, ...) INIT_CLASS(mem, name##Class, Class, Class, __VA_ARGS__)

/**
 * Class description method initializers
 */
#define MATCH_METHODS8(obj, sel, pro, met, mem, ...) MATCH_METHOD(obj, sel, pro, met, mem) else MATCH_METHODS7(obj, sel, pro, met, __VA_ARGS__)
#define MATCH_METHODS7(obj, sel, pro, met, mem, ...) MATCH_METHOD(obj, sel, pro, met, mem) else MATCH_METHODS6(obj, sel, pro, met, __VA_ARGS__)
#define MATCH_METHODS6(obj, sel, pro, met, mem, ...) MATCH_METHOD(obj, sel, pro, met, mem) else MATCH_METHODS5(obj, sel, pro, met, __VA_ARGS__)
#define MATCH_METHODS5(obj, sel, pro, met, mem, ...) MATCH_METHOD(obj, sel, pro, met, mem) else MATCH_METHODS4(obj, sel, pro, met, __VA_ARGS__)
#define MATCH_METHODS4(obj, sel, pro, met, mem, ...) MATCH_METHOD(obj, sel, pro, met, mem) else MATCH_METHODS3(obj, sel, pro, met, __VA_ARGS__)
#define MATCH_METHODS3(obj, sel, pro, met, mem, ...) MATCH_METHOD(obj, sel, pro, met, mem) else MATCH_METHODS2(obj, sel, pro, met, __VA_ARGS__)
#define MATCH_METHODS2(obj, sel, pro, met, mem, ...) MATCH_METHOD(obj, sel, pro, met, mem) else MATCH_METHOD(obj, sel, pro, met, __VA_ARGS__)
#define MATCH_METHOD(obj, sel, pro, met, mem) if (sel == (void (*)(void))mem) { \
    obj->mem.selector = sel; \
    obj->mem.implements = pro; \
    obj->mem.method = met; \
}
#define MATCH_SELECTOR(a, b, c, d, e, f, g, h, i, ...) i
#define MATCH_METHODS(obj, sel, pro, met, ...) MATCH_SELECTOR(__VA_ARGS__, \
    MATCH_METHODS8, MATCH_METHODS7, MATCH_METHODS6, MATCH_METHODS5, \
    MATCH_METHODS4, MATCH_METHODS3, MATCH_METHODS2, MATCH_METHOD, \
)(obj, sel, pro, met, __VA_ARGS__)
#define INIT_METHODS(args, object, ...) do { \
    void (*selector)(); \
    void (*method)(); \
    while (selector = va_arg(args, void (*)(void))) { \
        assert(method = va_arg(args, void (*)(void))); \
        const void *protocol = va_arg(args, const void *); \
        if (protocol) cast(Protocol(), protocol); \
        MATCH_METHODS(object, selector, protocol, method, __VA_ARGS__); \
    } \
} while (0)

/**
 * Metaclass instance initializer
 */
#define METACLASS_INIT(class, ...) static void *class##Class_init(void *self, va_list *app) { \
    self = super_init(class##Class(), self, app); \
    va_list ap = *app; \
	INIT_METHODS(ap, ((class##Class_t)self), __VA_ARGS__); \
	return self; \
}

/**
 * Casts
 */
#define OBJECT(o) ((Object_t *)o)
#define C_OBJECT(o) ((const Object_t *)o)
#define CLASS(c) ((Class_t *)c)
#define C_CLASS(c) ((const Class_t *)c)

/**
 * Object, Class, Protocol concrete data types
 */
typedef struct Object {
	const struct Class *class;
} Object_t;

typedef struct Protocol {
    const Object_t _;
    void (*selector)(void);
} Protocol_t;

typedef struct method {
    void (*selector)(void);
    const void *implements;
    void (*method)(void);
} method_t;

typedef struct Class {
	const Object_t _;
    const Class_t *super;
	const char *name;
	size_t size;
    int methods;
    method_t init;      // void *(*init)(void *self, va_list *app)
    method_t equals;    // int (*equals)(const void *self, const void *other)
    method_t fput;      // int (*fput)(const void *self, FILE *fp)
} Class_t;

void *super_init(const void *class, void *self, va_list *app);
int super_equals(const void *class, const void *self, const void *other);
int super_fput(const void *class, const void *self, FILE *fp);

#endif  /* OBJECT_R */