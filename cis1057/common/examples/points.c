#include <stdio.h>
#include <math.h>

typedef struct {
    double x, y;
} Point2D;

typedef struct {
    double x, y, z;
} Point3D;

typedef struct {
    int dimensions; // 2 or 3
    union {
        Point2D p2;
        Point3D p3;
    } as;
} Point;

double distance(Point p1, Point p2) {
    if (p1.dimensions != p2.dimensions) {
        printf("Can't handle unequal dimensions\n");
        return 0.0;
    }
    double dx = 0.0, dy = 0.0, dz = 0.0;
    switch (p1.dimensions) {
        case 2: // handle 2d distance
            dx = p1.as.p2.x - p2.as.p2.x;
            dy = p1.as.p2.y - p2.as.p2.y;
            break;
        case 3: // handle 3d distance
            dx = p1.as.p3.x - p2.as.p3.x;
            dy = p1.as.p3.y - p2.as.p3.y;
            dz = p1.as.p3.z - p2.as.p3.z;
            break;
        default: printf("Can't handle other dimensions\n");
    }
    return sqrt(dx * dx + dy * dy + dz * dz);
}

int main(int argc, char *argv[]) {
    Point p1, p2;
    p1.dimensions = 4;
    p1.as.p3 = (Point3D){3.4, -7.2, 1.2};
    p2.dimensions = 4;
    p2.as.p3 = (Point3D){6.4, -3.2, -9.3};

    printf("Distance between p1 and p2: %lf\n", distance(p1, p2));
}