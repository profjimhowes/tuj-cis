#include <stdio.h>
#include <math.h>

// multiple shape types with polymorphic area calculation

typedef struct {
    double x, y;
} Point;

typedef struct {
    Point center;
    double radius;
} Circle;

typedef struct {
    Point vertices[3];
} Triangle;

typedef struct {
    Point corner1, corner2;
} Rectangle;

enum {CIRCLE, TRIANGLE, RECTANGLE};
typedef struct {
    int tag;
    union {
        Circle circle;
        Triangle triangle;
        Rectangle rectangle;
    } as;
} Shape;

double area(Shape shape) {
    double dx, dy;
    Triangle *t;
    switch (shape.tag) {
        case CIRCLE: // pi times r^2
            return 3.1415926535987 * shape.as.circle.radius * shape.as.circle.radius;
        case TRIANGLE:
            t = &shape.as.triangle;
            dx = t->vertices[0].x * (t->vertices[1].y - t->vertices[2].y);
            dx += t->vertices[1].x * (t->vertices[2].y - t->vertices[0].y);
            dx += t->vertices[2].x * (t->vertices[0].y - t->vertices[1].y);
            return abs(dx) / 2;
        case RECTANGLE:
            dx = shape.as.rectangle.corner2.x - shape.as.rectangle.corner1.x;
            dy = shape.as.rectangle.corner2.y - shape.as.rectangle.corner1.y;
            return dx * dy;
        default: printf("Unrecognized shape");
    }
    return 0.0;
}

int main(int argc, char *argv[]) {
    Shape s;
    s.tag = CIRCLE;
    s.as.circle = (Circle){(Point){0, 0}, 10};
    printf("Area: %lf\n", area(s));
    return 0;
}