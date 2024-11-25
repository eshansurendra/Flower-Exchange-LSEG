#ifndef HANDLECSV_H
#define HANDLECSV_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct CSVRow {
    std::string ord;
    std::string column1;
    std::string column2;
    std::string column3;
    std::string newColumn;
    std::string column4;
    std::string column5;
    std::string column6;
};

// Function to trim whitespace from a string
void trim(std::string& s);

// Function to read CSV file and return a vector of CSVRow objects
std::vector<CSVRow> readCSV(const std::string& filename);

// Function to add a row to either buyTable or sellTable based on column3 value
void addToTable(std::vector<CSVRow>& buyTable, std::vector<CSVRow>& sellTable, const CSVRow& row);

// Function to write a single row to an output CSV file
void writeLineToOutputCSV(const CSVRow& row);


#endif // HANDLECSV_H
