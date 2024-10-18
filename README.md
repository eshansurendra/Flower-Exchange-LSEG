# Flower-Exchange-LSEG
This project implements a simplified Flower Exchange system as part of the C++ Workshop Series at the University of Moratuwa, organized by the London Stock Exchange Group (LSEG). The Flower Exchange allows traders to buy and sell various types of flowers through a simulated trading platform.

## Key Features

- **Trader Application:** Enables users to submit buy and sell orders for different flower types.
- **Exchange Application:** Processes incoming orders against the existing order book, matching buyers and sellers. 
- **Order Matching:** Supports both full and partial order executions.
- **Execution Reports:** Provides detailed reports on the status of each order (e.g., filled, partially filled, rejected).
- **Order Rejection Handling:** Implements logic to reject orders based on factors like quantity limits and invalid flower types.

## Project Structure

```
└── Flower-Exchange-LSEG
    ├── Flower Exchange App
    │   ├── src
    │   ├── flow chart
    │   ├── Executable Program 
    │   │   └── output_program   
    │   └── Tested Examples
    ├── frontend
    └── backend

```

## Getting Started:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/eshansurendra/Flower-Exchange-LSEG.git
   ```
2. **Navigate to Executable:**
   ```bash
   cd 'Flower Exchange App/Executable Program'
   ```

3. **Run the Program:**
   ```bash
   ./output_program ../Tested Examples/example_2.csv
   ```

   **Explanation:**

   - `./output_program`: Executes the compiled C++ program.
   - `../Tested Examples/example_2.csv`: Provides the path to a CSV file containing sample order data. The program processes this file to simulate trading activity.
  
## Team Members

- [**Vidmal H.V.P**](https://github.com/pulinduvidmal)
- [**Surendra S.A.J.E**](https://github.com/eshansurendra)
 
