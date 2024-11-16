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


int main() {
    while (true) {
        cout << "\n=== E-commerce System ===\n";
        cout << "1. Sign Up\n2. Login\n3. Exit\nChoose an option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "\n=== Sign Up ===\n";
            cout << "1. Buyer\n2. Seller\n3. Admin\nChoose user type: ";
            int userType;
            cin >> userType;
            cin.ignore();

            string username, password, email, fullName, phoneNumber;

            cout << "Enter username: ";
            getline(cin, username);
            if (FileHandler::doesUserExist(username)) {
                cout << "Username already exists!\n";
                continue;
            }

            cout << "Enter password (min 6 characters): ";
            getline(cin, password);
            if (!isValidPassword(password)) {
                cout << "Invalid password!\n";
                continue;
            }

            cout << "Enter email: ";
            getline(cin, email);
            if (!isValidEmail(email)) {
                cout << "Invalid email format!\n";
                continue;
            }

            cout << "Enter full name: ";
            getline(cin, fullName);

            cout << "Enter phone number (10 digits): ";
            getline(cin, phoneNumber);
            if (!isValidPhone(phoneNumber)) {
                cout << "Invalid phone number!\n";
                continue;
            }

            User* newUser = nullptr;
            switch (userType) {
                case 1: newUser = new Buyer(username, password, email, fullName, phoneNumber); break;
                case 2: newUser = new Seller(username, password, email, fullName, phoneNumber); break;
                case 3: newUser = new Admin(username, password, email, fullName, phoneNumber); break;
                default: 
                    cout << "Invalid user type!\n";
                    continue;
            }

            newUser->saveToFile();
            cout << "Registration successful!\n";
            delete newUser;

        } else if (choice == 2) {
            cout << "\n=== Login ===\n";
            string username, password;

            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);

            User* user = User::login(username, password);
            if (user) {
                cout << "Login successful!\n";

                if (dynamic_cast<Buyer*>(user)) {
                    cout << "Welcome Buyer: " << username << endl;
                } else if (dynamic_cast<Seller*>(user)) {
                    cout << "Welcome Seller: " << username << endl;
                } else if (Admin* admin = dynamic_cast<Admin*>(user)) {
                    cout << "Welcome Admin: " << username << endl;
                    cout << "1. View All Users\n2. View All Transactions\nChoose an option: ";
                    int adminChoice;
                    cin >> adminChoice;
                    cin.ignore();

                    if (adminChoice == 1) admin->viewAllUsers();
                    else if (adminChoice == 2) admin->viewAllTransactions();
                }
                delete user;
            } else {
                cout << "Invalid username or password.\n";
            }
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
