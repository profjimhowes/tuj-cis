/**
 * @file test_book.c
 * @brief Unit tests for order book system
 * 
 * This file contains comprehensive tests for the book.h interface.
 * Tests cover order book creation, order management, and price level operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/test.h"
#include "../include/kabu/book.h"

/** Global test counters (defined in test framework) */
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

/**
 * @brief Test order book creation
 */
void test_book_creation() {
    puts("\n--- Testing Order Book Creation ---");
    
    // Test bid book creation
    OrderBook* bid_book = book_create(true);
    ASSERT_NOT_NULL(bid_book, "Bid book creation should succeed");
    
    if (bid_book) {
        ASSERT_TRUE(bid_book->is_bid_side, "Bid book should have is_bid_side = true");
        ASSERT_EQ(0, bid_book->level_count, "New book should have 0 levels");
        ASSERT_TRUE(bid_book->level_capacity > 0, "Book should have positive capacity");
        ASSERT_TRUE(book_is_empty(bid_book), "New book should be empty");
        
        book_destroy(bid_book);
    }
    
    // Test ask book creation
    OrderBook* ask_book = book_create(false);
    ASSERT_NOT_NULL(ask_book, "Ask book creation should succeed");
    
    if (ask_book) {
        ASSERT_FALSE(ask_book->is_bid_side, "Ask book should have is_bid_side = false");
        ASSERT_TRUE(book_is_empty(ask_book), "New ask book should be empty");
        
        book_destroy(ask_book);
    }
}

/**
 * @brief Test adding orders to bid book
 */
void test_add_orders_to_bid_book() {
    puts("\n--- Testing Add Orders to Bid Book ---");
    
    OrderBook* book = book_create(true);
    ASSERT_NOT_NULL(book, "Book creation should succeed");
    
    if (book) {
        // Create test orders
        Order* order1 = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                                   ORDER_TYPE_LIMIT, 100, 50000);
        Order* order2 = order_create(1002, "BTC/USD", ORDER_SIDE_BUY, 
                                   ORDER_TYPE_LIMIT, 50, 49950);
        Order* order3 = order_create(1003, "BTC/USD", ORDER_SIDE_BUY, 
                                   ORDER_TYPE_LIMIT, 75, 50000);
        
        if (order1 && order2 && order3) {
            // Add orders to book
            bool added1 = book_add_order(book, order1);
            ASSERT_TRUE(added1, "First order should be added successfully");
            ASSERT_FALSE(book_is_empty(book), "Book should not be empty after adding order");
            ASSERT_EQ(1, book_get_depth(book), "Book should have 1 price level");
            
            bool added2 = book_add_order(book, order2);
            ASSERT_TRUE(added2, "Second order should be added successfully");
            ASSERT_EQ(2, book_get_depth(book), "Book should have 2 price levels");
            
            bool added3 = book_add_order(book, order3);
            ASSERT_TRUE(added3, "Third order should be added successfully");
            ASSERT_EQ(2, book_get_depth(book), "Book should still have 2 price levels");
            
            // Test best price (highest for bids)
            int best_price = book_get_best_price(book);
            ASSERT_EQ(50000, best_price, "Best bid price should be 50000");
            
            // Test quantity at best price
            int best_qty = book_get_best_quantity(book);
            ASSERT_EQ(175, best_qty, "Best price level should have 175 shares (100+75)");
            
            // Test quantity at specific price
            int qty_at_50000 = book_get_quantity_at_price(book, 50000);
            ASSERT_EQ(175, qty_at_50000, "Price level 50000 should have 175 shares");
            
            int qty_at_49950 = book_get_quantity_at_price(book, 49950);
            ASSERT_EQ(50, qty_at_49950, "Price level 49950 should have 50 shares");
            
            int qty_at_49900 = book_get_quantity_at_price(book, 49900);
            ASSERT_EQ(0, qty_at_49900, "Non-existent price level should have 0 shares");
            
            order_destroy(order1);
            order_destroy(order2);
            order_destroy(order3);
        }
        
        book_destroy(book);
    }
}

/**
 * @brief Test adding orders to ask book
 */
void test_add_orders_to_ask_book() {
    puts("\n--- Testing Add Orders to Ask Book ---");
    
    OrderBook* book = book_create(false);
    ASSERT_NOT_NULL(book, "Ask book creation should succeed");
    
    if (book) {
        // Create test orders (sell orders)
        Order* order1 = order_create(1001, "BTC/USD", ORDER_SIDE_SELL, 
                                   ORDER_TYPE_LIMIT, 100, 50100);
        Order* order2 = order_create(1002, "BTC/USD", ORDER_SIDE_SELL, 
                                   ORDER_TYPE_LIMIT, 50, 50200);
        Order* order3 = order_create(1003, "BTC/USD", ORDER_SIDE_SELL, 
                                   ORDER_TYPE_LIMIT, 75, 50100);
        
        if (order1 && order2 && order3) {
            book_add_order(book, order1);
            book_add_order(book, order2);
            book_add_order(book, order3);
            
            // Test best price (lowest for asks)
            int best_price = book_get_best_price(book);
            ASSERT_EQ(50100, best_price, "Best ask price should be 50100 (lowest)");
            
            // Test quantity at best price
            int best_qty = book_get_best_quantity(book);
            ASSERT_EQ(175, best_qty, "Best price level should have 175 shares");
            
            order_destroy(order1);
            order_destroy(order2);
            order_destroy(order3);
        }
        
        book_destroy(book);
    }
}

/**
 * @brief Test removing orders from book
 */
void test_remove_orders() {
    puts("\n--- Testing Remove Orders ---");
    
    OrderBook* book = book_create(true);
    
    if (book) {
        Order* order1 = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                                   ORDER_TYPE_LIMIT, 100, 50000);
        Order* order2 = order_create(1002, "BTC/USD", ORDER_SIDE_BUY, 
                                   ORDER_TYPE_LIMIT, 50, 49950);
        
        if (order1 && order2) {
            book_add_order(book, order1);
            book_add_order(book, order2);
            
            ASSERT_EQ(2, book_get_depth(book), "Book should have 2 levels before removal");
            
            // Remove first order
            bool removed1 = book_remove_order(book, order1->id);
            ASSERT_TRUE(removed1, "Order removal should succeed");
            ASSERT_EQ(1, book_get_depth(book), "Book should have 1 level after removal");
            
            // Try to remove same order again
            bool removed_again = book_remove_order(book, order1->id);
            ASSERT_FALSE(removed_again, "Removing non-existent order should fail");
            
            // Remove second order
            bool removed2 = book_remove_order(book, order2->id);
            ASSERT_TRUE(removed2, "Second order removal should succeed");
            ASSERT_TRUE(book_is_empty(book), "Book should be empty after removing all orders");
            
            order_destroy(order1);
            order_destroy(order2);
        }
        
        book_destroy(book);
    }
}

/**
 * @brief Test invalid order additions
 */
void test_invalid_order_additions() {
    puts("\n--- Testing Invalid Order Additions ---");
    
    OrderBook* bid_book = book_create(true);
    
    if (bid_book) {
        // Try to add NULL order
        bool added_null = book_add_order(bid_book, NULL);
        ASSERT_FALSE(added_null, "Adding NULL order should fail");
        
        // Try to add sell order to bid book
        Order* sell_order = order_create(1001, "BTC/USD", ORDER_SIDE_SELL, 
                                        ORDER_TYPE_LIMIT, 100, 50000);
        if (sell_order) {
            bool added_wrong_side = book_add_order(bid_book, sell_order);
            ASSERT_FALSE(added_wrong_side, "Adding sell order to bid book should fail");
            
            order_destroy(sell_order);
        }
        
        book_destroy(bid_book);
    }
    
    // Test adding buy order to ask book
    OrderBook* ask_book = book_create(false);
    
    if (ask_book) {
        Order* buy_order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                                       ORDER_TYPE_LIMIT, 100, 50000);
        if (buy_order) {
            bool added_wrong_side = book_add_order(ask_book, buy_order);
            ASSERT_FALSE(added_wrong_side, "Adding buy order to ask book should fail");
            
            order_destroy(buy_order);
        }
        
        book_destroy(ask_book);
    }
}

/**
 * @brief Test book clearing
 */
void test_book_clear() {
    puts("\n--- Testing Book Clear ---");
    
    OrderBook* book = book_create(true);
    
    if (book) {
        // Add several orders
        Order* orders[3];
        for (int i = 0; i < 3; i++) {
            orders[i] = order_create(1001 + i, "BTC/USD", ORDER_SIDE_BUY, 
                                   ORDER_TYPE_LIMIT, 100, 50000 - i * 50);
            if (orders[i]) {
                book_add_order(book, orders[i]);
            }
        }
        
        ASSERT_FALSE(book_is_empty(book), "Book should not be empty before clear");
        ASSERT_EQ(3, book_get_depth(book), "Book should have 3 levels before clear");
        
        book_clear(book);
        
        ASSERT_TRUE(book_is_empty(book), "Book should be empty after clear");
        ASSERT_EQ(0, book_get_depth(book), "Book should have 0 levels after clear");
        ASSERT_EQ(-1, book_get_best_price(book), "Best price should be -1 for empty book");
        ASSERT_EQ(0, book_get_best_quantity(book), "Best quantity should be 0 for empty book");
        
        // Clean up orders
        for (int i = 0; i < 3; i++) {
            if (orders[i]) {
                order_destroy(orders[i]);
            }
        }
        
        book_destroy(book);
    }
}

/**
 * @brief Test edge cases
 */
void test_edge_cases() {
    puts("\n--- Testing Edge Cases ---");
    
    // Test operations on NULL book
    bool added_to_null = book_add_order(NULL, NULL);
    ASSERT_FALSE(added_to_null, "Adding to NULL book should fail");
    
    bool removed_from_null = book_remove_order(NULL, 1);
    ASSERT_FALSE(removed_from_null, "Removing from NULL book should fail");
    
    int price_from_null = book_get_best_price(NULL);
    ASSERT_EQ(-1, price_from_null, "Best price from NULL book should be -1");
    
    bool empty_null = book_is_empty(NULL);
    ASSERT_TRUE(empty_null, "NULL book should be considered empty");
    
    // Test operations on empty book
    OrderBook* empty_book = book_create(true);
    if (empty_book) {
        ASSERT_EQ(-1, book_get_best_price(empty_book), "Empty book best price should be -1");
        ASSERT_EQ(0, book_get_best_quantity(empty_book), "Empty book best quantity should be 0");
        ASSERT_EQ(0, book_get_depth(empty_book), "Empty book depth should be 0");
        
        bool removed_from_empty = book_remove_order(empty_book, 12345);
        ASSERT_FALSE(removed_from_empty, "Removing from empty book should fail");
        
        book_destroy(empty_book);
    }
}

/**
 * @brief Main test function
 */
int main() {
    TEST_INIT();
    
    puts("\n" COLOR_GREEN "=== TESTING ORDER BOOK SYSTEM ===" COLOR_RESET);
    
    // Run all tests
    test_book_creation();
    test_add_orders_to_bid_book();
    test_add_orders_to_ask_book();
    test_remove_orders();
    test_invalid_order_additions();
    test_book_clear();
    test_edge_cases();
    
    TEST_SUMMARY();
    return TEST_EXIT();
}