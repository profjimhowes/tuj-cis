#include <sys/time.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

#define TIMER(code) do { \
    double start = get_time(); \
    { code } \
    double elapsed = get_time() - start; \
    printf("Elapsed time: %9.6lf seconds\n", elapsed); \
} while(0)
