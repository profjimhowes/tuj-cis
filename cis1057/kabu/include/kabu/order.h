/***************************************
* order.h                              *
* Order header                         *
***************************************/

typedef uint32_t OrderId;

typedef enum {
    PENDING,
    OPEN,
    CANCELLED,
    COMPLETE
} OrderStatus;

typedef enum {
    MAKER,
    MAKER_TAKER,
    TAKER
} OrderLiquidity;

typedef struct {
    OrderId id;
    char ticker[32];
    int quantity;
    int price;
    OrderStatus status;
    OrderLiquidity liquidity;
} Order;