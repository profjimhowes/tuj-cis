/**
 * @file trade.h
 * @brief Trade data structures and management functions
 * 
 * This header defines the Trade data structure which represents an individual
 * trade execution between buyer and seller.
 */

#ifndef KABU_TRADE_H
#define KABU_TRADE_H

#include "order.h"
#include <stdbool.h>

/**
 * @brief Trade execution data
 */
typedef struct {
    OrderId maker_id;       /**< Order ID of the maker */
    OrderId taker_id;       /**< Order ID of the taker */
    int quantity;           /**< Amount traded, negative if taker is seller */
    int value;              /**< Value traded, negative is taker is buyer */
    time_t timestamp;       /**< When trade occurred */
} Trade;

#endif /* KABU_TRADE_H */