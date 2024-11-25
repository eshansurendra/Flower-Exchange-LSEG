#include "match_order.h"
#include <algorithm>

void matchOrders(std::vector<CSVRow>& buyTable, std::vector<CSVRow>& sellTable, int indicator) {
    for (auto buyRow = buyTable.begin(); buyRow != buyTable.end();) {
        if (buyRow->newColumn != "New" && buyRow->newColumn != "PFill") {
            ++buyRow;
            continue;
        }

        for (auto sellRow = sellTable.begin(); sellRow != sellTable.end();) {
            if (sellRow->newColumn != "New" && sellRow->newColumn != "PFill") {
                ++sellRow;
                continue;
            }

            // Match logic based on indicator
            if (buyRow->column2 == sellRow->column2) { // Same instrument
                if (std::stoi(sellRow->column5) <= std::stoi(buyRow->column5)) { // Price matches
                    if (indicator == 2) { // Buy Priority
                        if (std::stoi(sellRow->column4) > std::stoi(buyRow->column4)) {
                            // Fill buy order, partial fill sell order
                            buyRow->newColumn = "Fill";
                            CSVRow newSellRow = *sellRow;
                            newSellRow.column4 = buyRow->column4;
                            newSellRow.newColumn = "PFill";

                            sellRow->column4 = std::to_string(std::stoi(sellRow->column4) - std::stoi(buyRow->column4));
                            sellRow->newColumn = "PFill";

                            writeLineToOutputCSV(newSellRow);
                            writeLineToOutputCSV(*buyRow);
                            ++sellRow;
                        } else if (std::stoi(sellRow->column4) == std::stoi(buyRow->column4)) {
                            // Fill both buy and sell orders
                            buyRow->newColumn = "Fill";
                            sellRow->newColumn = "Fill";

                            sellRow->column5 = buyRow->column5;
                            writeLineToOutputCSV(*sellRow);
                            writeLineToOutputCSV(*buyRow);
                            ++sellRow;
                        } else {
                            // Partial fill buy order, fill sell order
                            sellRow->newColumn = "Fill";

                            CSVRow newBuyRow = *buyRow;
                            newBuyRow.column4 = sellRow->column4;
                            newBuyRow.newColumn = "PFill";

                            buyRow->column4 = std::to_string(std::stoi(buyRow->column4) - std::stoi(sellRow->column4));
                            buyRow->newColumn = "PFill";

                            writeLineToOutputCSV(*sellRow);
                            writeLineToOutputCSV(newBuyRow);
                            ++sellRow;
                        }
                    } else if (indicator == 1) { // Sell Priority
                        if (std::stoi(buyRow->column4) > std::stoi(sellRow->column4)) {
                            // Fill sell order, partial fill buy order
                            sellRow->newColumn = "Fill";

                            CSVRow newBuyRow = *buyRow;
                            newBuyRow.column4 = sellRow->column4;
                            newBuyRow.newColumn = "PFill";

                            buyRow->column4 = std::to_string(std::stoi(buyRow->column4) - std::stoi(sellRow->column4));
                            buyRow->newColumn = "PFill";

                            writeLineToOutputCSV(newBuyRow);
                            writeLineToOutputCSV(*sellRow);
                            ++sellRow;
                        } else if (std::stoi(buyRow->column4) == std::stoi(sellRow->column4)) {
                            // Fill both buy and sell orders
                            sellRow->newColumn = "Fill";
                            buyRow->newColumn = "Fill";

                            buyRow->column5 = sellRow->column5;
                            writeLineToOutputCSV(*buyRow);
                            writeLineToOutputCSV(*sellRow);
                            ++sellRow;
                        } else {
                            // Partial fill sell order, fill buy order
                            buyRow->newColumn = "Fill";

                            CSVRow newSellRow = *sellRow;
                            newSellRow.column4 = buyRow->column4;
                            newSellRow.newColumn = "PFill";

                            sellRow->column4 = std::to_string(std::stoi(sellRow->column4) - std::stoi(buyRow->column4));
                            sellRow->newColumn = "PFill";

                            writeLineToOutputCSV(*buyRow);
                            writeLineToOutputCSV(newSellRow);
                            ++sellRow;
                        }
                    }
                } else {
                    break; // Prices don't match, move to next row
                }
            } else {
                ++sellRow;
            }
        }

        // Move to the next buy row if still "New" or "PFill"
        if (buyRow->newColumn == "New" || buyRow->newColumn == "PFill") {
            ++buyRow;
        }
    }

    // Remove filled orders from buy and sell tables
    buyTable.erase(std::remove_if(buyTable.begin(), buyTable.end(),
                                  [](const CSVRow& row) { return row.newColumn == "Fill"; }),
                   buyTable.end());
    sellTable.erase(std::remove_if(sellTable.begin(), sellTable.end(),
                                   [](const CSVRow& row) { return row.newColumn == "Fill"; }),
                    sellTable.end());
}
