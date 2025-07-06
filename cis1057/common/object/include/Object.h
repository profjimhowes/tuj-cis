#ifndef	OBJECT_H
#define	OBJECT_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "Allocator.h"
#include "static_mem.h"

void *new(Allocator_t *mem, const void *class, ...);

const void *Object(void);
const void *Class(void);
const void *Protocol(void);

/**
 * Static methods
 */
const void *getclass(const void *self);
size_t getsize(const void *self);
bool instanceof(const void *self, const void *class);
bool extends(const void *self, const void *class);
const void *implements(const void *self, const void *proto);
void *cast(const void *class, const void *self);
#define CAST(class, obj) ((class##_t *)cast(class, obj))
#define CONST(class, obj) ((const class##_t *)cast(class, obj))

/**
 * Dynamic methods (selectors)
 */
void *init(void *self, va_list *app);
int differ(const void *self, const void *b);
int fput(const void *self, FILE *fp);

const void *super(const void *self);

#endif  /* OBJECT_H */