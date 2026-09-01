#include <stdlib.h>

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (uint32_t)r | ((uint32_t)g << 8) | ((uint32_t)b << 16);
}

uint32_t random_color() {
    return rgb(rand() % 256, rand() % 256, rand() % 256);
}