#ifndef MATCH_ORDER_H
#define MATCH_ORDER_H

#include <vector>
#include <string>
#include "handlecsv.h" // Include the CSVRow struct and related functions

// Matches orders between buy and sell tables based on the given indicator.
// Indicator = 1 for Sell Priority, 2 for Buy Priority
void matchOrders(std::vector<CSVRow>& buyTable, std::vector<CSVRow>& sellTable, int indicator);

#endif // MATCH_ORDER_H
