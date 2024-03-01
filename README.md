# Car-Rental-System
This project implements a comprehensive car rental system using C++ and adhering to Object-Oriented Programming (OOP) principles. It operates through a command-line interface, offering a range of functionalities tailored for customers, employees, and managers.

## Rental Limits Based on Record:
- "Good" record: Can rent up to 3 cars.
- "Average" record: Can rent up to 2 cars.
- "Poor" record: Can rent only 1 car.

## Car Conditions:
- Possible conditions are "good", "average", and "poor".

## Total Due Calculation:
- The 'total_due' is the sum of the net fine imposed and the rent of the car.

## Database Files:
- The car database file is named "cars.csv".
  - Format: Car ID, Model, Condition, Rent Price, Rent Person Role (-1 if not rented, 0 for employee, 1 for customer), Rent Person ID (-1 if not rented), Due Date
- The customer database file is named "customers.csv".
  - Format: Customer ID, Name, Password, Record (good, average, poor), Total Due, Number of Cars Rented, Car ID 1, Car ID 2, Car ID 3
- The employee database file is named "employees.csv".
  - Format: Employee ID, Name, Password, Record (good, average, poor), Total Due, Number of Cars Rented, Car ID 1, Car ID 2, Car ID 3
- The manager database file is named "managers.csv".
  - Format: Manager ID, Name, Password

## Program Exit:
- Please ensure to exit the program properly before running it again to prevent potential crashes.

## Contact:
- For any queries, please contact me at: 
  - Email: harshag22@iitk.ac.in
