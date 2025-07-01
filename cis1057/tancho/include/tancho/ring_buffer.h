/***************************************
* ring_buffer.h                        *
* Ring buffers for data storage        *
***************************************/

typedef struct {
    size_t head;
    size_t tail;
    size_t capacity;
    uint32_t data[];
} RingBuffer_t;