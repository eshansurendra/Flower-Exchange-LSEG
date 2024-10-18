#include "validateOrder.h"
#include <iostream>
#include <cstdlib> // For std::stoi

// Trim whitespace from a string
extern void trim(std::string& s);  // Declare it as external since it's already defined in another module

// Function to validate an order based on the specified rules
bool validateOrder(CSVRow& row) {

    // Check if any required field is empty
    if (row.column1.empty() || row.column2.empty() || row.column3.empty() ||
        row.column4.empty() || row.column5.empty()) {
        row.newColumn = "Reject";
        return false;
    }

    // Trim whitespace from column2
    trim(row.column2);

    // Define a set of valid instruments
    std::set<std::string> validInstruments = {"Rose", "Lavender", "Lotus", "Tulip", "Orchid"};

    // Check if column2 is not in the set of valid instruments
    if (validInstruments.find(row.column2) == validInstruments.end()) {
        row.newColumn = "Reject";
        return false;
    }

    // Trim whitespace from column3
    trim(row.column3);

    // Check for invalid Side (must be "1" or "2")
    try {
        int side = std::stoi(row.column3);
        if (side > 2 || side < 1) {
            row.newColumn = "Reject";
            return false;
        }
    } catch (std::exception& e) {
        row.newColumn = "Reject";
        return false;
    }

    // Check if Price is not greater than 0
    try {
        if (std::stoi(row.column5) <= 0) {
            row.newColumn = "Reject";
            return false;
        }
    } catch (std::exception& e) {
        row.newColumn = "Reject";
        return false;
    }

    // Check if Quantity is not a multiple of 10
    try {
        int quantity = std::stoi(row.column4);
        if (quantity % 10 != 0 || quantity < 10 || quantity > 1000) {
            row.newColumn = "Reject";
            return false;
        }
    } catch (std::exception& e) {
        row.newColumn = "Reject";
        return false;
    }

    // If all validations pass, return true
    return true;
}
