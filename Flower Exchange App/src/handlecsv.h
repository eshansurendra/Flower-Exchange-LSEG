#ifndef HANDLECSV_H
#define HANDLECSV_H

#include <string>
#include <vector>

struct CSVRow {
    std::string ord; // New column
    std::string column1;
    std::string column2;
    std::string column3;
    std::string newColumn; // New column between column3 and column4
    std::string column4;
    std::string column5;
};

// Function declarations
void trim(std::string& s);
std::vector<CSVRow> readCSV(const std::string& filename);
void writeLineToOutputCSV(const CSVRow& row);
void addToTable(std::vector<CSVRow>& buyTable, std::vector<CSVRow>& sellTable, const CSVRow& row);

#endif
