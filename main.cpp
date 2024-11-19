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

// Buyer Class
class Buyer : public User {
public:
    Buyer(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "buyer") {}

    void viewAvailableProducts() {
        ifstream file(PRODUCTS_FILE);
        string line;
        cout << "\nAvailable Products:\n";
        while (getline(file, line)) {
            stringstream ss(line);
            string seller, product, price, quantity;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');

            if (stoi(quantity) > 0) { // Only display products with stock
                cout << "Product: " << product
                     << " | Price: " << price
                     << " | Quantity: " << quantity
                     << " | Sold by: " << seller << "\n";
            }
        }
        file.close();
    }

    void placeOrder(const string& productName, int quantity) {
        ifstream file(PRODUCTS_FILE);
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        while (getline(file, line)) {
            stringstream ss(line);
            string seller, product, price, stock;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, stock, ',');

            if (product == productName && stoi(stock) >= quantity) {
                found = true;
                int remainingStock = stoi(stock) - quantity;
                tempFile << seller << "," << product << "," << price << "," << remainingStock << endl;

                // Save the order with seller, buyer, product, quantity, and timestamp
                string orderData = seller + "," + username + "," + product + "," + to_string(quantity) + "," + to_string(time(0));
                FileHandler::saveToFile(ORDERS_FILE, orderData);
                cout << "Order placed successfully for " << quantity << " unit(s) of \"" << productName << "\".\n";
            } else {
                tempFile << line << endl;
            }
        }
        file.close();
        tempFile.close();

        remove(PRODUCTS_FILE.c_str());
        rename("temp.txt", PRODUCTS_FILE.c_str());

        if (!found) {
            cout << "Product not found or insufficient stock.\n";
        }
    }
};

// Seller Class
class Seller : public User {
public:
    Seller(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "seller") {}

    void addProduct(const string& productName, double price, int quantity) {
        string productData = username + "," + productName + "," + to_string(price) + "," + to_string(quantity);
        FileHandler::saveToFile(PRODUCTS_FILE, productData);
        cout << "Product \"" << productName << "\" added successfully.\n";
    }

    void editProduct(const string& productName, double newPrice, int newQuantity) {
        ifstream file(PRODUCTS_FILE);
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        while (getline(file, line)) {
            stringstream ss(line);
            string seller, product, price, quantity;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');

            if (seller == username && product == productName) {
                found = true;
                tempFile << seller << "," << product << "," << newPrice << "," << newQuantity << endl;
            } else {
                tempFile << line << endl;
            }
        }
        file.close();
        tempFile.close();

        remove(PRODUCTS_FILE.c_str());
        rename("temp.txt", PRODUCTS_FILE.c_str());

        if (found) {
            cout << "Product \"" << productName << "\" updated successfully.\n";
        } else {
            cout << "Product \"" << productName << "\" not found for the seller \"" << username << "\".\n";
        }
    }

    void deleteProduct(const string& productName) {
        ifstream file(PRODUCTS_FILE);
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        while (getline(file, line)) {
            stringstream ss(line);
            string seller, product, price, quantity;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');

            if (seller == username && product == productName) {
                found = true;
            } else {
                tempFile << line << endl;
            }
        }
        file.close();
        tempFile.close();

        remove(PRODUCTS_FILE.c_str());
        rename("temp.txt", PRODUCTS_FILE.c_str());

        if (found) {
            cout << "Product \"" << productName << "\" deleted successfully.\n";
        } else {
            cout << "Product not found!\n";
        }
    }

    void viewDispatchNotifications() {
        ifstream file(ORDERS_FILE);
        string line;
        bool hasNotifications = false;

        cout << "\n=== Dispatch Notifications ===\n";
        while (getline(file, line)) {
            stringstream ss(line);
            string seller, buyer, product, quantity, timestamp;
            getline(ss, seller, ',');
            getline(ss, buyer, ',');
            getline(ss, product, ',');
            getline(ss, quantity, ',');
            getline(ss, timestamp, ',');

            if (seller == username) {
                hasNotifications = true;
                time_t orderTime = stoi(timestamp);
                cout << "Buyer: " << buyer
                     << " | Product: " << product
                     << " | Quantity: " << quantity
                     << " | Ordered At: " << ctime(&orderTime);
            }
        }
        file.close();

        if (!hasNotifications) {
            cout << "No pending orders for dispatch.\n";
        }
    }

    void viewListedProducts() {
        ifstream file(PRODUCTS_FILE);
        string line;
        cout << "\nYour Listed Products:\n";
        while (getline(file, line)) {
            stringstream ss(line);
            string seller, product, price, quantity;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');

            if (seller == username) {
                cout << "Product: " << product
                     << " | Price: " << price
                     << " | Quantity: " << quantity << "\n";
            }
        }
        file.close();
    }
};

// Admin Class
class Admin : public User {
public:
    Admin(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "admin") {}

    void viewAllUsers() {
        ifstream file(USERS_FILE);
        string line;

        if (!file.is_open()) {
            cout << "Error opening file." << endl;
            return;
        }

        cout << "\nAll Users:\n";
        cout << "Username | Full Name | Role | Email | Phone No | Encrypted Password\n";
        cout << "--------------------------------------------------------\n";

        while (getline(file, line)) {
            stringstream ss(line);
            string username, role, email, phone, password, encryptedPassword, fullname;

            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, email, ',');
            getline(ss, role, ',');
            getline(ss, fullname, ',');
            getline(ss, phone, ',');

            encryptedPassword = string(password.length(), '*');

            cout << username << " | " << fullname << " | " << role << " | " << email << " | " << phone << " | " << encryptedPassword << endl;
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

                if (Buyer* buyer = dynamic_cast<Buyer*>(user)) {
                    while (true) {
                        cout << "\n1. View Available Products\n"
                             << "2. Place Order\n"
                             << "3. Logout\n"
                             << "Choose an option: ";
                        int buyerChoice;
                        cin >> buyerChoice;
                        cin.ignore();

                        if (buyerChoice == 1) {
                            buyer->viewAvailableProducts();
                        } else if (buyerChoice == 2) {
                            string productName;
                            int quantity;

                            cout << "Enter product name: ";
                            getline(cin, productName);
                            cout << "Enter quantity: ";
                            cin >> quantity;
                            cin.ignore();

                            buyer->placeOrder(productName, quantity);
                        } else if (buyerChoice == 3) {
                            cout << "Logging out...\n";
                            break;
                        } else {
                            cout << "Invalid choice! Try again.\n";
                        }
                    }

                } else if (Seller* seller = dynamic_cast<Seller*>(user)) {
                    while (true) {
                        cout << "\n1. Add Product\n"
                             << "2. Edit Product\n"
                             << "3. Delete Product\n"
                             << "4. View Listed Products\n"
                             << "5. View Dispatch Notifications\n"
                             << "6. Logout\n"
                             << "Choose an option: ";
                        int sellerChoice;
                        cin >> sellerChoice;
                        cin.ignore();

                        if (sellerChoice == 1) {
                            string productName;
                            double price;
                            int quantity;

                            cout << "Enter product name: ";
                            getline(cin, productName);
                            cout << "Enter price: ";
                            cin >> price;
                            cout << "Enter quantity: ";
                            cin >> quantity;
                            cin.ignore();

                            seller->addProduct(productName, price, quantity);
                        } else if (sellerChoice == 2) {
                            string productName;
                            double newPrice;
                            int newQuantity;

                            cout << "Enter product name to edit: ";
                            getline(cin, productName);

                            // Check if the product exists
                            ifstream file(PRODUCTS_FILE);
                            string line;
                            bool productExists = false;

                            while (getline(file, line)) {
                                stringstream ss(line);
                                string seller, product, price, quantity;
                                getline(ss, seller, ',');
                                getline(ss, product, ',');

                                if (seller == username && product == productName) {
                                    productExists = true;
                                    break;
                                }
                            }
                            file.close();

                            if (productExists) {
                                cout << "Enter new price: ";
                                cin >> newPrice;
                                cout << "Enter new quantity: ";
                                cin >> newQuantity;
                                cin.ignore();

                                seller->editProduct(productName, newPrice, newQuantity);
                            } else {
                                cout << "Oops!!! No such product exists.\n";
                            }
                        }

                        else if (sellerChoice == 3) {
                            string productName;

                            cout << "Enter product name to delete: ";
                            getline(cin, productName);

                            seller->deleteProduct(productName);
                        } else if (sellerChoice == 4) {
                            seller->viewListedProducts();
                        } else if (sellerChoice == 5) {
                            seller->viewDispatchNotifications();
                        } else if (sellerChoice == 6) {
                            cout << "Logging out...\n";
                            break;
                        } else {
                            cout << "Invalid choice! Try again.\n";
                        }
                    }

                } else if (Admin* admin = dynamic_cast<Admin*>(user)) {
                    while (true) {
                        cout << "\n1. View All Users\n"
                             << "2. View All Transactions\n"
                             << "3. Logout\n"
                             << "Choose an option: ";
                        int adminChoice;
                        cin >> adminChoice;
                        cin.ignore();

                        if (adminChoice == 1) {
                            admin->viewAllUsers();
                        } else if (adminChoice == 2) {
                            admin->viewAllTransactions();
                        } else if (adminChoice == 3) {
                            cout << "Logging out...\n";
                            break;
                        } else {
                            cout << "Invalid choice! Try again.\n";
                        }
                    }
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
