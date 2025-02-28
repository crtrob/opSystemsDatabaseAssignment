// Filename: databaseAssignment.cpp
// Description: Code for an assignment in COSC-A365-001 at Loyola University New
// Orleans, Operating Systems. This assignment involves a company that wants to 
// create a database to store employee IDs, names and incomes. This is a program
// which creates, searches, and modifies the database in a .txt file.
// Author: Carter Roberts
// Instructor: Omar EL Khatib, Prof. @ Loyola University
// File Created: 2/27/2025 (MM/DD/YYYY)
// File Modified: 2/27/2025 (MM/DD/YYYY)

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
using namespace std;

// Declare variables for accessing file & filename:
fstream file;
string filename = "employeeDatabase.txt";
// Declare a struct to store the employee data:
struct Person 
{
    string id;
    string firstName, lastName;
    float income;
};

// 'search' takes an employee id and searches the file for it, then returns a 
// pointer to an employee structure if it is found. otherwise, it returns 0.
struct Person *search(string id)
{
    // create pointer to employee struct to possibly return, as well as
    // c-strings to temporarily store what will be the values in that struct
    struct Person *p2 = new Person;
    char empId[6], firstName[15], lastName[15], incomeStr[10];
    file.clear();
    // start looking through file from beginning
    file.seekg(0, ios::beg);
    // read for each id
    while (file.read(empId, 5))
    {
        // last char of c-string is a null character
        empId[5] = '\0';
        // id value for returned employee struct is value of c-string
        p2->id = empId;
        // if that value matches the employee id searched for on func. call,
        if (p2->id == id)
        {
            // Reading all data for matched Person
            file.read(firstName, 15);
            file.read(lastName, 15);
            file.read(incomeStr, 10);
            // save to returned Person instance
            p2->firstName = firstName;
            p2->lastName = lastName;
            p2->income = stof(incomeStr); // convert to float
            return p2; // returns found employee data
        }
        // 40 because 15(fN) + 15(lN) + 10(i) chars = distance on file to next 
        // employee -- essentially, move to next employee to keep searching
        file.seekg(40, ios::cur); 
    }
    return 0;
}

// asks user to input employee id and calls search() in order to display output
void get_employee()
{
    string empId;
    // Ask the user to enter the employee id to get its information
    cout << "Enter employee ID to search for: ";
    cin >> empId;
    if (empId.size() > 6)
    {
        cout << "That is not a valid employee ID length. Try again: ";
        cin >> empId;
    }
    // Then call the function search with the employee id as a parameter
    Person* employeeGot = search(empId);
    if (employeeGot == nullptr)
    {
        cout << "Employee could not be found." << endl;
        return;
    }
    cout << "Employee found! Here is their information: " << endl;
    cout << "ID: " << employeeGot->id << " F/L Name: " << employeeGot->firstName;
    cout << " " << employeeGot->lastName << " Income: $" << employeeGot->income;
    cout << endl;
    return;
}

// lists all employees' information
void list() 
{
    // define c-strings for employee data features
    char id[6], firstName[15], lastName[15], incomeStr[10];
    file.clear();
    // rewind file to beginning
    file.seekg(0, ios::beg);
    // read for each id
    while (file.read(id, 5))
    {
        // read rest of data
        file.read(firstName, 15);
        file.read(lastName, 15);
        file.read(incomeStr, 10);
        // convert incomeStr into a float
        float income = atof(incomeStr);
        // display employee / Person struct
        cout << id << " " << firstName << " " << lastName << " " << income << endl;
    }
}

// stores data into the file
void write_data(Person p) 
{
    // write data to file
    file.clear();
    // write chars for id
    file.write(p.id.c_str(), 5);
    // write chars for names
    file.write(p.firstName.c_str(), 15);
    file.write(p.lastName.c_str(), 15);
    // write chars for income after converting to string
    file.write(to_string(p.income).c_str(), 10);
    // force the write to the file
    file.flush();
}

// necessary to check that a new employee's inputted income is a float-convertible string
// with 10 or less numbers in it
bool validIncome(string& str)
{
    int decimals = 0;
    int digits = 0;

    for (char c : str)
    {
        if (isdigit(c))
        {
            // if c is 0-9, that's one more digit
            digits++;
        }
        else if (c == '.' && decimals == 0)
        {
            // if c is not 0-9 but is ., that's one more decimal
            decimals++;
        }
        else if (c == '-' && &c == &str[0])
        {
            // if c is the first digit and is a negative sign, just iterate loop
            continue;
        }
        else
        {
            // anything else automatically invalidates the income
            return false;
        }
    }
    if (digits > 10) return false;
    return true;
}

// asks user about new employee data and calls write_data() to save that data
void new_employee() 
{
    string empId;
    string empFn;
    string empLn;
    string empInStr;

    // (1) just means it will keep going as long as no break within is triggered
    while (1)
    {
        cout << endl << "*** New Employee ***" << endl;

        cout << "Enter employee ID (type -1 to end) (6 or less chars): ";
        cin >> empId;
        // make sure empId is 6 or less characters
        while (empId.size() > 6)
        {
            cout << "That is not a valid Employee ID length. Try again: ";
            cin >> empId;
        }
        // stop if user enters -1
        if (empId == "-1") break;
        // search if user exists, exit if it does, continue if it doesn't
        if (search(empId) != nullptr)
        {
            cout << "ERROR: Employee ID already taken." << endl;
            return;
        }
        
        cout << "Enter employee first name (type -1 to end) (15 or less chars): ";
        cin >> empFn;
        while (empFn.size() > 15)
        {
            cout << "That is not a valid Employee first name length. Try again: ";
            cin >> empFn;
        }
        if (empFn == "-1") break;

        cout << "Enter employee last name (type -1 to end) (15 or less chars): ";
        cin >> empLn;
        while (empLn.size() > 15)
        {
            cout << "That is not a valid Employee last name length. Try again: ";
            cin >> empLn;
        }
        if (empLn == "-1") break;

        cout << "Enter employee income (type -1 to end) (10 or less chars): ";
        cin >> empInStr;
        while (!validIncome(empInStr))
        {
            cout << "That is not a valid Employee income length/value. Try again: ";
            cin >> empInStr;
        }
        if (empInStr == "-1" || empInStr == "-1.0") break;

        // continue & save data to Person struct
        Person p;
        p.id = empId;
        p.firstName = empFn;
        p.lastName = empLn;
        p.income = stof(empInStr);
        write_data(p);
    }
}

// main function
int main()
{
    // open file for read and write
    file.open(filename, ios::in | ios::out);
    // if the file can't be found
    if (!file)
    {
        // create file
        file.open(filename, ios::out);
        file.close();
        file.open(filename, ios::in | ios::out);
    }
    // Main Function Menu for all options with database
    int option = -1;
    while (1)
    {
        cout << "*** Menu ***" << endl;
        cout << "1) New Employee" << endl;
        cout << "2) Search Employee Information" << endl;
        cout << "3) List Employees" << endl;
        cout << "your option: ";
        cin >> option;
        switch (option)
        {
            case 1: new_employee(); break;
            case 2: get_employee(); break;
            case 3: list(); break;
            // if anything else is entered, exit menu 
            default: return 0;
        }
    }
    // close files
    file.close();
    // exit program
    return 0;
}