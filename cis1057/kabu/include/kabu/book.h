/**
 * @file book.h
 * @brief Order book data structures and management functions
 * 
 * This header defines the order book system used for matching buy and sell
 * orders. The order book maintains price levels with associated liquidity,
 * enabling efficient order matching and market depth visualization.
 */

#ifndef KABU_BOOK_H
#define KABU_BOOK_H

#include "order.h"
#include <stdbool.h>

/**
 * @brief Liquidity entry representing quantity available from a maker order
 * 
 * Each liquidity entry tracks the unfilled portion of a live maker order.
 */
typedef struct {
    OrderId order_id;    /**< ID of the order this liquidity belongs to */
    int quantity;        /**< Amount of liquidity remaining */
} Liquidity;

/**
 * @brief Price level containing all orders at a specific price
 * 
 * The order book is organized by price levels, where each level contains
 * all the liquidity (unfilled orders) available at that price. Liquidity
 * within a price level is ordered as a queue (first in, first out).
 */
typedef struct {
    int price;                    /**< Price in cents for this level */
    int total_quantity;           /**< Total quantity available at this price */
    int count;                    /**< Number of active entries */
    int capacity;                 /**< Maximum entries this level can hold */
    Liquidity liquidity[];        /**< Array of liquidity entries */
} PriceLevel;

/**
 * @brief Single-sided order book (either bids or asks)
 * 
 * Represents one side of the order book, containing multiple price levels
 * sorted by price. For bids (buy orders), levels are sorted descending
 * (highest price first). For asks (sell orders), levels are sorted
 * ascending (lowest price first).
 */
typedef struct {
    PriceLevel* levels;     /**< Array of price levels */
    int level_count;        /**< Number of active price levels */
    int level_capacity;     /**< Maximum price levels this book can hold */
    bool is_bid_side;       /**< True for bid book, false for ask book */
} OrderBook;

/**
 * @brief Create a new empty order book
 * @param is_bid_side True for bid book (buy orders), false for ask book (sell orders)
 * @return Pointer to newly created order book, or NULL on error
 */
OrderBook* book_create(bool is_bid_side);

/**
 * @brief Free memory allocated for an order book
 * @param book Order book to destroy
 */
void book_destroy(OrderBook* book);

/**
 * @brief Add an order to the order book
 * @param book Order book to add to
 * @param order Order to add
 * @return true if order was successfully added
 */
bool book_add_order(OrderBook* book, const Order* order);

/**
 * @brief Remove an order from the order book
 * @param book Order book to remove from
 * @param order_id ID of order to remove
 * @return true if order was found and removed
 */
bool book_remove_order(OrderBook* book, OrderId order_id);

/**
 * @brief Get the best price available in the book
 * @param book Order book to check
 * @return Best price, or -1 if book is empty
 */
int book_get_best_price(const OrderBook* book);

/**
 * @brief Get total quantity available at the best price
 * @param book Order book to check
 * @return Quantity at best price, or 0 if book is empty
 */
int book_get_best_quantity(const OrderBook* book);

/**
 * @brief Get total quantity available at a specific price
 * @param book Order book to check
 * @param price Price level to check
 * @return Total quantity at that price level
 */
int book_get_quantity_at_price(const OrderBook* book, int price);

/**
 * @brief Check if the order book is empty
 * @param book Order book to check
 * @return true if book contains no orders
 */
bool book_is_empty(const OrderBook* book);

/**
 * @brief Get the number of price levels in the book
 * @param book Order book to check
 * @return Number of active price levels
 */
int book_get_depth(const OrderBook* book);

/**
 * @brief Print order book contents for debugging
 * @param book Order book to print
 * @param max_levels Maximum number of levels to print
 */
void book_print(const OrderBook* book, int max_levels);

/**
 * @brief Clear all orders from the order book
 * @param book Order book to clear
 */
void book_clear(OrderBook* book);

#endif /* KABU_BOOK_H */