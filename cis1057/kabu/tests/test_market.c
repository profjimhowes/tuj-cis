/**
 * @file test_market.c
 * @brief Unit tests for market system
 * 
 * This file contains comprehensive tests for the market.h interface.
 * Tests cover market creation, order processing, trade matching, and statistics.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/test.h"
#include "../include/kabu/market.h"

/** Global test counters (defined in test framework) */
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

/**
 * @brief Test market creation
 */
void test_market_creation() {
    puts("\n--- Testing Market Creation ---");
    
    Market* market = market_create("BTC/USD", "BTC", "USD");
    ASSERT_NOT_NULL(market, "Market creation should succeed");
    
    if (market) {
        ASSERT_STR_EQ("BTC/USD", market->ticker, "Ticker should be set correctly");
        ASSERT_STR_EQ("BTC", market->base_symbol, "Base symbol should be set correctly");
        ASSERT_STR_EQ("USD", market->quote_symbol, "Quote symbol should be set correctly");
        ASSERT_NOT_NULL(market->bids, "Bid book should be created");
        ASSERT_NOT_NULL(market->asks, "Ask book should be created");
        ASSERT_TRUE(market->is_active, "New market should be active by default");
        ASSERT_TRUE(market->created_time > 0, "Created time should be set");
        
        // Test initial statistics
        MarketStats stats = market_get_stats(market);
        ASSERT_EQ(0, stats.last_price, "Initial last price should be 0");
        ASSERT_EQ(0, stats.daily_volume, "Initial daily volume should be 0");
        ASSERT_EQ(0, stats.daily_high, "Initial daily high should be 0");
        ASSERT_EQ(0, stats.daily_low, "Initial daily low should be 0");
        
        market_destroy(market);
    }
}

/**
 * @brief Test invalid market creation
 */
void test_invalid_market_creation() {
    puts("\n--- Testing Invalid Market Creation ---");
    
    Market* market1 = market_create(NULL, "BTC", "USD");
    ASSERT_NULL(market1, "Market creation with NULL ticker should fail");
    
    Market* market2 = market_create("BTC/USD", NULL, "USD");
    ASSERT_NULL(market2, "Market creation with NULL base symbol should fail");
    
    Market* market3 = market_create("BTC/USD", "BTC", NULL);
    ASSERT_NULL(market3, "Market creation with NULL quote symbol should fail");
    
    Market* market4 = market_create("", "BTC", "USD");
    ASSERT_NULL(market4, "Market creation with empty ticker should fail");
}

/**
 * @brief Test market activation/deactivation
 */
void test_market_activation() {
    puts("\n--- Testing Market Activation ---");
    
    Market* market = market_create("ETH/USD", "ETH", "USD");
    
    if (market) {
        ASSERT_TRUE(market_is_active(market), "New market should be active");
        
        market_set_active(market, false);
        ASSERT_FALSE(market_is_active(market), "Market should be inactive after deactivation");
        
        market_set_active(market, true);
        ASSERT_TRUE(market_is_active(market), "Market should be active after reactivation");
        
        market_destroy(market);
    }
}

/**
 * @brief Test adding orders to empty market
 */
void test_add_orders_to_empty_market() {
    puts("\n--- Testing Add Orders to Empty Market ---");
    
    Market* market = market_create("BTC/USD", "BTC", "USD");
    
    if (market) {
        // Initially no best bid/ask
        ASSERT_EQ(-1, market_get_best_bid(market), "Empty market should have no best bid");
        ASSERT_EQ(-1, market_get_best_ask(market), "Empty market should have no best ask");
        ASSERT_EQ(-1, market_get_spread(market), "Empty market should have no spread");
        
        // Add a buy order
        Order* buy_order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                                       ORDER_TYPE_LIMIT, 100, 50000);
        if (buy_order) {
            OrderResult result = market_process_order(market, buy_order);
            ASSERT_TRUE(result.success, "Processing buy order should succeed");
            ASSERT_EQ(0, result.filled_quantity, "Buy order should not be filled in empty market");
            ASSERT_EQ(0, result.trade_count, "No trades should occur with single buy order");
            
            ASSERT_EQ(50000, market_get_best_bid(market), "Best bid should be 50000");
            ASSERT_EQ(-1, market_get_best_ask(market), "Best ask should still be empty");
            
            order_destroy(buy_order);
            order_result_destroy(&result);
        }
        
        // Add a sell order
        Order* sell_order = order_create(1002, "BTC/USD", ORDER_SIDE_SELL, 
                                        ORDER_TYPE_LIMIT, 50, 50100);
        if (sell_order) {
            OrderResult result = market_process_order(market, sell_order);
            ASSERT_TRUE(result.success, "Processing sell order should succeed");
            ASSERT_EQ(0, result.filled_quantity, "Sell order should not be filled (no price match)");
            
            ASSERT_EQ(50000, market_get_best_bid(market), "Best bid should remain 50000");
            ASSERT_EQ(50100, market_get_best_ask(market), "Best ask should be 50100");
            ASSERT_EQ(100, market_get_spread(market), "Spread should be 100 cents");
            
            order_destroy(sell_order);
            order_result_destroy(&result);
        }
        
        market_destroy(market);
    }
}

/**
 * @brief Test order matching and trade execution
 */
void test_order_matching() {
    puts("\n--- Testing Order Matching ---");
    
    Market* market = market_create("BTC/USD", "BTC", "USD");
    
    if (market) {
        // Add a sell order first
        Order* sell_order = order_create(1001, "BTC/USD", ORDER_SIDE_SELL, 
                                        ORDER_TYPE_LIMIT, 100, 50000);
        if (sell_order) {
            market_process_order(market, sell_order);
            
            // Add a matching buy order
            Order* buy_order = order_create(1002, "BTC/USD", ORDER_SIDE_BUY, 
                                           ORDER_TYPE_LIMIT, 50, 50000);
            if (buy_order) {
                OrderResult result = market_process_order(market, buy_order);
                
                ASSERT_TRUE(result.success, "Order matching should succeed");
                ASSERT_EQ(50, result.filled_quantity, "Buy order should be fully filled");
                ASSERT_TRUE(result.trade_count > 0, "At least one trade should occur");
                ASSERT_EQ(50000, result.average_price, "Average price should match order price");
                
                // Check market statistics
                MarketStats stats = market_get_stats(market);
                ASSERT_EQ(50000, stats.last_price, "Last price should be updated");
                ASSERT_EQ(50, stats.daily_volume, "Daily volume should be updated");
                
                order_destroy(buy_order);
                order_result_destroy(&result);
            }
            
            order_destroy(sell_order);
        }
        
        market_destroy(market);
    }
}

/**
 * @brief Test market order execution
 */
void test_market_order_execution() {
    puts("\n--- Testing Market Order Execution ---");
    
    Market* market = market_create("ETH/USD", "ETH", "USD");
    
    if (market) {
        // Add some limit orders to provide liquidity
        Order* sell1 = order_create(1001, "ETH/USD", ORDER_SIDE_SELL, 
                                   ORDER_TYPE_LIMIT, 50, 3000);
        Order* sell2 = order_create(1002, "ETH/USD", ORDER_SIDE_SELL, 
                                   ORDER_TYPE_LIMIT, 30, 3010);
        
        if (sell1 && sell2) {
            market_process_order(market, sell1);
            market_process_order(market, sell2);
            
            // Execute market buy order
            Order* market_buy = order_create(1003, "ETH/USD", ORDER_SIDE_BUY, 
                                           ORDER_TYPE_MARKET, 60, 0);
            if (market_buy) {
                OrderResult result = market_process_order(market, market_buy);
                
                ASSERT_TRUE(result.success, "Market order should execute successfully");
                ASSERT_EQ(60, result.filled_quantity, "Market order should be fully filled");
                ASSERT_TRUE(result.trade_count >= 1, "Market order should generate trades");
                
                // Should execute at best available prices
                ASSERT_TRUE(result.average_price >= 3000, "Average price should be at least 3000");
                
                order_destroy(market_buy);
                order_result_destroy(&result);
            }
            
            order_destroy(sell1);
            order_destroy(sell2);
        }
        
        market_destroy(market);
    }
}

/**
 * @brief Test order cancellation in market
 */
void test_market_order_cancellation() {
    puts("\n--- Testing Order Cancellation in Market ---");
    
    Market* market = market_create("BTC/USD", "BTC", "USD");
    
    if (market) {
        Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                                   ORDER_TYPE_LIMIT, 100, 50000);
        if (order) {
            // Add order to market
            OrderResult result = market_process_order(market, order);
            ASSERT_TRUE(result.success, "Adding order to market should succeed");
            
            // Verify order is in the book
            ASSERT_EQ(50000, market_get_best_bid(market), "Order should be in bid book");
            
            // Cancel the order
            bool cancelled = market_cancel_order(market, order->id);
            ASSERT_TRUE(cancelled, "Order cancellation should succeed");
            
            // Verify order is removed from book
            ASSERT_EQ(-1, market_get_best_bid(market), "Order should be removed from bid book");
            
            // Try to cancel again
            bool cancelled_again = market_cancel_order(market, order->id);
            ASSERT_FALSE(cancelled_again, "Cancelling non-existent order should fail");
            
            order_destroy(order);
            order_result_destroy(&result);
        }
        
        market_destroy(market);
    }
}

/**
 * @brief Test market depth calculation
 */
void test_market_depth() {
    puts("\n--- Testing Market Depth ---");
    
    Market* market = market_create("BTC/USD", "BTC", "USD");
    
    if (market) {
        int bid_depth, ask_depth;
        
        // Initially no depth
        market_get_depth(market, &bid_depth, &ask_depth);
        ASSERT_EQ(0, bid_depth, "Initial bid depth should be 0");
        ASSERT_EQ(0, ask_depth, "Initial ask depth should be 0");
        
        // Add some orders at different price levels
        Order* orders[4];
        orders[0] = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, ORDER_TYPE_LIMIT, 100, 50000);
        orders[1] = order_create(1002, "BTC/USD", ORDER_SIDE_BUY, ORDER_TYPE_LIMIT, 50, 49950);
        orders[2] = order_create(1003, "BTC/USD", ORDER_SIDE_SELL, ORDER_TYPE_LIMIT, 75, 50100);
        orders[3] = order_create(1004, "BTC/USD", ORDER_SIDE_SELL, ORDER_TYPE_LIMIT, 25, 50200);
        
        for (int i = 0; i < 4; i++) {
            if (orders[i]) {
                market_process_order(market, orders[i]);
            }
        }
        
        market_get_depth(market, &bid_depth, &ask_depth);
        ASSERT_EQ(2, bid_depth, "Bid depth should be 2 after adding 2 bid levels");
        ASSERT_EQ(2, ask_depth, "Ask depth should be 2 after adding 2 ask levels");
        
        for (int i = 0; i < 4; i++) {
            if (orders[i]) {
                order_destroy(orders[i]);
            }
        }
        
        market_destroy(market);
    }
}

/**
 * @brief Test invalid operations
 */
void test_invalid_operations() {
    puts("\n--- Testing Invalid Operations ---");
    
    // Test operations on NULL market
    OrderResult null_result = market_process_order(NULL, NULL);
    ASSERT_FALSE(null_result.success, "Processing order on NULL market should fail");
    
    bool cancelled_null = market_cancel_order(NULL, 12345);
    ASSERT_FALSE(cancelled_null, "Cancelling order in NULL market should fail");
    
    ASSERT_EQ(-1, market_get_best_bid(NULL), "Best bid from NULL market should be -1");
    ASSERT_EQ(-1, market_get_best_ask(NULL), "Best ask from NULL market should be -1");
    ASSERT_EQ(-1, market_get_spread(NULL), "Spread from NULL market should be -1");
    
    // Test processing NULL order
    Market* market = market_create("BTC/USD", "BTC", "USD");
    if (market) {
        OrderResult result = market_process_order(market, NULL);
        ASSERT_FALSE(result.success, "Processing NULL order should fail");
        
        market_destroy(market);
    }
}

/**
 * @brief Test daily statistics reset
 */
void test_daily_stats_reset() {
    puts("\n--- Testing Daily Statistics Reset ---");
    
    Market* market = market_create("BTC/USD", "BTC", "USD");
    
    if (market) {
        // Simulate some trading activity
        Order* sell_order = order_create(1001, "BTC/USD", ORDER_SIDE_SELL, 
                                        ORDER_TYPE_LIMIT, 100, 50000);
        Order* buy_order = order_create(1002, "BTC/USD", ORDER_SIDE_BUY, 
                                       ORDER_TYPE_LIMIT, 100, 50000);
        
        if (sell_order && buy_order) {
            market_process_order(market, sell_order);
            OrderResult result = market_process_order(market, buy_order);
            
            // Verify stats are updated
            MarketStats stats = market_get_stats(market);
            ASSERT_EQ(50000, stats.last_price, "Last price should be set");
            ASSERT_EQ(100, stats.daily_volume, "Daily volume should be set");
            
            // Reset daily stats
            market_reset_daily_stats(market);
            
            MarketStats reset_stats = market_get_stats(market);
            ASSERT_EQ(0, reset_stats.daily_volume, "Daily volume should be reset to 0");
            ASSERT_EQ(0, reset_stats.daily_high, "Daily high should be reset to 0");
            ASSERT_EQ(0, reset_stats.daily_low, "Daily low should be reset to 0");
            // Note: last_price typically persists across daily resets
            
            order_destroy(sell_order);
            order_destroy(buy_order);
            order_result_destroy(&result);
        }
        
        market_destroy(market);
    }
}

/**
 * @brief Main test function
 */
int main() {
    TEST_INIT();
    
    puts("\n" COLOR_GREEN "=== TESTING MARKET SYSTEM ===" COLOR_RESET);
    
    // Run all tests
    test_market_creation();
    test_invalid_market_creation();
    test_market_activation();
    test_add_orders_to_empty_market();
    test_order_matching();
    test_market_order_execution();
    test_market_order_cancellation();
    test_market_depth();
    test_invalid_operations();
    test_daily_stats_reset();
    
    TEST_SUMMARY();
    return TEST_EXIT();
}