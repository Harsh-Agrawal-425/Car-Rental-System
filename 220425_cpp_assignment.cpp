//==============================================================================================================================
/*
README:

Name: Harsh Agarwal
Roll No: 220425
Course: CS253
Assignment 1

1. Rental Limits Based on Record:
   - "Good" record: Can rent up to 3 cars.
   - "Average" record: Can rent up to 2 cars.
   - "Poor" record: Can rent only 1 car.

2. Car Conditions:
   - Possible conditions are "good", "average", and "poor".

3. Total Due Calculation:
   - The 'total_due' is the sum of the net fine imposed and the rent of the car.

4. Car Database:
   - The car database file is named "cars.csv".
   - Format: Car ID, Model, Condition, Rent Price, Rent Person Role (-1 if not rented, 0 for employee, 1 for customer), Rent Person ID (-1 if not rented), Due Date

5. Customer Database:
    - The customer database file is named "customers.csv".
    - Format: Customer ID, Name, Password, Record (good, average, poor), Total Due, Number of Cars Rented, Car ID 1, Car ID 2, Car ID 3

6. Employee Database:
    - The employee database file is named "employees.csv".
    - Format: Employee ID, Name, Password, Record (good, average, poor), Total Due, Number of Cars Rented, Car ID 1, Car ID 2, Car ID 3

7. Manager Database:
    - The manager database file is named "managers.csv".
    - Format: Manager ID, Name, Password

8. Program Exit:
   - Please ensure to exit the program properly before running it again to prevent potential crashes.

9. Contact:
    - For any queries, please contact me at: 
      - Email:harshag22@iitk.ac.in

*/
//================================================================================================================================


//HEADER FILES
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

//===============================================================================================================================
//GLOBAL VARIABLES AND FUNCTIONS
char role;                                                       //role of the user i.e. employee or customer or manager
int Bad_condition_fine;                                          //fine imposed if car is in bad condition
int late_return_fine_per_day;                                    //fine imposed per day if car is returned late

int calculateDaysLate(string today_date,string due_date){        //Function to determine the number of days overdue for car
    int today_day=stoi(today_date.substr(0,2));
    int today_month=stoi(today_date.substr(2,2));
    int today_year=stoi(today_date.substr(4,4));
    int due_day=stoi(due_date.substr(0,2));
    int due_month=stoi(due_date.substr(2,2));
    int due_year=stoi(due_date.substr(4,4));
    int days_late=0;
    if(today_year==due_year){
        if(today_month==due_month){
            days_late=today_day-due_day;
        }
        else{
            days_late=(today_month-due_month)*30+(today_day-due_day);
        }
    }
    else{
        days_late=(today_year-due_year)*365+(today_month-due_month)*30+(today_day-due_day);
    }
    return days_late;
}

string addDaysToDate(string date, int daysToAdd) {
    stringstream ss(date);
    string dayStr, monthStr, yearStr;
    ss >> setw(2) >> dayStr;
    ss >> setw(2) >> monthStr;
    ss >> yearStr;
    
    int day = stoi(dayStr);
    int month = stoi(monthStr);
    int year = stoi(yearStr);
    
    int daysInMonth[] = {0, 31, 28 + (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    day += daysToAdd;
    while (day > daysInMonth[month]) {
        day -= daysInMonth[month];
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
    
    // Format the resulting date
    std::stringstream result_ss;
    result_ss <<setw(2) <<setfill('0') << day;
    result_ss <<setw(2) <<setfill('0') << month;
    result_ss << year;
    return result_ss.str();
}

int findMinimumMissingID(const string& filename) {               //Function to find minimum id that is not present in database
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return -1; 
    }

    vector<int> ids;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        ids.push_back(stoi(token));
    }
    file.close();

    sort(ids.begin(), ids.end()); 

    int minMissingID = 1;
    for (int id : ids) {
        if (id == minMissingID) {
            minMissingID++;
        } else if (id > minMissingID) {
            break;
        }
    }

    return minMissingID;
}

//===============================================================================================================================
class car;

class database{                                                    //Parent class for employee and customer database class
    protected:
    string database_address;

    public:
    string get_database_address(){
        return database_address;
    }

    void set_database_address(string location){
        database_address=location;
    }
};

class User {                                                       //parent class for employee and customer class
protected:
    int id;
    string name;
    string password;

public:
    void setid(int _id){
        id=_id;
    }
    int getId() const {
        return id;
    }

    const string& getName() const {
        return name;
    }

    void setName(const string& newName) {
        name = newName;
    }

     const string& getPassword() const {
        return password;
    }

    void setPassword(const string& newPassword) {
        password = newPassword;
    }
};

//===============================================================================================================================
//EMPLOYEE

class Employee : public User {
private:
    string employee_record; // Three values possible "Good", "Average" and "Poor"
    int total_due;               // Fine imposed+ car rent price
    int number_of_cars_rented;
    int cars_rented[3];

public:
    Employee(int _id){                //constructor 1
        id=_id;
    }

    Employee(int _id,string _name, string _pwd){       //constructor 2
        id=_id;
        name=_name;
        password=_pwd;
        total_due=0;
        number_of_cars_rented=0;
        employee_record="average";
        cars_rented[0]=-1;
        cars_rented[1]=-1;
        cars_rented[2]=-1;
    }
    void clear_due(){                               //function to clear due
        if(total_due==0){
            cout<<"You have no due"<<endl;
            return;
        }
        cout<<"Your current due is: Rs."<<total_due<<endl;
        cout<<"Please Enter the amount you want to pay: ";
        int amount_paid;
        cin>>amount_paid;
        if(amount_paid>=total_due){
            total_due=0;
            cout<<"Thank You! for the payment. Your due has been cleared"<<endl;
        }
        else{
            total_due-=amount_paid;
            cout<<"Thank You! for the payment. Your current total_due is: Rs."<<total_due<<endl;
        }
    }

    void display_rented_cars();  //Defined later

    void rentcar();   //Defined later

    void returncar(); //defined later

    string getEmployeeRecord() const{
        return employee_record;
    }

    void setEmployeeRecord(const string& record) {
        employee_record = record;
    }

    int gettotal_due() const{
        return total_due;
    }

    void settotal_due(int _total_due) {
        total_due = _total_due;
    }

    int getNumberofRentedCars() const{
        return number_of_cars_rented;
    }

    void setNumberofRentedCars(int num){
        number_of_cars_rented=num;
    }

    int getIDofCarRented1() const{
        return cars_rented[0];
    }

    void setIDofCarRented1(int _idOfCar){
        cars_rented[0]=_idOfCar;
    }

    int getIDofCarRented2() const{
        return cars_rented[1];
    }

    void setIDofCarRented2(int _idOfCar){
        cars_rented[1]=_idOfCar;
    }

    int getIDofCarRented3() const{
        return cars_rented[2];
    }

    void setIDofCarRented3(int _idOfCar){
        cars_rented[2]=_idOfCar;
    }
};

class Employee_database: public database {
public:
    void readFromCSV(Employee * _employee) {
        ifstream file(database_address);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            int readId = stoi(token);
            if (readId == _employee->getId()) {
                string tempName;
                getline(ss, tempName, ',');
                _employee->setName(tempName);

                getline(ss, token, ',');
                _employee->setPassword(token);

                getline(ss, token, ',');
                _employee->setEmployeeRecord(token);

                getline(ss, token, ',');
                _employee->settotal_due(stoi(token));

                getline(ss, token, ',');
                _employee->setNumberofRentedCars(stoi(token));

                // Read car IDs
                for (int i = 0; i < _employee->getNumberofRentedCars(); ++i) {
                    getline(ss, token, ',');
                    if (i == 0) _employee->setIDofCarRented1(stoi(token));
                    else if (i == 1) _employee->setIDofCarRented2(stoi(token));
                    else if (i == 2) _employee->setIDofCarRented3(stoi(token));
                }
                break;
            }
        }
        file.close();
    }

    void writeToCSV(Employee* _employee) {
    ofstream file(database_address, ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }
    file << _employee->getId() << "," << _employee->getName() << "," << _employee->getPassword() << ","
         << _employee->getEmployeeRecord() << "," << _employee->gettotal_due() << "," << _employee->getNumberofRentedCars();

    for (int i = 0; i < 3; ++i) {
        if (i == 0) file << "," << _employee->getIDofCarRented1();
        else if (i == 1) file << "," << _employee->getIDofCarRented2();
        else if (i == 2) file << "," << _employee->getIDofCarRented3();
    }
    file << endl;
    file.close();
}


void updateInCSV(Employee* _employee) {
    ifstream file(database_address);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        int readId = stoi(token);
        if (readId == _employee->getId()) {
            line = to_string(_employee->getId()) + "," + _employee->getName() + "," + _employee->getPassword() + ","
                   + _employee->getEmployeeRecord() + "," + to_string(_employee->gettotal_due()) + ","
                   + to_string(_employee->getNumberofRentedCars());

            for (int i = 0; i < 3; ++i) {
                if (i == 0) line += "," + to_string(_employee->getIDofCarRented1());
                else if (i == 1) line += "," + to_string(_employee->getIDofCarRented2());
                else if (i == 2) line += "," + to_string(_employee->getIDofCarRented3());
            }
        }
        lines.push_back(line);
    }
    file.close();

    ofstream outFile(database_address);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }
    for (const auto& line : lines) {
        outFile << line << endl;
    }
    outFile.close();
}

void deleteFromCSV(int employeeID) {
        ifstream inFile(database_address);
        if (!inFile.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }

        vector<string> lines;
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ','); 
            int readID = stoi(token);
            if (readID != employeeID) {
                lines.push_back(line); 
            }
        }
        inFile.close();

        ofstream outFile(database_address);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }

        for (const auto& l : lines) {
            outFile << l << endl; 
        }
        outFile.close();

        cout << "Employee with ID " << employeeID << " deleted from " << database_address << endl;
    }

};

Employee_database EmployeeDB;                                        //Global class for employee database.

//===============================================================================================================================
//CUSTOMER

class Customer : public User {
private:
    string customer_record; // Three values possible "Good", "Average" and "Poor"
    int total_due;               // Fine imposed+car rent price
    int number_of_cars_rented;
    int cars_rented[3];


public:
    Customer(int _id){
        id=_id;
    }

    Customer(int _id,string _name, string _pwd){
        id=_id;
        name=_name;
        password=_pwd;
        total_due=0;
        number_of_cars_rented=0;
        customer_record="average";
        cars_rented[0]=-1;
        cars_rented[1]=-1;
        cars_rented[2]=-1;
    }

    void clear_due(){
        if(total_due==0){
            cout<<"You have no due"<<endl;
            return;
        }
        cout<<"Your current due is: Rs."<<total_due<<endl; 
        cout<<"Please Enter the amount you want to pay: ";
        int amount_paid;
        cin>>amount_paid;
        if(amount_paid>=total_due){
            total_due=0;
            cout<<"Thank You! for the payment. Your due has been cleared"<<endl;
        }
        else{
            total_due-=amount_paid;
            cout<<"Thank You! for the payment. Your current total_due is: Rs."<<total_due<<endl;
        }
    }

    void display_rented_cars();

    void rentcar();  //Defined later

    void returncar(); //Defined later

    string getCustomerRecord() const{
        return customer_record;
    }

    void setCustomerRecord(const string& record) {
        customer_record = record;
    }

    int gettotal_due() const{
        return total_due;
    }

    void settotal_due(int _total_due) {
        total_due= _total_due;
    }

    int getNumberofRentedCars()  const{
        return number_of_cars_rented;
    }

    void setNumberofRentedCars(int num){
        number_of_cars_rented=num;
    }

    int getIDofCarRented1() const{
        return cars_rented[0];
    }

    void setIDofCarRented1(int _idOfCar){
        cars_rented[0]=_idOfCar;
    }

    int getIDofCarRented2() const{
        return cars_rented[1];
    }

    void setIDofCarRented2(int _idOfCar){
        cars_rented[1]=_idOfCar;
    }

    int getIDofCarRented3() const{
        return cars_rented[2];
    }

    void setIDofCarRented3(int _idOfCar){
        cars_rented[2]=_idOfCar;
    }
};

class Customer_database : public database {
public:
    void readFromCSV(Customer* _customer) {
        ifstream file(database_address);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            int readId = stoi(token);
            if (readId == _customer->getId()) {
                string tempName;
                getline(ss, tempName, ',');
                _customer->setName(tempName);

                getline(ss, token, ',');
                _customer->setPassword(token);

                getline(ss, token, ',');
                _customer->setCustomerRecord(token);

                getline(ss, token, ',');
                _customer->settotal_due(stoi(token));

                getline(ss, token, ',');
                _customer->setNumberofRentedCars(stoi(token));

                // Read car IDs
                for (int i = 0; i < _customer->getNumberofRentedCars(); ++i) {
                    getline(ss, token, ',');
                    if (i == 0) _customer->setIDofCarRented1(stoi(token));
                    else if (i == 1) _customer->setIDofCarRented2(stoi(token));
                    else if (i == 2) _customer->setIDofCarRented3(stoi(token));
                }
                break;
            }
        }
        file.close();
    }

    void writeToCSV(Customer* _customer) {
        ofstream file(database_address, ios::app);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }
        file << _customer->getId() << "," << _customer->getName() << "," << _customer->getPassword() << ","
             << _customer->getCustomerRecord() << "," << _customer->gettotal_due() << "," << _customer->getNumberofRentedCars();

        for (int i = 0; i < 3; ++i) {
            if (i == 0) file << "," << _customer->getIDofCarRented1();
            else if (i == 1) file << "," << _customer->getIDofCarRented2();
            else if (i == 2) file << "," << _customer->getIDofCarRented3();
        }
        file << endl;
        file.close();
    }

    void updateInCSV(Customer* _customer) {
        ifstream file(database_address);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }
        vector<string> lines;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            int readId = stoi(token);
            if (readId == _customer->getId()) {
                line = to_string(_customer->getId()) + "," + _customer->getName() + "," + _customer->getPassword() + ","
                       + _customer->getCustomerRecord() + "," + to_string(_customer->gettotal_due()) + ","
                       + to_string(_customer->getNumberofRentedCars());

                for (int i = 0; i < 3; ++i) {
                    if (i == 0) line += "," + to_string(_customer->getIDofCarRented1());
                    else if (i == 1) line += "," + to_string(_customer->getIDofCarRented2());
                    else if (i == 2) line += "," + to_string(_customer->getIDofCarRented3());
                }
            }
            lines.push_back(line);
        }
        file.close();

        ofstream outFile(database_address);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }
        for (const auto& line : lines) {
            outFile << line << endl;
        }
        outFile.close();
    }
   
    void deleteFromCSV(int customerId) {
        ifstream file(database_address);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }

        vector<string> lines;
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            int readId = stoi(token);
            if (readId != customerId) {
                lines.push_back(line);
            }
        }
        file.close();

        ofstream outFile(database_address);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }

        for (const auto& line : lines) {
            outFile << line << endl;
        }
        outFile.close();

        cout << "Customer with ID " << customerId << " deleted from " << database_address << endl;
    }
};

Customer_database CustomerDB;                                         //Global class for customer database.

//===============================================================================================================================
//CAR

class car {
public:
    int id;
    string model;
    string condition;
    int rent_price;
    int rent_person_role;
    int rent_person_id;
    string due_date;

    car(int _id){
        this->id=_id;
    }
    car(int id, string model, int rent_price) {
        this->id = id;
        this->model = model;
        this->condition = "good";
        this->rent_price = rent_price;
        this->rent_person_role = -1;
        this->rent_person_id = -1;
        this->due_date="01012025";
    }

    void return_car() {
        if (rent_person_role == -1) {
            cout << "Car is not currently rented." << endl;
            return;
        }

        rent_person_role = -1;
        rent_person_id = -1;

    }

    void display_to_user() {
        cout << "Car ID: " << id << endl;
        cout << "Model: " << model << endl;
        cout << "Rent Price/Day: Rs." << rent_price << endl;
        cout<< "Due Date: "<< due_date<<endl;
        
    }

    bool rent_request_employee(Employee* _employee,int days,string current_date);

    bool rent_request_customer(Customer* _customer,int days,string current_date);
};

class car_database: public database{
    public:
    void viewAvailableCars() {
    ifstream file(database_address);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }
    cout << setw(5) << "ID" << setw(15) << "Model" << setw(15) << "Condition" << setw(10) << "    Rent Price/Day" << endl;
    cout << "===========================================================" << endl;

    string line;
    while (getline(file, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            string id = token;
            getline(ss, token, ',');
            string model = token;
            getline(ss, token, ','); 
            string condition = token;
            getline(ss, token, ','); 
            string rent_price = token;
            getline(ss, token, ','); 
            if (token == "-1") {
                cout << setw(5) << id << setw(15) << model << setw(15) << condition << setw(10) << rent_price << endl;
            }
    }
    file.close();
}


    void viewAllCars() {
        ifstream file(database_address);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << database_address << endl;
            return;
        }
        cout << setw(5) << "ID" << setw(15) << "Model" << setw(15) << "Condition" << setw(10) << "    Rent Price/Day" << setw(15) << "Rent Person" << setw(15) << "Rent Person ID" << setw(15) << "Due Date" << endl;
        cout << "=============================================================================================" << endl;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, ',');
            cout << setw(5) << token;
            getline(ss, token, ',');
            cout << setw(15) << token;
            getline(ss, token, ',');
            cout << setw(15) << token;
            getline(ss, token, ',');
            cout << setw(10) << token;
            getline(ss, token, ',');
            string rent_person;
            if (token == "0") {
                rent_person = "Employee";
            } else if (token == "-1") {
                rent_person = "Not Rented";
            } else {
                rent_person = "Customer";
            }
            cout << setw(15) << rent_person;
            getline(ss, token, ',');
            if(rent_person=="Not Rented"){
                cout<<setw(15)<<"N/A";
            }
            else cout << setw(15) << token ;
            getline(ss, token);
            if(rent_person=="Not Rented"){
                cout<<setw(15)<<"N/A"<<endl;
            }
            else{
                string day = token.substr(0, 2);
                string month = token.substr(2, 2);
                string year = token.substr(4, 4);
                cout << setw(15) << day + "-" + month + "-" + year<<endl;
            }
        }
        file.close();
    }
    
    void readFromCSV(car* _car) {
    ifstream file(database_address);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        int readId = stoi(token);
        if (readId == _car->id) {
            getline(ss, _car->model, ',');
            getline(ss, _car->condition, ',');
            getline(ss, token, ',');
            _car->rent_price = stoi(token);
            getline(ss, token, ',');
            _car->rent_person_role = stoi(token);
            getline(ss, token, ',');
            _car->rent_person_id = stoi(token);
            getline(ss, token);
            _car->due_date = token; 
            break;
        }
    }
    file.close();
}

void updateInCSV(car *_car) {
    ifstream file(database_address);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        int readId = stoi(token);
        if (readId == _car->id) {
            line = to_string(_car->id) + "," + _car->model + "," + _car->condition + "," + to_string(_car->rent_price) + "," +
                   to_string(_car->rent_person_role) + "," + to_string(_car->rent_person_id) + "," + _car->due_date;
        }
        lines.push_back(line);
    }
    file.close();

    ofstream outFile(database_address);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }
    for (const auto& line : lines) {
        outFile << line << endl;
    }
    outFile.close();
    cout << "Car information updated in " << database_address << endl;
}


void writeToCSV(car* _car) {
    ofstream file(database_address, ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }
    file << _car->id << "," << _car->model << "," << _car->condition << "," << _car->rent_price << ","
         << _car->rent_person_role << "," << _car->rent_person_id << "," << _car->due_date << endl;
    file.close();
    cout << "Car information written to " << database_address << endl;
}

void deleteFromCSV(int _id) {
    ifstream file(database_address);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }

    vector<string> lines;
    string line;
    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        int readId = stoi(token);
        
        if (readId == _id) {
            found = true;
            continue; 
        }
        
        lines.push_back(line);
    }

    file.close();

    ofstream outFile(database_address);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open file " << database_address << endl;
        return;
    }

    for (const auto &line : lines) {
        outFile << line << endl;
    }

    outFile.close();

    if (found) {
        cout << "Car with ID " << _id << " deleted from " << database_address << endl;
    } else {
        cout << "Car with ID " << _id << " not found in the database." << endl;
    }
}

};

car_database CarDB;

//===============================================================================================================================
//MANAGER

class Manager: public User, public Customer_database,public Employee_database, public car_database{
    public:
    void modifyRecord_customer(int modify_operation){    // modify_operation 1 for add, 2 for update, 3 for delete
        if(modify_operation==1){          //ADDING A CUSTOMER
            string new_customer_name,new_customer_pwd;
            cout<<"Please Enter Customer Name:(please enter only first name with no spaces) ";
            cin>>new_customer_name;
            cout<<"Please Enter Customer Password: ";
            cin>>new_customer_pwd;
            int new_customer_id=findMinimumMissingID(CustomerDB.get_database_address());
            Customer new_customer(new_customer_id,new_customer_name,new_customer_pwd);
            CustomerDB.writeToCSV(&new_customer);
            cout<<"New customer has been added. Details are as follows: "<<endl;
            cout<<"ID: "<<new_customer_id<<",  "<<"Name: "<<new_customer_name<<endl;
        }
        else if(modify_operation==2){   //UPDATING A CUSTOMER RECORD
            int customer_id;
            cout<<"Please Enter the ID of customer you want to update: ";
            cin>>customer_id;
            Customer c1(customer_id);
            CustomerDB.readFromCSV(&c1);
            cout<<"Current details of customer with id "<<customer_id<<" are: "<<endl;
            cout<<"Name: "<<c1.getName()<<" Password: "<<c1.getPassword()<<" Customer Record: "<<c1.getCustomerRecord()<<endl;
            string new_name,new_pwd, new_customer_rcd;
            cout<<"Enter new name:(please enter only first name with no spaces) ";
            cin>>new_name;
            cout<<"Enter new Password: ";
            cin>>new_pwd;
            cout<<"Enter new customer record: "<<endl;
            cout<<"Press 1 if customer's new record is good"<<endl;
            cout<<"Press 2 if customer's new record is average"<<endl;
            cout<<"Press 3 if customer's new record is poor"<<endl;
            char temp;
            cin>>temp;
            if(temp=='1') new_customer_rcd="good";
            else if(temp=='2') new_customer_rcd="average";
            else new_customer_rcd="poor";
            c1.setName(new_name);
            c1.setCustomerRecord(new_customer_rcd);
            c1.setPassword(new_pwd);
            CustomerDB.updateInCSV(&c1);
        }
        else{                             //DELETING A CUSTOMER RECORD
            int customer_id;
            cout<<"Please Enter the id of customer you want to delete: ";
            cin>>customer_id;
            CustomerDB.deleteFromCSV(customer_id);
        }
    }

    void modifyRecord_employee(int modify_operation){    // modify_operation 1 for add, 2 for update, 3 for delete
        if(modify_operation==1){          //ADDING A CUSTOMER
            string new_employee_name,new_employee_pwd;
            cout<<"Please Enter Employee Name:(please enter only first name with no spaces) ";
            cin>>new_employee_name;
            cout<<"Please Enter Employee Password: ";
            cin>>new_employee_pwd;
            int new_employee_id=findMinimumMissingID(EmployeeDB.get_database_address());
            Employee new_employee(new_employee_id,new_employee_name,new_employee_pwd);
            EmployeeDB.writeToCSV(&new_employee);
            cout<<"New Employee has been added. Details are as follows: "<<endl;
            cout<<"ID: "<<new_employee_id<<",  "<<"Name: "<<new_employee_name<<endl;
        }
        else if(modify_operation==2){   //UPDATING A CUSTOMER RECORD
            int employee_id;
            cout<<"Please Enter the ID of employee you want to update: ";
            cin>>employee_id;
            Employee c1(employee_id);
            EmployeeDB.readFromCSV(&c1);
            cout<<"Current details of employee with id "<<employee_id<<" are: "<<endl;
            cout<<"Name: "<<c1.getName()<<" Password: "<<c1.getPassword()<<" Employee Record: "<<c1.getEmployeeRecord()<<endl;
            string new_name,new_pwd, new_employee_rcd;
            cout<<"Enter new name:(please enter only first name with no spaces) ";
            cin>>new_name;
            cout<<"Enter new Password: ";
            cin>>new_pwd;
            cout<<"Enter new employee record: "<<endl;
            cout<<"Press 1 if employee's new record is good"<<endl;
            cout<<"Press 2 if employee's new record is average"<<endl;
            cout<<"Press 3 if employee's new record is poor"<<endl;
            char temp;
            cin>>temp;
            if(temp=='1') new_employee_rcd="good";
            else if(temp=='2') new_employee_rcd="average";
            else new_employee_rcd="poor";
            c1.setName(new_name);
            c1.setEmployeeRecord(new_employee_rcd);
            c1.setPassword(new_pwd);
            EmployeeDB.updateInCSV(&c1);
        }
        else{                             //DELETING A CUSTOMER RECORD
            int employee_id;
            cout<<"Please Enter the id of employee you want to delete: ";
            cin>>employee_id;
            EmployeeDB.deleteFromCSV(employee_id);
        }
    }

    void modifyRecord_car(int modify_operation){     // modify_operation 1 for add, 2 for update, 3 for delete
        if(modify_operation==1){      //ADDING A CAR
            string new_car_model;
            int new_car_rent_price;
            cout<<"Please Enter Car Model: ";
            cin>>new_car_model;
            cout<<"Please Enter Car Rent Price/Day: ";
            cin>>new_car_rent_price;
            int new_car_id=findMinimumMissingID("cars.csv");
            car new_car(new_car_id,new_car_model,new_car_rent_price);
            CarDB.writeToCSV(&new_car);
            cout<<"New car has been added. Details are as follows: "<<endl;
            cout<<"ID: "<<new_car_id<<",  "<<"Model: "<<new_car_model<<",  "<<"Rent Price/Day: "<<new_car_rent_price<<endl;
        }
        else if(modify_operation==2){    //UPDATING A CAR RECORD
            int car_id;
            cout<<"Please Enter the ID of car you want to update: ";
            cin>>car_id;
            car c1(car_id);
            CarDB.readFromCSV(&c1);
            cout<<"Current details of car with id "<<car_id<<" are: "<<endl;
            cout<<"Model: "<<c1.model<<" Condition: "<<c1.condition<<" Rent Price/Day: "<<c1.rent_price<<endl;
            string new_model,new_condition;
            int new_rent_price;
            cout<<"Enter new model: ";
            cin>>new_model;
            cout<<"Enter new condition: ";
            cin>>new_condition;
            cout<<"Enter new rent price/day: ";
            cin>>new_rent_price;
            c1.model=new_model;
            c1.condition=new_condition;
            c1.rent_price=new_rent_price;
            CarDB.updateInCSV(&c1);
        }
        else{                        //DELETING A CAR RECORD
            int car_id;
            cout<<"Please Enter the id of car you want to delete: ";
            cin>>car_id;
            CarDB.deleteFromCSV(car_id);
        }
    }
};

//===============================================================================================================================
//RENT FUNCTIONALITY

bool car::rent_request_employee(Employee* _employee,int days,string current_date) {
    if (rent_person_role != -1) {
        cout << "Car is already rented." << endl;
        return false;
    }

    int max_allowed_cars = 0;
    if (_employee->getEmployeeRecord() == "good") {
        max_allowed_cars = 3;
    } 
    else if (_employee->getEmployeeRecord() == "average") {
        max_allowed_cars = 2;
    } 
    else {
        max_allowed_cars = 1;
    }

    if (_employee->getNumberofRentedCars() >= max_allowed_cars) {
        cout << "Employee has reached the maximum limit for renting cars." << endl;
        return false;
    }

    rent_person_role = 0; // Employee role
    rent_person_id = _employee->getId();
    string _due_date=addDaysToDate(current_date,days);
    due_date=_due_date;
    CarDB.updateInCSV(this);

    cout << "Car rented to Employee ID " << _employee->getId() << " successfully." << endl;
    return true;
}

bool car::rent_request_customer(Customer* _customer,int days,string current_date) {
    if (rent_person_role != -1) {
        cout << "Car is already rented." << endl;
        return false;
    }

    int max_allowed_cars = 0;
    if (_customer->getCustomerRecord() == "good") {
        max_allowed_cars = 3;
    } else if (_customer->getCustomerRecord() == "average") {
        max_allowed_cars = 2;
    } else {
        max_allowed_cars = 1;
    }
    if (_customer->getNumberofRentedCars() >= max_allowed_cars) {
        cout << "Customer has reached the maximum limit for renting cars." << endl;
        return false;
    }

    rent_person_role = 1; // Customer role
    rent_person_id = _customer->getId();
    string _due_date=addDaysToDate(current_date,days);
    due_date=_due_date;
    CarDB.updateInCSV(this);

    cout << "Congrats! "<<_customer->getName()<<". Car is rented to you successfully." << endl;
    return true;
}

void Employee::rentcar() {
    cout<<"Enter the id of the car you want to rent: "<<endl;
    int _id;
    cin>>_id;
    car _car(_id);
    CarDB.readFromCSV(&_car);
    int _days;
    string current_date;
    cout<<"Please Enter Below details and we'll check if the car is available for renting!"<<endl;
    cout<<"Enter today's date in DDMMYYYY format (no spaces,example: Enter-> 05032024 if today's date is  5 march 2024): "<<endl;
    cin>>current_date;
    cout<<"Please input the desired rental duration for the car in days: "<<endl;
    cin>>_days;
    bool canRent = _car.rent_request_employee(this,_days,current_date);
    if (canRent) {
        total_due += _days* 0.75 * _car.rent_price;

        number_of_cars_rented++;
        if (number_of_cars_rented == 1) {
            setIDofCarRented1(_car.id);
        } else if (number_of_cars_rented == 2) {
            setIDofCarRented2(_car.id);
        } else if (number_of_cars_rented == 3) {
            setIDofCarRented3(_car.id);
        }

        string _day = _car.due_date.substr(0, 2);
        string _month = _car.due_date.substr(2, 2);
        string _year = _car.due_date.substr(4, 4);
        
        cout << "Due Date: "<<_day + "-" + _month + "-" + _year<<endl;
        cout << "Price of this car rent: Rs." << 0.75 * _car.rent_price*_days << endl;
        cout << "Current total_due: Rs." << total_due << endl;
        EmployeeDB.updateInCSV(this);
    } else {
        cout << "Sorry! Car cannot be rented." << endl;
    }
}

void Customer::rentcar() {
    cout<<"Enter the id of the car you want to rent: "<<endl;
    int _id;
    cin>>_id;
    car _car(_id);
    CarDB.readFromCSV(&_car);
    int _days;
    string current_date;
    cout<<"Please Enter Below details and we'll check if the car is available for renting!"<<endl;
    cout<<"Enter today's date in DDMMYYYY format (no spaces,example: Enter-> 05032024 if today's date is  5 march 2024): "<<endl;
    cin>>current_date;
    cout<<"Please input the desired rental duration for the car in days: "<<endl;
    cin>>_days;
    bool canRent = _car.rent_request_customer(this,_days,current_date);
    if (canRent) {

        total_due +=_days*_car.rent_price;

        number_of_cars_rented++;
        if (number_of_cars_rented == 1) {
            setIDofCarRented1(_car.id);
        } else if (number_of_cars_rented == 2) {
            setIDofCarRented2(_car.id);
        } else if (number_of_cars_rented == 3) {
            setIDofCarRented3(_car.id);
        }
        string _day = _car.due_date.substr(0, 2);
        string _month = _car.due_date.substr(2, 2);
        string _year = _car.due_date.substr(4, 4);
        
        cout << "Due Date: "<<_day + "-" + _month + "-" + _year<<endl;
        cout << "Price of this car rent: Rs." << _car.rent_price*_days << endl;
        cout << "Current total_due: Rs." << total_due << endl;

        CustomerDB.updateInCSV(this);
    } else {
        cout << "Sorry! Car cannot be rented." << endl;
    }
}

//===============================================================================================================================
//RETURN CAR FUNCTIONALITY

void Employee::returncar() {
    cout << "Enter the id of the car you want to return: " << endl;
    cout<<"Enter -1 to exit!"<<endl;
    int _id;
    cin >> _id;
    if(_id<0) return;
    car _car(_id);
    CarDB.readFromCSV(&_car);
    if (_car.rent_person_id != this->id || _car.rent_person_role!=0) {
        cout << "You have not rented this car" << endl;
        return;
    }
    cout << "Enter today's date in the format DDMMYYYY (no spaces,example: Enter-> 05032024 if today's date is  5 march 2024): " << endl;
    string today_date;
    cin >> today_date;
    cout << "Enter the condition of the car: " << endl;
    cout << "Press 1 if car condition is good" << endl;
    cout << "Press 2 if car condition is average" << endl;
    cout << "Press 3 if car condition is poor" << endl;
    char condition;
    cin >> condition;

    int days_late = calculateDaysLate(today_date, _car.due_date);
    if(days_late<0) days_late=0;

    int late_return_fine = days_late * late_return_fine_per_day;

    int bad_condition_fine = 0;
    if (condition == '1') {
        bad_condition_fine = 0;
    } else if (condition == '2') {
        bad_condition_fine = 1*Bad_condition_fine;
    } else if (condition == '3') {
        bad_condition_fine = 2*Bad_condition_fine;
    }

    int total_fine = late_return_fine + bad_condition_fine;

    total_due += total_fine;
    int is_employee_record_changed=0;

    if(total_fine>0 && total_fine<10000){                     //UPDATING EMPLOYEE RECORD BASED ON LATE RETURN AND CONDITION OF CAR RETURNED
        if(this->getEmployeeRecord()=="good"){
            is_employee_record_changed=1;
            this->employee_record="average";
        }
        else if(this->getEmployeeRecord()=="average"){
            is_employee_record_changed=1;
            this->employee_record="poor";
        }
    }

    if(total_fine>10000){
        is_employee_record_changed=1;
        this->employee_record="poor";
    }

    number_of_cars_rented--;
    if (_car.id == getIDofCarRented1()) {
        int IDofCarRented2=getIDofCarRented2();
        int IDofCarRented3=getIDofCarRented3();
        setIDofCarRented1(IDofCarRented2);
        setIDofCarRented2(IDofCarRented3);
        setIDofCarRented3(-1);
    } else if (_car.id == getIDofCarRented2()) {
        int IDofCarRented3=getIDofCarRented3();
        setIDofCarRented2(IDofCarRented3);
        setIDofCarRented3(-1);
    } else if (_car.id == getIDofCarRented3()) {
        setIDofCarRented3(-1);
    }

    cout << "Car returned successfully!" << endl;
    cout << "Days late: " << days_late << endl;
    cout << "Late return fine: Rs." << late_return_fine << endl;
    cout << "Bad condition fine: Rs." << bad_condition_fine << endl;
    cout << "total_fine: Rs." << total_fine << endl;
    cout << "Current total dues: Rs." << total_due << endl;
    if(is_employee_record_changed)  cout << "Your employee record is updated to: "<< getEmployeeRecord() << endl;
    _car.return_car();
    CarDB.updateInCSV(&_car);
    EmployeeDB.updateInCSV(this);
}

void Customer::returncar() {
    cout << "Enter the id of the car you want to return: " << endl;
    cout<<"Enter -1 to exit!"<<endl;
    int _id;
    cin >> _id;
    if(_id<0) return;
    car _car(_id);
    CarDB.readFromCSV(&_car);
    if (_car.rent_person_id != this->id || _car.rent_person_role!=1) {
        cout << "You have not rented this car" << endl;
        return;
    }
    cout << "Enter today's date in the format DDMMYYYY (no spaces, example: Enter-> 05032024 if today's date is  5 march 2024): " << endl;
    string today_date;
    cin >> today_date;
    cout << "Enter the condition of the car: " << endl;
    cout << "Press 1 if car condition is good" << endl;
    cout << "Press 2 if car condition is average" << endl;
    cout << "Press 3 if car condition is poor" << endl;
    char condition;
    cin >> condition;

    int days_late = calculateDaysLate(today_date, _car.due_date);
     if(days_late<0) days_late=0;
    int late_return_fine = days_late * late_return_fine_per_day;

    int bad_condition_fine = 0;
    if (condition == '1') {
        bad_condition_fine = 0;
    } else if (condition == '2') {
        bad_condition_fine = 1*Bad_condition_fine;
    } else if (condition == '3') {
        bad_condition_fine = 2*Bad_condition_fine;
    }

    int total_fine = late_return_fine + bad_condition_fine;

    total_due += total_fine;
    int is_customer_record_changed=0;
    if(total_fine>0 && total_fine<10000){                       //UPDATING CUSTOMER RECORD BASED ON LATE RETURN AND CONDITION OF CAR RETURNED
        if(this->getCustomerRecord()=="good"){
            is_customer_record_changed=1;
            this->customer_record="average";
        }
        else if(this->getCustomerRecord()=="average"){
            is_customer_record_changed=1;
            this->customer_record="poor";
        }
    }

    if(total_fine>10000){
        is_customer_record_changed=1;
        this->customer_record="poor";
    }

    number_of_cars_rented--;
    if (_car.id == getIDofCarRented1()) {
        int IDofCarRented2=getIDofCarRented2();
        int IDofCarRented3=getIDofCarRented3();
        setIDofCarRented1(IDofCarRented2);
        setIDofCarRented2(IDofCarRented3);
        setIDofCarRented3(-1);
    } else if (_car.id == getIDofCarRented2()) {
        int IDofCarRented3=getIDofCarRented3();
        setIDofCarRented2(IDofCarRented3);
        setIDofCarRented3(-1);
    } else if (_car.id == getIDofCarRented3()) {
        setIDofCarRented3(-1);
    }

    cout << "Car returned successfully!" << endl;
    cout << "Days late: " << days_late << endl;
    cout << "Late return fine: Rs." << late_return_fine << endl;
    cout << "Bad condition fine: Rs." << bad_condition_fine << endl;
    cout << "Total fine: Rs." << total_fine << endl;
    cout << "Current total_due: Rs." << total_due << endl;
    if(is_customer_record_changed)  cout << "Your customer record is updated to: "<< getCustomerRecord() << endl;

    _car.return_car();
    CarDB.updateInCSV(&_car);
    CustomerDB.updateInCSV(this);
}

//===============================================================================================================================
//DISPLAYING RENTED CAR FUNCTIONALITY

void Employee::display_rented_cars() {
    cout << "Rented Cars:" << endl;
    cout << "---------------------------" << endl;
    
    // Iterate through rented cars array
    for (int i = 0; i < number_of_cars_rented; ++i) {
        // Access car details from the database using car ID
        car rented_car(cars_rented[i]);
        CarDB.readFromCSV(&rented_car);
        
        // Display car details
        cout << "Car ID: " << rented_car.id << endl;
        cout << "Model: " << rented_car.model << endl;
        cout << "Rent Price/Day: Rs." << rented_car.rent_price << endl;
        string token=rented_car.due_date;
        string day = token.substr(0, 2);
        string month = token.substr(2, 2);
        string year = token.substr(4, 4);
        
        cout << "Due Date: "<<day + "-" + month + "-" + year<<endl;
        cout << "---------------------------" << endl;
    }
}

void Customer::display_rented_cars() {
    cout << "Rented Cars:" << endl;
    cout << "---------------------------" << endl;
    
    for (int i = 0; i < number_of_cars_rented; ++i) {
        car rented_car(cars_rented[i]);
        CarDB.readFromCSV(&rented_car);
        
        cout << "Car ID: " << rented_car.id << endl;
        cout << "Model: " << rented_car.model << endl;
        cout << "Rent Price/Day: Rs." << rented_car.rent_price << endl;
        string token=rented_car.due_date;
        string day = token.substr(0, 2);
        string month = token.substr(2, 2);
        string year = token.substr(4, 4);
        
        cout << "Due Date: "<<day + "-" + month + "-" + year<<endl;
        cout << "---------------------------" << endl;
    }
}

//===============================================================================================================================
//MANAGER DATABASE
string managerDB;
//================================================================================================================================
// User Authentication

int loggedin_id;

bool verifyEmployeePassword(int id, const string& password) {                  //User Authentication for Employee
    ifstream file(EmployeeDB.get_database_address());
    if (!file.is_open()) {
        cerr << "Error: Could not open employees.csv" << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ','); 
        int readId = stoi(token);
        if (readId == id) {
            getline(ss, token, ',');  //since password is the third attribute of database 
            getline(ss, token, ',');   
            if (token == password) {
                file.close();
                return true; 
            }
        }
    }
    file.close();
    return false; 
}

bool verifyCustomerPassword(int id, const string& password) {                  //User Authentication for Customer
    ifstream file(CustomerDB.get_database_address());
    if (!file.is_open()) {
        cerr << "Error: Could not open customers.csv" << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ','); // Read ID
        int readId = stoi(token);
        if (readId == id) {
            getline(ss, token, ','); // Skip name
            getline(ss, token, ','); // Read password
            if (token == password) {
                file.close();
                return true; 
            }
        }
    }
    file.close();
    return false; 
}

bool verifyManagerPassword(int id,const string& password){                      //User Authentication for Manager
    ifstream file(managerDB);
    if (!file.is_open()) {
        cerr << "Error: Could not open Managers.csv" << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ','); // Read ID
        int readId = stoi(token);
        if (readId == id) {
            getline(ss, token, ','); // Skip name
            getline(ss, token, ','); // Read password
            if (token == password) {
                file.close();
                return true; 
            }
        }
    }
    file.close();
    return false;
}

bool verifyid() {
    cout << "Enter your role (m->Manager, e->Employee, c->Customer)               Press x to Exit    ";
    cin >> role;                                        
                                                    
    if(role == 'x') {                                     
        return false;                               
    }

    int id;
    string password;

    if(role == 'e') {
        cout << "Please Enter your Employee ID: ";
        cin >> id;
        loggedin_id=id;
        cout << "Please Enter your password: ";
        cin >> password;

        if(verifyEmployeePassword(id, password)) {
            cout << "Employee login successful!" << endl;
        } else {
            cout << "Invalid Employee ID or password. Please try again." << endl;
            return verifyid(); 
        }
    }
    else if(role == 'c') {
        cout << "Please Enter your Customer ID: ";
        cin >> id;
        loggedin_id=id;
        cout << "Please Enter your password: ";
        cin >> password;

        if(verifyCustomerPassword(id, password)) {
            cout << "Customer login successful!" << endl;
        } else {
            cout << "Invalid Customer ID or password. Please try again." << endl;
            return verifyid();
        }
    }
    else if(role == 'm') {
        cout << "Please Enter your Manager ID: ";
        cin >> id;
        loggedin_id=id;
        cout << "Please Enter your password: ";
        cin >> password;

        if(verifyManagerPassword(id, password)) {
            cout << "Manager login successful!" << endl;
        } else {
            cout << "Invalid Manager ID or password. Please try again." << endl;
            return verifyid();
        }
    }
    else {
        cout << "Invalid role. Please try again." << endl;
        return verifyid(); 
    }
    return true;
}
//=================================================================================================================================

//Main Function

int main() {
    late_return_fine_per_day=100;
    Bad_condition_fine=500;

    EmployeeDB.set_database_address("employees.csv");    //set employee database address
    // Format: Employee ID,Name,Password,Employee Record (good, average, or poor),Fine Imposed,Number of Cars Rented,ID of Car Rented 1 (-1 if not rented),ID of Car Rented 2 (-1 if not rented),ID of Car Rented 3 (-1 if not rented)
    
    CustomerDB.set_database_address("customers.csv");    //set customers database address
    // Format :Customer ID,Name,Password,Employee Record (good, average, or poor),Fine Imposed,Number of Cars Rented,ID of Car Rented 1 (-1 if not rented),ID of Car Rented 2 (-1 if not rented),ID of Car Rented 3 (-1 if not rented)
    

    CarDB.set_database_address("cars.csv");              //set car database address

    managerDB="Managers.csv";                            //set managers database address
    
        while(true){
        cout<<"Hello! Welcome to the Agrawal Car Rentals!"<<endl;
        cout<<"Enter any character to get started"<<"                      "<<"Enter x to exit    ";
        char c;
        cin>> c;
        if(c=='x') break;
        else{
            while(true){
                bool proceed=verifyid();
                if(proceed==false) break;
                else{
                    if(role=='m'){
                        Manager loggedin_Manager;
                        while(true){
                            cout<<endl;
                            cout<<"MANAGER DASHBOARD"<<endl;
                            cout<<"========================"<<endl;
                            cout<<"Press 1 to update/delete/add any car!"<<endl;
                            cout<<"Press 2 to update/delete/add any customer!"<<endl;
                            cout<<"Press 3 to update/delete/add any Employee!"<<endl;
                            cout<<"Press 4 to see details of all the cars in store!"<<endl;
                            cout<<"Press any other number to log out!"<<endl;
                            char manager_action;
                            cin>>manager_action;
                            if(manager_action=='1'){
                                while(true){
                                    cout<<"Press 1 to add a new car"<<endl;
                                    cout<<"Press 2 to update details of existing car"<<endl;
                                    cout<<"Press 3 to delete an existing car"<<endl;
                                    cout<<"Press any other number to exit!"<<endl;
                                    char modify_action;
                                    cin>>modify_action;
                                    if(modify_action!='1' &&  modify_action!='2' && modify_action!='3'){
                                        break;
                                    }
                                    else loggedin_Manager.modifyRecord_car((int)modify_action-48);
                                }

                            }
                            else if(manager_action=='2'){
                                cout<<"Press 1 to add a new customer"<<endl;
                                cout<<"Press 2 to update details of existing customer"<<endl;
                                cout<<"Press 3 to delete an existing customer"<<endl;
                                cout<<"Press any other number to exit!"<<endl;
                                char modify_action;
                                cin>>modify_action;
                                if(modify_action!='1' &&  modify_action!='2' && modify_action!='3'){
                                    break;
                                }
                                loggedin_Manager.modifyRecord_customer((int)modify_action-48);
                            }
                            else if(manager_action=='3'){
                                cout<<"Press 1 to add a new employee"<<endl;
                                cout<<"Press 2 to update details of existing employee"<<endl;
                                cout<<"Press 3 to delete an existing employee"<<endl;
                                cout<<"Press any other number to exit!"<<endl;
                                char modify_action;
                                cin>>modify_action;
                                if(modify_action!='1' &&  modify_action!='2' && modify_action!='3'){
                                    break;
                                }
                                loggedin_Manager.modifyRecord_employee((int)modify_action-48);
                            }
                            else if(manager_action=='4'){
                                CarDB.viewAllCars();
                            }
                            else{
                                break;
                            }
                        }
                    }
                    else if(role=='e'){
                        while(true){
                            Employee logged_in_employee(loggedin_id);
                            EmployeeDB.readFromCSV(&logged_in_employee);
                            cout<<endl;
                            cout<<"EMPLOYEE DASHBOARD"<<endl;
                            cout<<"========================"<<endl;
                            cout<<"Press 1 to see all the available cars in our store!"<<endl;
                            cout<<"Press 2 to see all the cars you have rented!"<<endl;
                            cout<<"Press 3 to rent a new car!"<<endl;
                            cout<<"Press 4 to clear your due!"<<endl;
                            cout<<"Press 5 to return a car!"<<endl;
                            cout<<"Press any other number to logout!"<<endl;
                            char employee_action;
                            cin>>employee_action;
                            if(employee_action=='1'){
                                CarDB.viewAvailableCars();
                            }
                            else if(employee_action=='2'){
                                logged_in_employee.display_rented_cars();
                            }
                            else if(employee_action=='3'){
                                logged_in_employee.rentcar();
                            }
                            else if(employee_action=='4'){
                                logged_in_employee.clear_due();
                                EmployeeDB.updateInCSV(&logged_in_employee);
                            }
                            else if(employee_action=='5'){
                                logged_in_employee.returncar();
                            }
                            else{
                                break;
                            }
                        }
                    }
                    else{
                        while(true){
                            Customer logged_in_customer(loggedin_id);
                            CustomerDB.readFromCSV(&logged_in_customer);
                            cout<<endl;
                            cout<<"CUSTOMER DASHBOARD"<<endl;
                            cout<<"========================"<<endl;
                            cout<<"Press 1 to see all the available cars in our store!"<<endl;
                            cout<<"Press 2 to see all the cars you have rented!"<<endl;
                            cout<<"Press 3 to rent a new car!"<<endl;
                            cout<<"Press 4 to clear your due!"<<endl;
                            cout<<"Press 5 to return a car!"<<endl;
                            cout<<"Press any other number to logout!"<<endl;
                            char customer_action;
                            cin>>customer_action;
                            if(customer_action=='1'){
                                CarDB.viewAvailableCars();
                            }
                            else if(customer_action=='2'){
                                logged_in_customer.display_rented_cars();
                            }
                            else if(customer_action=='3'){
                                logged_in_customer.rentcar();
                            }
                            else if(customer_action=='4'){
                                logged_in_customer.clear_due();
                                CustomerDB.updateInCSV(&logged_in_customer);
                            }
                            else if(customer_action=='5'){
                                logged_in_customer.returncar();
                            }
                            else{
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

//================================================================================================================================