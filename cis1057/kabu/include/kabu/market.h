/**
 * @file market.h
 * @brief Market management and order matching system
 * 
 * This header defines the market structure that combines bid and ask order
 * books for a trading pair. The market handles order matching, trade execution,
 * and maintains market statistics like last price and volume.
 */

#ifndef KABU_MARKET_H
#define KABU_MARKET_H

#include "book.h"
#include <stdbool.h>

/**
 * @brief Market statistics for tracking trading activity
 */
typedef struct {
    int last_price;          /**< Price of most recent trade */
    int daily_volume;        /**< Total shares traded today */
    int daily_high;          /**< Highest price today */
    int daily_low;           /**< Lowest price today */
    int opening_price;       /**< First trade price today */
    time_t last_trade_time;  /**< Timestamp of last trade */
} MarketStats;

/**
 * @brief Market structure representing a complete trading pair
 * 
 * A market combines bid (buy) and ask (sell) order books for a specific
 * trading pair. It handles order matching between the two sides and
 * maintains market statistics and state.
 */
typedef struct {
    char ticker[32];         /**< Trading pair symbol (e.g., "BTC/USD") */
    char base_symbol[8];     /**< Base currency symbol (e.g., "BTC") */
    char quote_symbol[8];    /**< Quote currency symbol (e.g., "USD") */
    OrderBook* bids;         /**< Buy order book (sorted high to low) */
    OrderBook* asks;         /**< Sell order book (sorted low to high) */
    MarketStats stats;       /**< Current market statistics */
    bool is_active;          /**< Whether market is open for trading */
    time_t created_time;     /**< When market was created */
} Market;

/**
 * @brief Result of processing an order in the market
 */
typedef struct {
    bool success;            /**< Whether order was processed successfully */
    int filled_quantity;     /**< Amount of order that was filled */
    int average_price;       /**< Average execution price */
    Trade* trades;           /**< Array of resulting trades */
    int trade_count;         /**< Number of trades generated */
    char error_message[256]; /**< Error description if success is false */
} OrderResult;

/**
 * @brief Create a new market for a trading pair
 * @param ticker Trading pair symbol (e.g., "BTC/USD")
 * @param base_symbol Base currency symbol
 * @param quote_symbol Quote currency symbol
 * @return Pointer to newly created market, or NULL on error
 */
Market* market_create(const char* ticker, const char* base_symbol, const char* quote_symbol);

/**
 * @brief Free memory allocated for a market
 * @param market Market to destroy
 */
void market_destroy(Market* market);

/**
 * @brief Process an order in the market (matching and execution)
 * @param market Market to process order in
 * @param order Order to process
 * @return OrderResult containing execution details
 */
OrderResult market_process_order(Market* market, Order* order);

/**
 * @brief Cancel an order from the market
 * @param market Market containing the order
 * @param order_id ID of order to cancel
 * @return true if order was found and cancelled
 */
bool market_cancel_order(Market* market, OrderId order_id);

/**
 * @brief Get current bid-ask spread
 * @param market Market to check
 * @return Spread in cents, or -1 if no spread available
 */
int market_get_spread(const Market* market);

/**
 * @brief Get current best bid price
 * @param market Market to check
 * @return Best bid price, or -1 if no bids
 */
int market_get_best_bid(const Market* market);

/**
 * @brief Get current best ask price
 * @param market Market to check
 * @return Best ask price, or -1 if no asks
 */
int market_get_best_ask(const Market* market);

/**
 * @brief Get market depth (number of price levels on each side)
 * @param market Market to check
 * @param bid_depth Pointer to store bid side depth
 * @param ask_depth Pointer to store ask side depth
 */
void market_get_depth(const Market* market, int* bid_depth, int* ask_depth);

/**
 * @brief Check if market is currently active for trading
 * @param market Market to check
 * @return true if market is active
 */
bool market_is_active(const Market* market);

/**
 * @brief Activate or deactivate a market
 * @param market Market to modify
 * @param active New active state
 */
void market_set_active(Market* market, bool active);

/**
 * @brief Get current market statistics
 * @param market Market to check
 * @return Copy of current market statistics
 */
MarketStats market_get_stats(const Market* market);

/**
 * @brief Reset daily statistics (called at market open)
 * @param market Market to reset
 */
void market_reset_daily_stats(Market* market);

/**
 * @brief Print market summary for debugging
 * @param market Market to print
 * @param show_depth Whether to show order book depth
 */
void market_print(const Market* market, bool show_depth);

/**
 * @brief Free memory allocated for an OrderResult
 * @param result OrderResult to free
 */
void order_result_destroy(OrderResult* result);

#endif /* KABU_MARKET_H */