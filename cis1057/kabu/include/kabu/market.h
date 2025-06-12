/***************************************
* market.h                             *
* Market header                        *
***************************************/

#include "book.h"

typedef struct {
    char ticker[32];
    char base_symbol[8];
    char quote_symbol[8];
    OrderBook* bids;
    OrderBook* asks;
} Market;