extern const void *Object;
extern const void *Class;

void *new(const void *class, ...);
void delete(void *self);
bool equals(const void *self, const void *other)

const void *getClass(const void *self);
bool instanceOf(const void *obj, const *class);
bool extends(const void *obj, const *class);
bool implements(const void *self, const Protocol_t *protocol);
void *cast(const void *class, const void *self);
