#ifndef	OBJECT_H
#define	OBJECT_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

extern const void *Object;

void *new(const void *class, ...);

const void *classOf(const void *self);
size_t sizeOf(const void *self);

void *init(void *self, va_list *app);
int differ(const void *self, const void *b);
int fput(const void *self, FILE *fp);

extern const void *Class;

const void *super(const void *self);

#endif  /* OBJECT_H */