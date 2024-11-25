#include "validate_order.h"

bool validateOrder(CSVRow& row) {
    // Check if required fields are empty
    if (row.column1.empty() || row.column2.empty() || row.column3.empty() ||
        row.column4.empty() || row.column5.empty()) {
        row.newColumn = "Reject";
        row.column6 = "Does not contain a required field";
        return false;
    }

    // Trim column2 (instrument name)
    trim(row.column2);

    // Validate instrument name
    std::set<std::string> validInstruments = { "Rose", "Lavender", "Lotus", "Tulip", "Orchid" };
    if (validInstruments.find(row.column2) == validInstruments.end()) {
        row.newColumn = "Reject";
        row.column6 = "Invalid instrument";
        return false;
    }

    // Trim column3 (side)
    trim(row.column3);

    // Validate side (must be 1 or 2)
    if ((std::stoi(row.column3) > 2) || (std::stoi(row.column3) < 1)) {
        row.newColumn = "Reject";
        row.column6 = "Invalid side";
        return false;
    }

    // Validate price (column5, must be > 0)
    if (std::stoi(row.column5) <= 0) {
        row.newColumn = "Reject";
        row.column6 = "Invalid price (Price is less than or equal to 0)";
        return false;
    }

    // Validate quantity (column4, must be a multiple of 10)
    if (std::stoi(row.column4) % 10 != 0) {
        row.newColumn = "Reject";
        row.column6 = "Invalid quantity (Not a multiple of 10)";
        return false;
    }

    // Validate quantity range (10 <= quantity <= 1000)
    if (std::stoi(row.column4) < 10 || std::stoi(row.column4) > 1000) {
        row.newColumn = "Reject";
        row.column6 = "Invalid quantity (Quantity is less than 10 or greater than 1000)";
        return false;
    }

    return true;
}
