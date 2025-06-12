/***************************************
* book.h                               *
* Order book header                    *
***************************************/

#include "order.h"

// Liquidity entry within a price level
typedef struct {
    OrderId order_id;
    int quantity;
} Liquidity;

// Price level in the order book
typedef struct {
    int price;
    int total_quantity;
    Liquidity* liquidity;
    int count;
    int capacity;
} PriceLevel;

// Single-sided order book
typedef struct {
    PriceLevel* levels;
    int count;
    int capacity;
} OrderBook;