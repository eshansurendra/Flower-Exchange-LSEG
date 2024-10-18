#include "handlecsv.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Trim whitespace from the beginning and end of a string
void trim(string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start != string::npos && end != string::npos) {
        s = s.substr(start, end - start + 1);
    }
    else {
        s.clear();
    }
}

// Function to read a CSV file and return the data as a vector of CSVRow objects
vector<CSVRow> readCSV(const string& filename) {
    vector<CSVRow> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        CSVRow row;

        row.ord = "ord" + to_string(data.size() + 1); // Set the new column

        getline(ss, row.column1, ',');
        getline(ss, row.column2, ',');
        getline(ss, row.column3, ',');
        row.newColumn = "New"; // Set the new column value
        getline(ss, row.column4, ',');
        getline(ss, row.column5, ',');

        // Trim whitespace from column3 value
        trim(row.column3);

        data.push_back(row);
    }

    file.close();
    return data;
}

// Function to write a row to the output.csv file
void writeLineToOutputCSV(const CSVRow& row) {
    ofstream csvFile("execution_rep.csv", ios_base::app); // Open in append mode

    if (!csvFile.is_open()) {
        cerr << "Error opening output.csv file." << endl;
        return;
    } 

    // Write the line to output.csv
    csvFile << row.ord << "," << row.column1 << "," << row.column2 << "," << row.column3 << ","
            << row.newColumn << "," << row.column4 << "," << row.column5 << endl;

    csvFile.close();
}

// Function to add a row to the appropriate table based on column3 value
void addToTable(vector<CSVRow>& buyTable, vector<CSVRow>& sellTable, const CSVRow& row) {
    if (row.column3 == "1") {
        buyTable.push_back(row);
    }
    else if (row.column3 == "2") {
        sellTable.push_back(row);
    }
}
