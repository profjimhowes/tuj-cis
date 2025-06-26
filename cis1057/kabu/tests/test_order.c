/**
 * @file test_order.c
 * @brief Unit tests for order management system
 * 
 * This file contains comprehensive tests for the order.h interface.
 * Tests cover order creation, manipulation, and state management.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/test.h"
#include "../include/kabu/order.h"

/** Global test counters (defined in test framework) */
int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

/**
 * @brief Test order creation with valid parameters
 */
void test_order_creation_valid() {
    puts("\n--- Testing Order Creation (Valid) ---");
    
    Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                               ORDER_TYPE_LIMIT, 100, 50000);
    
    ASSERT_NOT_NULL(order, "Order creation should succeed with valid parameters");
    
    if (order) {
        ASSERT_EQ(1001, order->user_id, "User ID should be set correctly");
        ASSERT_STR_EQ("BTC/USD", order->ticker, "Ticker should be set correctly");
        ASSERT_EQ(ORDER_SIDE_BUY, order->side, "Order side should be BUY");
        ASSERT_EQ(ORDER_TYPE_LIMIT, order->type, "Order type should be LIMIT");
        ASSERT_EQ(100, order->quantity, "Quantity should be set correctly");
        ASSERT_EQ(50000, order->price, "Price should be set correctly");
        ASSERT_EQ(0, order->filled_quantity, "Filled quantity should start at 0");
        ASSERT_EQ(STATUS_PENDING, order->status, "New order should have PENDING status");
        ASSERT_TRUE(order->created_time > 0, "Created time should be set");
        
        order_destroy(order);
    }
}

/**
 * @brief Test order creation with invalid parameters
 */
void test_order_creation_invalid() {
    puts("\n--- Testing Order Creation (Invalid) ---");
    
    // Test with invalid user ID
    Order* order1 = order_create(0, "BTC/USD", ORDER_SIDE_BUY, 
                                ORDER_TYPE_LIMIT, 100, 50000);
    ASSERT_NULL(order1, "Order creation should fail with invalid user ID");
    
    // Test with NULL ticker
    Order* order2 = order_create(1001, NULL, ORDER_SIDE_BUY, 
                                ORDER_TYPE_LIMIT, 100, 50000);
    ASSERT_NULL(order2, "Order creation should fail with NULL ticker");
    
    // Test with zero quantity
    Order* order3 = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                                ORDER_TYPE_LIMIT, 0, 50000);
    ASSERT_NULL(order3, "Order creation should fail with zero quantity");
    
    // Test with negative quantity
    Order* order4 = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                                ORDER_TYPE_LIMIT, -10, 50000);
    ASSERT_NULL(order4, "Order creation should fail with negative quantity");
}

/**
 * @brief Test market order creation (price should be ignored)
 */
void test_market_order_creation() {
    puts("\n--- Testing Market Order Creation ---");
    
    Order* order = order_create(1001, "ETH/USD", ORDER_SIDE_SELL, 
                               ORDER_TYPE_MARKET, 50, 99999);
    
    ASSERT_NOT_NULL(order, "Market order creation should succeed");
    
    if (order) {
        ASSERT_EQ(ORDER_TYPE_MARKET, order->type, "Order type should be MARKET");
        ASSERT_EQ(ORDER_SIDE_SELL, order->side, "Order side should be SELL");
        // Price is ignored for market orders, but stored for reference
        
        order_destroy(order);
    }
}

/**
 * @brief Test order cancellation
 */
void test_order_cancellation() {
    puts("\n--- Testing Order Cancellation ---");
    
    Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                               ORDER_TYPE_LIMIT, 100, 50000);
    
    ASSERT_NOT_NULL(order, "Order should be created successfully");
    
    if (order) {
        // Set order to open status (normally done by order book)
        order->status = STATUS_OPEN;
        
        bool cancelled = order_cancel(order);
        ASSERT_TRUE(cancelled, "Order cancellation should succeed for open order");
        ASSERT_EQ(STATUS_CANCELLED, order->status, "Order status should be CANCELLED");
        
        // Try to cancel again
        bool cancelled_again = order_cancel(order);
        ASSERT_FALSE(cancelled_again, "Already cancelled order should not be cancellable");
        
        order_destroy(order);
    }
}

/**
 * @brief Test order cancellation on completed order
 */
void test_cancel_completed_order() {
    puts("\n--- Testing Cancel Completed Order ---");
    
    Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                               ORDER_TYPE_LIMIT, 100, 50000);
    
    if (order) {
        // Set order to completed status
        order->status = STATUS_COMPLETE;
        
        bool cancelled = order_cancel(order);
        ASSERT_FALSE(cancelled, "Completed order should not be cancellable");
        ASSERT_EQ(STATUS_COMPLETE, order->status, "Order status should remain COMPLETE");
        
        order_destroy(order);
    }
}

/**
 * @brief Test partial order filling
 */
void test_partial_order_fill() {
    puts("\n--- Testing Partial Order Fill ---");
    
    Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                               ORDER_TYPE_LIMIT, 100, 50000);
    
    if (order) {
        order->status = STATUS_OPEN;
        
        // Fill 30 shares at $500.00
        bool filled = order_fill(order, 30, 50000);
        ASSERT_TRUE(filled, "Partial fill should succeed");
        ASSERT_EQ(30, order->filled_quantity, "Filled quantity should be 30");
        ASSERT_EQ(STATUS_OPEN, order->status, "Order should remain OPEN after partial fill");
        ASSERT_FALSE(order_is_filled(order), "Order should not be completely filled");
        ASSERT_EQ(70, order_remaining_quantity(order), "Remaining quantity should be 70");
        
        order_destroy(order);
    }
}

/**
 * @brief Test complete order filling
 */
void test_complete_order_fill() {
    puts("\n--- Testing Complete Order Fill ---");
    
    Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                               ORDER_TYPE_LIMIT, 100, 50000);
    
    if (order) {
        order->status = STATUS_OPEN;
        
        // Fill remaining 100 shares
        bool filled = order_fill(order, 100, 50000);
        ASSERT_TRUE(filled, "Complete fill should succeed");
        ASSERT_EQ(100, order->filled_quantity, "Filled quantity should be 100");
        ASSERT_EQ(STATUS_COMPLETE, order->status, "Order should be COMPLETE after full fill");
        ASSERT_TRUE(order_is_filled(order), "Order should be completely filled");
        ASSERT_EQ(0, order_remaining_quantity(order), "Remaining quantity should be 0");
        
        order_destroy(order);
    }
}

/**
 * @brief Test overfilling an order (should fail)
 */
void test_order_overfill() {
    puts("\n--- Testing Order Overfill ---");
    
    Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                               ORDER_TYPE_LIMIT, 100, 50000);
    
    if (order) {
        order->status = STATUS_OPEN;
        
        // Try to fill more than the order quantity
        bool filled = order_fill(order, 150, 50000);
        ASSERT_FALSE(filled, "Overfill should fail");
        ASSERT_EQ(0, order->filled_quantity, "Filled quantity should remain 0");
        ASSERT_EQ(STATUS_OPEN, order->status, "Order status should remain OPEN");
        
        order_destroy(order);
    }
}

/**
 * @brief Test multiple partial fills
 */
void test_multiple_partial_fills() {
    puts("\n--- Testing Multiple Partial Fills ---");
    
    Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                               ORDER_TYPE_LIMIT, 100, 50000);
    
    if (order) {
        order->status = STATUS_OPEN;
        
        // First fill: 25 shares
        bool fill1 = order_fill(order, 25, 50000);
        ASSERT_TRUE(fill1, "First partial fill should succeed");
        ASSERT_EQ(25, order->filled_quantity, "Filled quantity should be 25");
        
        // Second fill: 30 shares
        bool fill2 = order_fill(order, 30, 50000);
        ASSERT_TRUE(fill2, "Second partial fill should succeed");
        ASSERT_EQ(55, order->filled_quantity, "Filled quantity should be 55");
        
        // Third fill: 45 shares (complete the order)
        bool fill3 = order_fill(order, 45, 50000);
        ASSERT_TRUE(fill3, "Third partial fill should succeed");
        ASSERT_EQ(100, order->filled_quantity, "Filled quantity should be 100");
        ASSERT_EQ(STATUS_COMPLETE, order->status, "Order should be COMPLETE");
        
        order_destroy(order);
    }
}

/**
 * @brief Test order utility functions
 */
void test_order_utilities() {
    puts("\n--- Testing Order Utility Functions ---");
    
    Order* order = order_create(1001, "BTC/USD", ORDER_SIDE_BUY, 
                               ORDER_TYPE_LIMIT, 100, 50000);
    
    if (order) {
        // Test remaining quantity calculation
        ASSERT_EQ(100, order_remaining_quantity(order), "Full order should have 100 remaining");
        
        order->filled_quantity = 25;
        ASSERT_EQ(75, order_remaining_quantity(order), "Partially filled order should have 75 remaining");
        
        order->filled_quantity = 100;
        ASSERT_EQ(0, order_remaining_quantity(order), "Completely filled order should have 0 remaining");
        
        // Test is_filled function
        ASSERT_TRUE(order_is_filled(order), "Order with filled_quantity == quantity should be filled");
        
        order->filled_quantity = 99;
        ASSERT_FALSE(order_is_filled(order), "Order with filled_quantity < quantity should not be filled");
        
        order_destroy(order);
    }
}

/**
 * @brief Main test function
 */
int main() {
    TEST_INIT();
    
    puts("\n" COLOR_GREEN "=== TESTING ORDER MANAGEMENT SYSTEM ===" COLOR_RESET);
    
    // Run all tests
    test_order_creation_valid();
    test_order_creation_invalid();
    test_market_order_creation();
    test_order_cancellation();
    test_cancel_completed_order();
    test_partial_order_fill();
    test_complete_order_fill();
    test_order_overfill();
    test_multiple_partial_fills();
    test_order_utilities();
    
    TEST_SUMMARY();
    return TEST_EXIT();
}