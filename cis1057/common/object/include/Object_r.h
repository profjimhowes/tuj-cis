#ifndef OBJECT_R
#define OBJECT_R

typedef struct Object {
	const struct Class *class;
} Object_t;

typedef struct Class {
	const struct Object _;
	const char *name;
	const struct Class *super;
	size_t size;
	void *(*init)(void *self, va_list *app);
	int (*equals)(const void *self, const void *other);
	int (*fput)(const void *self, FILE *fp);
} Class_t;

void *super_init(const void *class, void *self, va_list *app);
int super_equals(const void *class, const void *self, const void *other);
int super_fput(const void *class, const void *self, FILE *fp);

#endif  /* OBJECT_R */