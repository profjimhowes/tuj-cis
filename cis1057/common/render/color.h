#include <stdlib.h>

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b;
}

uint32_t random_color() {
    return rgb(rand() % 192 + 64, rand() % 192 + 64, rand() % 192 + 64);
}

uint32_t fade(uint32_t color, float delta) {
    uint8_t r = color >> 16;
    uint8_t g = color >> 8;
    uint8_t b = color;
    return rgb(delta * r, delta * g, delta * b);
}