/*******************************************************************************
* object.h
* Objects and classes
*******************************************************************************/

#ifndef OBJECT_H
#define OBJECT_H

/* PUBLIC INTERFACE */

extern const void *Object;

void* new(const void *class, ...);
void delete(void *self);
int equals(const void *self, const void *other);

extern const void *Class;

/* PRIVATE REPRESENTATION */

struct Object {
    const struct Class *class; // object description
};

struct Class {
    const struct Object _; // class description
    const int id; // class id
    const struct Class *super; // super class
    size_t size; // instance size
    void* (*ctor)(void *self, va_list *app);
    void* (*dtor)(void *self);
    int (*differ)(const void *self, const void *other);
};

#endif