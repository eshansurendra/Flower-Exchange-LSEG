#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>    
#include <set> 
#include <chrono>
#include "handlecsv.h"
#include "validateOrder.h"

using namespace std;

int main() {
    string inputFileName = "example_4.csv"; 
    vector<CSVRow> csvRows = readCSV(inputFileName);

    vector<CSVRow> buyOrders; 
    vector<CSVRow> sellOrders; 

    // Create and open the .csv file
    std::ofstream outputFile("execution_rep.csv");

    // Check if the file is open
    if (!outputFile.is_open()) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    for (const CSVRow& currentRow : csvRows) {
    
        bool isOrderValid = validateOrder(const_cast<CSVRow&>(currentRow));

        // Check the validation result
        if (!isOrderValid) {
            // If the row is invalid, it has already been marked as "Reject" in the newColumn
            // Write the rejected row to output.csv
            writeLineToOutputCSV(currentRow);
            continue;
        }

        // Add the row to buy or sell orders
        addToTable(buyOrders, sellOrders, currentRow);

        // Check if either buyOrders or sellOrders is empty
        if (buyOrders.empty() || sellOrders.empty()) {
            // If this condition is satisfied, push a copy of this line to output.csv file and read the next line in .csv file
            writeLineToOutputCSV(currentRow);
            continue;
        }

        else {
            
            // Sort buy orders in descending order based on integer conversion of column5
            sort(buyOrders.begin(), buyOrders.end(), [](const CSVRow& a, const CSVRow& b) {
                return stoi(a.column5) > stoi(b.column5); // Descending order
                });
           

            // Sort sell orders in ascending order based on integer conversion of column5
            sort(sellOrders.begin(), sellOrders.end(), [](const CSVRow& a, const CSVRow& b) {
                return stoi(a.column5) < stoi(b.column5); // Ascending order
                });
            

            // Check whether the last read row is a sell item (if currentRow.column3 == "2") or a buy item (if currentRow.column3 == "1").
            const CSVRow& lastReadRow = currentRow; // Get the last-read row


            if (lastReadRow.column3 == "2") {
                for (auto sellOrder = sellOrders.begin(); sellOrder != sellOrders.end() && ((sellOrder->newColumn == "New") || (sellOrder->newColumn == "PFill")); ++sellOrder) {
                    for (CSVRow& buyOrder : buyOrders) {
                        // Compare the sell price and buy price after converting to integers
                        if (stoi(sellOrder->column5) <= stoi(buyOrder.column5)) {
                            // Check the amount of the orders after converting to integers
                            if (stoi(sellOrder->column4) > stoi(buyOrder.column4)) {
                                buyOrder.newColumn = "Fill";

                                // Create another row in the sell orders by changing newColumn = "PFill", column5 = buyOrder.column5, and column4 = buyOrder.column4
                                CSVRow partialSellOrder;
                                partialSellOrder.ord = sellOrder->ord; // Set a new order ID
                                partialSellOrder.column1 = sellOrder->column1;
                                partialSellOrder.column2 = sellOrder->column2;
                                partialSellOrder.column3 = sellOrder->column3;
                                partialSellOrder.newColumn = "PFill";
                                partialSellOrder.column4 = buyOrder.column4;
                                partialSellOrder.column5 = buyOrder.column5;

                                // Update the existing sell row
                                sellOrder->column4 = to_string(stoi(sellOrder->column4) - stoi(buyOrder.column4));
                                sellOrder->newColumn = "PFill";

                                // Write partialSellOrder to output.csv
                                writeLineToOutputCSV(partialSellOrder);

                                // Write buyOrder to output.csv
                                writeLineToOutputCSV(buyOrder);

                                if (stoi(sellOrder->column4) == 0) {
                                    break;
                                }
                            }

                            else if (stoi(sellOrder->column4) == stoi(buyOrder.column4)) {
                                buyOrder.newColumn = "Fill";
                                sellOrder->newColumn = "Fill";

                                sellOrder->column5 = buyOrder.column5;

                                // Write sellOrder to output.csv
                                writeLineToOutputCSV(*sellOrder);

                                // Write buyOrder to output.csv
                                writeLineToOutputCSV(buyOrder);

                                break;
                            }
                            // If sell order is less than buy order
                            else {
                                sellOrder->newColumn = "Fill";

                                // Create another row in the buy orders by changing newColumn = "PFill" and column4 = sellOrder->column4
                                CSVRow partialBuyOrder;
                                partialBuyOrder.ord = buyOrder.ord; // Set a new order ID
                                partialBuyOrder.column1 = buyOrder.column1;
                                partialBuyOrder.column2 = buyOrder.column2;
                                partialBuyOrder.column3 = buyOrder.column3;
                                partialBuyOrder.newColumn = "Pfill";
                                partialBuyOrder.column4 = sellOrder->column4;
                                partialBuyOrder.column5 = buyOrder.column5;

                                // Update the existing buy row
                                buyOrder.column4 = to_string(stoi(buyOrder.column4) - stoi(sellOrder->column4));
                                buyOrder.newColumn = "PFill";

                                // Write partialBuyOrder to output.csv
                                writeLineToOutputCSV(partialBuyOrder);

                                // Write sellOrder to output.csv
                                writeLineToOutputCSV(*sellOrder);

                            }
                        }
                        else {
                            if (sellOrder->newColumn == "New") {
                                writeLineToOutputCSV(*sellOrder);
                                break;
                            }
                            else {
                                break;
                            }
                        }
                    }

                    // Remove rows with "Fill" in the newColumn from both buy and sell orders.
                    buyOrders.erase(
                        std::remove_if(buyOrders.begin(), buyOrders.end(),
                            [](const CSVRow& row) { return row.newColumn == "Fill"; }),
                        buyOrders.end());

                    sellOrders.erase(
                        std::remove_if(sellOrders.begin(), sellOrders.end(),
                            [](const CSVRow& row) { return row.newColumn == "Fill"; }),
                        sellOrders.end());

                }
               
            }

            else if (lastReadRow.column3 == "1") {
                for (auto buyOrder = buyOrders.begin(); buyOrder != buyOrders.end() && ((buyOrder->newColumn == "New") || (buyOrder->newColumn == "PFill")); ++buyOrder) {
                    for (CSVRow& sellOrder : sellOrders) {
                        // Compare the sell price and buy price after converting to integers
                        if (stoi(sellOrder.column5) <= stoi(buyOrder->column5)) {
                            // Check the amount of the orders after converting to integers
                            if (stoi(buyOrder->column4) > stoi(sellOrder.column4)) {
                                sellOrder.newColumn = "Fill";

                                // Create another row in the buy orders by changing newColumn = "PFill", column5 = sellOrder.column5, and column4 = sellOrder.column4
                                CSVRow partialBuyOrder;
                                partialBuyOrder.ord = buyOrder->ord; // Set a new order ID
                                partialBuyOrder.column1 = buyOrder->column1;
                                partialBuyOrder.column2 = buyOrder->column2;
                                partialBuyOrder.column3 = buyOrder->column3;
                                partialBuyOrder.newColumn = "PFill";
                                partialBuyOrder.column4 = sellOrder.column4;
                                partialBuyOrder.column5 = sellOrder.column5;

                                // Update the existing buy row
                                buyOrder->column4 = to_string(stoi(buyOrder->column4) - stoi(sellOrder.column4));
                                buyOrder->newColumn = "PFill";

                                // Write partialBuyOrder to output.csv
                                writeLineToOutputCSV(partialBuyOrder);

                                // Write sellOrder to output.csv
                                writeLineToOutputCSV(sellOrder);

                            }

                            else if (stoi(buyOrder->column4) == stoi(sellOrder.column4)) {
                                sellOrder.newColumn = "Fill";
                                buyOrder->newColumn = "Fill";

                                buyOrder->column5 = sellOrder.column5;

                                // Write buyOrder to output.csv
                                writeLineToOutputCSV(*buyOrder);

                                // Write sellOrder to output.csv
                                writeLineToOutputCSV(sellOrder);

                                break;
                            }
                            // If buy order is less than sell order
                            else {
                                buyOrder->newColumn = "Fill";

                                // Create another row in the sell orders by changing newColumn = "PFill" and column4 = buyOrder->column4
                                CSVRow partialSellOrder;
                                partialSellOrder.ord = sellOrder.ord; // Set a new order ID
                                partialSellOrder.column1 = sellOrder.column1;
                                partialSellOrder.column2 = sellOrder.column2;
                                partialSellOrder.column3 = sellOrder.column3;
                                partialSellOrder.newColumn = "Pfill";
                                partialSellOrder.column4 = buyOrder->column4;
                                partialSellOrder.column5 = sellOrder.column5;

                                // Update the existing sell row
                                sellOrder.column4 = to_string(stoi(sellOrder.column4) - stoi(buyOrder->column4));
                                sellOrder.newColumn = "PFill";

                                // Write partialSellOrder to output.csv
                                writeLineToOutputCSV(partialSellOrder);

                                // Write buyOrder to output.csv
                                writeLineToOutputCSV(*buyOrder);
                            }
                        }
                        else {
                            if (buyOrder->newColumn == "New") {
                                writeLineToOutputCSV(*buyOrder);
                                break;
                            }

                            else {
                                break;
                            }
                        }
                    }

                    // Remove rows with "Fill" in the newColumn from both buy and sell orders.
                    buyOrders.erase(
                        std::remove_if(buyOrders.begin(), buyOrders.end(),
                            [](const CSVRow& row) { return row.newColumn == "Fill"; }),
                        buyOrders.end());

                    sellOrders.erase(
                        std::remove_if(sellOrders.begin(), sellOrders.end(),
                            [](const CSVRow& row) { return row.newColumn == "Fill"; }),
                        sellOrders.end());
                }
            }
        }

    }
    
    // Close the file
    outputFile.close();

    cout << "Buy Orders:" << endl;
    for (const CSVRow& row : buyOrders) {
        cout << row.ord << ", " << row.column1 << ", " << row.column2 << ", " << row.column3 << ", "
            << row.newColumn << ", " << row.column4 << ", " << row.column5 << endl;
    }

    cout << "Sell Orders:" << endl;
    for (const CSVRow& row : sellOrders) {
        cout << row.ord << ", " << row.column1 << ", " << row.column2 << ", " << row.column3 << ", "
            << row.newColumn << ", " << row.column4 << ", " << row.column5 << endl;
    }
    
    return 0;
}
