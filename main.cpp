#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// File paths for data storage
const string USERS_FILE = "users.txt";
const string PRODUCTS_FILE = "products.txt";
const string ORDERS_FILE = "orders.txt";
const string TRANSACTIONS_FILE = "transactions.txt";

// Utility class for handling file operations
class FileHandler {
public:
    static bool doesUserExist(const string& username) {
        ifstream file(USERS_FILE);
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string user;
            getline(ss, user, ',');
            if (user == username) {
                return true;
            }
        }
        return false;
    }

    static void saveToFile(const string& filename, const string& data) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << data << endl;
        }
        file.close();
    }
};

// Base User class
class User {
protected:
    string username;
    string password;
    string email;
    string fullName;
    string phoneNumber;
    string userType;

public:
    User(string uname, string pwd, string mail, string name, string phone, string type)
        : username(uname), password(pwd), email(mail), fullName(name), phoneNumber(phone), userType(type) {}

    virtual void saveToFile() {
        string userData = username + "," + password + "," + email + "," + userType + "," + fullName + "," + phoneNumber;
        FileHandler::saveToFile(USERS_FILE, userData);
    }

    static User* login(const string& username, const string& password);
};

class Buyer : public User {
public:
    Buyer(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "buyer") {}

    void saveOrder(const string& orderDetails) {
        string orderData = username + "," + orderDetails + "," + to_string(time(0));
        FileHandler::saveToFile(ORDERS_FILE, orderData);
    }
};

class Seller : public User {
public:
    Seller(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "seller") {}

    void addProduct(const string& productName, double price, int quantity) {
        string productData = username + "," + productName + "," + to_string(price) + "," + to_string(quantity);
        FileHandler::saveToFile(PRODUCTS_FILE, productData);
    }
};

class Admin : public User {
public:
    Admin(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "admin") {}

    void viewAllUsers() {
        ifstream file(USERS_FILE);
        string line;
        cout << "\nAll Users:\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    void viewAllTransactions() {
        ifstream file(TRANSACTIONS_FILE);
        string line;
        cout << "\nAll Transactions:\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
};

// Static method implementation for User login
User* User::login(const string& username, const string& password) {
    ifstream file(USERS_FILE);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string uname, pwd, email, type, name, phone;
        
        getline(ss, uname, ',');
        getline(ss, pwd, ',');
        getline(ss, email, ',');
        getline(ss, type, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');

        if (uname == username && pwd == password) {
            if (type == "buyer")
                return new Buyer(uname, pwd, email, name, phone);
            else if (type == "seller")
                return new Seller(uname, pwd, email, name, phone);
            else if (type == "admin")
                return new Admin(uname, pwd, email, name, phone);
        }
    }
    return nullptr;
}

// Helper functions for input validation
bool isValidEmail(const string& email) {
    return email.find('@') != string::npos && email.find('.') != string::npos;
}

bool isValidPassword(const string& password) {
    return password.length() >= 6;
}

bool isValidPhone(const string& phone) {
    return phone.length() == 10 && phone.find_first_not_of("0123456789") == string::npos;
}


int main(){
    
}