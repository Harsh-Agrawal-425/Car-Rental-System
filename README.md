## Running the Car Rental System

A step-by-step series of examples that tell you how to get the car rental system up and running:

1. Clone the repository to your local machine.
2. Open the project in VS Code or any IDE.
3. Compile and run the `220425_cpp_assignment.cpp` file.
4. Make sure that all the files(cpp source files and all CSV files are located in same folder).

## Built With

* [C++](http://www.cplusplus.com/)

## Author

* **Harsh Agrawal** 

## Car Rental System Readme

**Rental Limits Based on Record:**
- "Good" record: Can rent up to 3 cars.
- "Average" record: Can rent up to 2 cars.
- "Poor" record: Can rent only 1 car.

**Car Conditions:**
- Possible conditions are "good", "average", and "poor".

**Total Due Calculation:**
- The 'total_due' is the sum of the net fine imposed and the rent of the car.

**Database Files:**
- The car database file is named "cars.csv".
  - Format: Car ID, Model, Condition, Rent Price, Rent Person Role (-1 if not rented, 0 for employee, 1 for customer), Rent Person ID (-1 if not rented), Due Date
- The customer database file is named "customers.csv".
  - Format: Customer ID, Name, Password, Record (good, average, poor), Total Due, Number of Cars Rented, Car ID 1, Car ID 2, Car ID 3
- The employee database file is named "employees.csv".
  - Format: Employee ID, Name, Password, Record (good, average, poor), Total Due, Number of Cars Rented, Car ID 1, Car ID 2, Car ID 3
- The manager database file is named "managers.csv".
  - Format: Manager ID, Name, Password

**Program Exit:**
- Please ensure to exit the program properly before running it again to prevent potential crashes.

**Contact:**
- For any queries, please contact me at: 
  - Email: harshag22@iitk.ac.in
