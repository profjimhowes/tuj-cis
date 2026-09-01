#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"

#define FPS_LIMIT 30

#define BLACK  0x00000000
#define WHITE  0x00ffffff
#define RED    0x00ff0000
#define YELLOW 0x00ffff00

static struct {
    int head, length;
    enum {UP, DOWN, LEFT, RIGHT} direction;
    int history[255], h_index;
} snake;

const int W = 80, H = 80, scale = 4;

void reset_snake(uint32_t *fb) {
    memset(fb, 0, W * H * scale * scale * sizeof(uint32_t));
    memset(snake.history, 0, 255 * sizeof(int));
    snake.head = W * (H - 1) / 2;
    snake.length = 8;
    snake.h_index = 0;
}

int scale_pixel(int p) {
    return (p / W * W * scale + p % W) * scale;
}

void paint_square(uint32_t *fb, int x, uint32_t color) {
    int p = scale_pixel(x);
    for (int i = 0; i < scale; i++)
        for (int j = 0; j < scale; j++)
            fb[p + i * W * scale + j] = color;
}

int main(void) {
    // Start up the application and get the frame buffer
    uint32_t *fb = app_open("Snake", W * scale, H * scale);
    reset_snake(fb);

    while (app_continue(FPS_LIMIT) && !key_up(KEY_ESC)) {
        // Step 1: Change direction based on keyboard input
        if (key_down(KEY_UP) && snake.direction != DOWN)
            snake.direction = UP;
        else if (key_down(KEY_DOWN) && snake.direction != UP)
            snake.direction = DOWN;
        else if (key_down(KEY_LEFT) && snake.direction != RIGHT)
            snake.direction = LEFT;
        else if (key_down(KEY_RIGHT) && snake.direction != LEFT)
            snake.direction = RIGHT;

        // Step 2: Move snake based on current direction
        switch (snake.direction) {
            case UP:    snake.head -= W; break;
            case DOWN:  snake.head += W; break;
            case LEFT:  snake.head--;    break;
            case RIGHT: snake.head++;
        }

        // Step 3: Kill (reset) snake if out of bounds or self-collision
        if (snake.head < 0 || snake.head >= W * H) reset_snake(fb);
        else if (snake.head % W == 0 && snake.direction == RIGHT) reset_snake(fb);
        else if (snake.head % W == W - 1 && snake.direction == LEFT) reset_snake(fb);
        else if (fb[scale_pixel(snake.head)] == WHITE) reset_snake(fb);

        // Step 4: Check if snake ate a fruit and extend it
        if (fb[scale_pixel(snake.head)] == RED) snake.length += 1;
        else if (fb[scale_pixel(snake.head)] == YELLOW) snake.length += 2;

        // Step 5: Write snake head and erase snake tail in frame buffer
        paint_square(fb, snake.head, WHITE);
        paint_square(fb, snake.history[(snake.h_index - snake.length + 255) % 255], BLACK);
        snake.history[snake.h_index++ % 255] = snake.head;

        // Step 6: Randomly generate a new fruit occasionally
        if (!(rand() % 100)) {
            int pixel = rand() % (W * H);
            if (!fb[scale_pixel(pixel)])
                paint_square(fb, pixel, rand() % 8 ? RED : YELLOW);
        }
    }

    app_close();
    return EXIT_SUCCESS;
}
