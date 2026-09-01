// to sort, I don't care about the structure of the objects
// I only need to be able to COMPARE them

typedef int (*comparator)(void *, void *); // function pointer

void sort(void *arr[], int n, comparator cmp) {
    // selection sort
    for (int i = 0; i < n; i++) {
        int imin = i;
        for (int j = i + 1; j < n; j++)
            if (cmp(arr[imin], arr[j]) > 0)
                imin = j;
        // swap imin and i
        void *temp = arr[i];
        arr[i] = arr[imin];
        arr[imin] = temp;
    }
}

int int_cmp(void *left, void *right) {
    return *((int *)left) - *((int *)right);
}

int double_cmp(void *left, void *right) {
    double left_double = *((double *)left);
    double right_double = *((double *)right);
    return right_double < left_double - left_double < right_double;
}

typedef struct {
    double x, y, z;
} Point3d;

int point_x_cmp(void *left, void *right) {
    return double_cmp(
        &((Point3d *)left)->x,
        &((Point3d *)right)->x
    );
}

// non void pointer approach
void sort(void *vparr, size_t size, int n, comparator cmp) {
    // selection sort
    char *arr = (char *)vparr;
    for (int i = 0; i < size * n; i += size) {
        int imin = i;
        for (int j = i + size; j < size * n; j += size)
            if (cmp(&arr[imin], &arr[j]) > 0)
                imin = j;
                // swap imin and i one character at a time
                // swap arr[i..i+size-1] with arr[imin..imin+size-1] via Duff's device
        char t;
        int k = 0;
        int chunks = (size + 3) / 4;  // ceil(size / 4)

        switch (size % 4) {
            do {
            case 0: t = arr[i+k]; arr[i+k] = arr[imin+k]; arr[imin+k] = t; k++;
            case 3: t = arr[i+k]; arr[i+k] = arr[imin+k]; arr[imin+k] = t; k++;
            case 2: t = arr[i+k]; arr[i+k] = arr[imin+k]; arr[imin+k] = t; k++;
            case 1: t = arr[i+k]; arr[i+k] = arr[imin+k]; arr[imin+k] = t; k++;
            } while (--chunks > 0);
        }
    }
}

Point3d points[10];
sort(points, sizeof(Point3d), 10, point_x_cmp);