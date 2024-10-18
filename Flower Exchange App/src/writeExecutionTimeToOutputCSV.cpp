#include "writeExecutionTimeToOutputCSV.h"

// Function to write the execution time to the CSV file
void writeExecutionTimeToOutputCSV(long long executionTime) {
    std::ofstream csvFile("execution_rep.csv", std::ios_base::app); // Open in append mode

    if (!csvFile.is_open()) {
        std::cerr << "Error opening execution_rep.csv file." << std::endl;
        return;
    }

    // Write the execution time to execution_rep.csv
    csvFile << "Execution Time (ms)," << executionTime << std::endl;

    // Close the CSV file
    csvFile.close();
}