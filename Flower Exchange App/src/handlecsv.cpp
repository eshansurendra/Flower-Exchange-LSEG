#include "handlecsv.h"

void trim(std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start != std::string::npos && end != std::string::npos) {
        s = s.substr(start, end - start + 1);
    } else {
        s.clear();
    }
}

std::vector<CSVRow> readCSV(const std::string& filename) {
    std::vector<CSVRow> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        CSVRow row;

        row.ord = "ord" + std::to_string(data.size() + 1);

        std::getline(ss, row.column1, ',');
        std::getline(ss, row.column2, ',');
        std::getline(ss, row.column3, ',');
        row.newColumn = "New";
        std::getline(ss, row.column4, ',');
        std::getline(ss, row.column5, ',');

        trim(row.column3);

        data.push_back(row);
    }

    file.close();
    return data;
}

void addToTable(std::vector<CSVRow>& buyTable, std::vector<CSVRow>& sellTable, const CSVRow& row) {
    if (row.column3 == "1") {
        buyTable.push_back(row);
    } else if (row.column3 == "2") {
        sellTable.push_back(row);
    }
}

void writeLineToOutputCSV(const CSVRow& row) {
    std::ofstream csvFile("execution_rep.csv", std::ios_base::app);

    if (!csvFile.is_open()) {
        std::cerr << "Error opening execution_rep.csv file." << std::endl;
        return;
    }

    csvFile << row.ord << "," << row.column1 << "," << row.column2 << "," << row.column3 << ","
            << row.newColumn << "," << row.column4 << "," << row.column5 << "," << row.column6 << std::endl;

    csvFile.close();
}

