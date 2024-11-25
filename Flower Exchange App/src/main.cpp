#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <set>
#include <chrono>
#include "handlecsv.h" // Include the CSVRow struct and trim function
#include "validate_order.h" // Include the validateOrder function
#include "match_order.h" // Include the matchOrders function

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <csv_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    
    vector<CSVRow> csvData = readCSV(filename);

    vector<CSVRow> buyTable;
    vector<CSVRow> sellTable;

    std::ofstream csvFile("execution_rep.csv");

    if (!csvFile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    //csvFile << "Order ID,Client Order,Instrument, Side, Exec Status, Quantity, Price" << std::endl;

    // Measure the start time
    auto start = std::chrono::high_resolution_clock::now();

    for (const CSVRow& row : csvData) {
        bool isValid = validateOrder(const_cast<CSVRow&>(row));

        if (!isValid) {
            writeLineToOutputCSV(row);
            continue;
        }

        addToTable(buyTable, sellTable, row);

        if (buyTable.empty() || sellTable.empty()) {
            writeLineToOutputCSV(row);
            continue;
        }
        else {

            // Sort buy table in Descending order based on integer conversion of column5
            sort(buyTable.begin(), buyTable.end(), [](const CSVRow& a, const CSVRow& b) {
                return stoi(a.column5) > stoi(b.column5); // Descending order
                });


            // Sort sell table in Ascending order based on integer conversion of column5
            sort(sellTable.begin(), sellTable.end(), [](const CSVRow& a, const CSVRow& b) {
                return stoi(a.column5) < stoi(b.column5); // Ascending order
                });

            const CSVRow& lastRow = row;
            int indicator = stoi(lastRow.column3);
            //cout << indicator << "\n";


            matchOrders(buyTable, sellTable, indicator);
        }
    }

    // Measure the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the execution time in milliseconds
    auto executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Write the execution time to the output CSV
    // writeExecutionTimeToOutputCSV(executionTime);


    csvFile.close();

    return 0;
}