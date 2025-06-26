/**
 * @file order.h
 * @brief Order management system for financial trading platform
 * 
 * This header defines the core order structure and associated types used
 * throughout the Kabu trading system. Orders represent buy/sell requests
 * from traders and contain all necessary information for trade matching.
 */

#ifndef KABU_ORDER_H
#define KABU_ORDER_H

#include <stdint.h>
#include <time.h>
#include <stdbool.h>

/**
 * @brief Unique identifier for orders in the system
 * 
 * Each order is assigned a unique ID when created. IDs are sequential
 * and used to track orders throughout their lifecycle.
 */
typedef uint32_t OrderId;

/**
 * @brief User identifier for account management
 */
typedef uint32_t UserId;

/**
 * @brief Order side - whether buying or selling
 */
typedef enum {
    ORDER_SIDE_BUY,   /**< Buy order (bid) */
    ORDER_SIDE_SELL   /**< Sell order (ask) */
} OrderSide;

/**
 * @brief Current status of an order in the system
 * 
 * Orders progress through various states from creation to completion.
 * Understanding these states is crucial for order management.
 */
typedef enum {
    STATUS_PENDING,   /**< Order created but not yet in book */
    STATUS_OPEN,      /**< Order active in order book */
    STATUS_CANCELLED, /**< Order cancelled by user or system */
    STATUS_COMPLETE   /**< Order fully filled */
} OrderStatus;

/**
 * @brief Order flags for execution behavior
 * 
 * Orders are classified based on whether they add or remove liquidity
 * from the order book. Market orders only remove liquidity, while
 * post-only limit orders only add liquidity. Regular limit orders may
 * both add and remove liquidity depending on the execution price.
 */
typedef enum {
    LIQUIDITY_TAKER     = 1 << 0,   /**< Order may remove liquidity from book */
    IMMEDIATE_OR_CANCEL = 1 << 1,   /**< Order is filled immediately or canceled */
    ALL_OR_NONE         = 1 << 2,   /**< Order is filled completely or canceled */
} OrderFlags;

/**
 * @brief Core order structure containing all order information
 * 
 * This structure represents a trading order with all necessary fields
 * for processing, matching, and tracking throughout the system.
 */
typedef struct {
    OrderId id;                /**< Unique order identifier */
    UserId user_id;            /**< ID of user who placed order */
    char ticker[32];           /**< Trading pair symbol (e.g., "BTC/USD") */
    char execution_flags;      /**< Order execution behavior flags */
    int32_t total_quantity;    /**< Number of shares/units to trade */
    int32_t unfilled_quantity; /**< Amount remaining to be filled */
    uint32_t price;            /**< Price (limit orders) */
    uint32_t trigger_price;    /**< Trigger price for e.g. stop orders */
    OrderStatus status;        /**< Current order status */
    time_t created_time;       /**< When order was created */
    time_t updated_time;       /**< Last modification time */
} Order;

/**
 * @brief Create a new order with basic validation
 * @param user_id User placing the order
 * @param ticker Trading pair symbol
 * @param side Buy or sell
 * @param type Order type (market, limit, stop)
 * @param quantity Number of shares to trade
 * @param price Price in cents (ignored for market orders)
 * @return Pointer to newly created order, or NULL on error
 */
Order* order_create(UserId user_id, const char* ticker, OrderSide side, 
                   OrderType type, int quantity, int price);

/**
 * @brief Free memory allocated for an order
 * @param order Pointer to order to free
 */
void order_destroy(Order* order);

/**
 * @brief Cancel an order if it's still active
 * @param order Order to cancel
 * @return true if successfully cancelled, false otherwise
 */
bool order_cancel(Order* order);

/**
 * @brief Partially fill an order
 * @param order Order to fill
 * @param fill_quantity Amount to fill
 * @param fill_price Price of the fill
 * @return true if fill was successful, false otherwise
 */
bool order_fill(Order* order, int fill_quantity, int fill_price);

/**
 * @brief Check if order is completely filled
 * @param order Order to check
 * @return true if order is fully filled
 */
bool order_is_filled(const Order* order);

/**
 * @brief Get remaining quantity to be filled
 * @param order Order to check
 * @return Remaining quantity
 */
int order_remaining_quantity(const Order* order);

/**
 * @brief Print order details for debugging
 * @param order Order to print
 */
void order_print(const Order* order);

#endif /* KABU_ORDER_H */