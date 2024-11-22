#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <bits/stdc++.h>
#include <chrono>
#include <cctype>
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define BOLD "\033[1m"
#define CYAN "\033[36m"
using namespace std;

// File paths for data storage
const string USERS_FILE = "users.txt";
const string PRODUCTS_FILE = "products.txt";
const string ORDERS_FILE = "orders.txt";
const string TRANSACTIONS_FILE = "transactions.txt";
const string COUPONS_FILE = "coupons.txt";
const string NOTIFICATIONS_FILE = "notifications.txt";

// Utility class for handling file operations
class FileHandler
{
public:
    static bool doesUserExist(const string &username)
    {
        ifstream file(USERS_FILE);
        string line;

        while (getline(file, line))
        {
            stringstream ss(line);
            string user;
            getline(ss, user, ',');
            if (user == username)
            {
                return true;
            }
        }
        return false;
    }

    static void saveToFile(const string &filename, const string &data)
    {
        ofstream file(filename, ios::app);
        if (file.is_open())
        {
            file << data << endl;
        }
        file.close();
    }
};

// Base User class
class User
{
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

    virtual void saveToFile()
    {
        string userData = username + "," + password + "," + email + "," + userType + "," + fullName + "," + phoneNumber;
        FileHandler::saveToFile(USERS_FILE, userData);
    }

    static User *login(const string &username, const string &password);
};

// Buyer

class Buyer : public User
{
public:
    Buyer(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "buyer") {}

    // View all available products
    void viewAvailableProducts()
    {
        ifstream file(PRODUCTS_FILE);
        if (!file.is_open())
        {
            cout << BOLD << RED << "Error: Unable to open products file.\n"
                 << RESET;
            return;
        }

        string line;
        cout << "\n"
             << BOLD << CYAN << "Available Products:\n"
             << RESET;
        bool found = false;
        while (getline(file, line))
        {
            stringstream ss(line);
            string seller, product, price, stock, category;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, stock, ',');
            getline(ss, category, ',');

            if (stoi(stock) > 0)
            { // Only display products with stock
                cout << BOLD << GREEN << "Product: " << RESET << product
                     << " | " << BOLD << GREEN << "Price: " << RESET << price
                     << " | " << BOLD << GREEN << "Quantity: " << RESET << stock
                     << " | " << BOLD << GREEN << "Category: " << RESET << category
                     << " | " << BOLD << GREEN << "Sold by: " << RESET << seller << "\n";
                found = true;
            }
        }
        file.close();

        if (!found)
        {
            cout << BOLD << YELLOW << "No products available at the moment.\n"
                 << RESET;
        }
    }

    // View products filtered by category
    void viewProductsByCategory(const string &category)
    {
        ifstream file(PRODUCTS_FILE);
        if (!file.is_open())
        {
            cout << BOLD << RED << "Error: Unable to open products file.\n"
                 << RESET;
            return;
        }

        string line;
        bool found = false;
        cout << "\n"
             << BOLD << CYAN << "Products in Category: " << RESET << category << "\n";

        while (getline(file, line))
        {
            stringstream ss(line);
            string seller, product, price, stock, categoryFromFile;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, stock, ',');
            getline(ss, categoryFromFile, ',');

            if (categoryFromFile == category && stoi(stock) > 0)
            {
                cout << BOLD << GREEN << "Product: " << RESET << product
                     << " | " << BOLD << GREEN << "Price: " << RESET << price
                     << " | " << BOLD << GREEN << "Quantity: " << RESET << stock
                     << " | " << BOLD << GREEN << "Sold by: " << RESET << seller << "\n";
                found = true;
            }
        }
        file.close();

        if (!found)
        {
            cout << BOLD << YELLOW << "No products found in this category.\n"
                 << RESET;
        }
    }

    void waitForSeconds(int seconds)
    {
        time_t startTime = time(0);
        while (difftime(time(0), startTime) < seconds)
        {
            // Busy-waiting loop to simulate delay
        }
    }
    void displayPaymentOptions()
    {
        cout << "\n"
             << BOLD << CYAN << "Choose a payment method:\n"
             << RESET;
        cout << BOLD << GREEN << "1. Card Payment\n"
             << RESET;
        cout << BOLD << GREEN << "2. UPI\n"
             << RESET;
        cout << BOLD << CYAN << "Enter your choice: " << RESET;
    }

    void processCardPayment()
    {
        string cardNumber, cardHolderName;
        cout << "\n"
             << BOLD << CYAN << "Enter Card Number: " << RESET;
        cin >> cardNumber;
        cout << BOLD << CYAN << "Enter Card Holder Name: " << RESET;

        cin.ignore();
        getline(cin, cardHolderName);

        cout << BOLD << GREEN << "Your payment will be redirected in 10 seconds...\n"
             << RESET;
        waitForSeconds(5);

        cout << BOLD << GREEN << "Processing payment...\n"
             << RESET;
        waitForSeconds(5); // Simulate a delay for payment processing
        cout << BOLD << GREEN << "Payment successful!\n"
             << RESET;
    }

    void processUPIPayment()
    {
        string upiID;
        cout << "\n"
             << BOLD << CYAN << "Enter UPI ID: " << RESET;
        cin >> upiID;

        cout << BOLD << GREEN << "Your payment will be redirected in 10 seconds...\n"
             << RESET;
        waitForSeconds(5);

        cout << BOLD << GREEN << "Processing payment...\n"
             << RESET;
        waitForSeconds(5); // Simulate a delay for payment processing
        cout << BOLD << GREEN << "Payment successful!\n"
             << RESET;
    }

    bool applyCoupon(const string &enteredCoupon, float &discountRate)
    {
        ifstream file(COUPONS_FILE);
        string coupon;

        // Check if the file was opened successfully
        if (!file)
        {
            cout << BOLD << RED << "Error opening coupon file!" << RESET << endl;
            return false;
        }

        // Read each line (coupon) from the file
        while (getline(file, coupon))
        {
            // Compare the entered coupon directly with the coupon from the file
            if (coupon == enteredCoupon)
            {
                // Apply a random discount between 15% and 20%
                discountRate = (rand() % 6 + 15) / 100.0; // Random discount between 15% and 20%
                cout << BOLD << GREEN << "Coupon matched! Discount applied: " << RESET << discountRate * 100 << "%" << endl;
                return true; // Coupon found, discount applied
            }
        }

        // If no coupon matched
        cout << BOLD << YELLOW << "Coupon not found!\n"
             << RESET;
        return false; // Coupon not found
    }

    void generateBill(const string &productName, int quantity, float pricePerUnit, float discountRate)
    {
        float totalAmount = pricePerUnit * quantity;
        float discountAmount = totalAmount * discountRate;
        float subTotal = totalAmount - discountAmount;
        float cgst = subTotal * 0.09; // 9% CGST
        float sgst = subTotal * 0.09; // 9% SGST
        float finalAmount = subTotal + cgst + sgst;

        cout << fixed << setprecision(2);
        cout << "\n--- BILL ---\n";
        cout << BOLD << CYAN << "Product Name: " << RESET << productName << "\n";
        cout << BOLD << CYAN << "Quantity: " << RESET << quantity << "\n";
        cout << BOLD << CYAN << "Price per Unit: " << RESET << pricePerUnit << "\n";
        cout << BOLD << CYAN << "Total Amount: " << RESET << totalAmount << "\n";
        cout << BOLD << CYAN << "Discount Applied: " << RESET << discountAmount << " (" << discountRate * 100 << "%)\n";
        cout << BOLD << CYAN << "Subtotal after Discount: " << RESET << subTotal << "\n";
        cout << BOLD << CYAN << "CGST (9%): " << RESET << cgst << "\n";
        cout << BOLD << CYAN << "SGST (9%): " << RESET << sgst << "\n";
        cout << BOLD << CYAN << "Final Amount to Pay: " << RESET << finalAmount << "\n";
        cout << BOLD << CYAN << "-------------------\n"
             << RESET;

        // Save transaction details to file
        ofstream transactionFile(TRANSACTIONS_FILE, ios::app);
        transactionFile << "Customer," << productName << "," << quantity << "," << finalAmount << "," << time(0) << endl;
        transactionFile.close();
    }

    void placeOrder(const string &username, const string &productName, int quantity)
    {
        ifstream file(PRODUCTS_FILE);
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        // Ask for the delivery address
        string deliveryAddress;
        cout << "\n"
             << BOLD << CYAN << "Enter your delivery address: " << RESET;
        getline(cin, deliveryAddress);

        string sellerName; // Store the seller name for the order
        float productPrice = 0.0;

        while (getline(file, line))
        {
            stringstream ss(line);
            string seller, product, price, stock, category;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, stock, ',');
            getline(ss, category, ',');

            if (product == productName && stoi(stock) >= quantity)
            {
                found = true;
                sellerName = seller; // Store the seller's name
                productPrice = stof(price);
                int remainingStock = stoi(stock) - quantity;
                tempFile << seller << "," << product << "," << price << "," << remainingStock << "," << category << endl;

                // Ask for coupon code first
                float discountRate = 0.0;
                cout << "\n"
                     << BOLD << CYAN << "Enter coupon code (or press Enter to skip): " << RESET;
                string enteredCoupon;
                getline(cin, enteredCoupon);

                if (!enteredCoupon.empty())
                {
                    if (applyCoupon(enteredCoupon, discountRate))
                    {
                        cout << BOLD << GREEN << "Coupon applied successfully. Discount of " << discountRate * 100 << "% will be applied.\n"
                             << RESET;
                    }
                    else
                    {
                        cout << BOLD << YELLOW << "Invalid coupon code. No discount will be applied.\n"
                             << RESET;
                    }
                }

                // Display Bill with applied discount
                generateBill(productName, quantity, stof(price), discountRate);

                // Ask for payment method after showing the bill
                displayPaymentOptions();
                int paymentChoice;
                cin >> paymentChoice;

                if (paymentChoice == 1)
                {
                    processCardPayment();
                }
                else if (paymentChoice == 2)
                {
                    processUPIPayment();
                }
                else
                {
                    cout << BOLD << RED << "Invalid payment choice. Transaction cancelled.\n"
                         << RESET;
                    return;
                }

                // Save the order along with the delivery address
                string orderData = sellerName + "," + username + "," + product + "," + to_string(quantity) + "," + to_string(time(0)) + "," + deliveryAddress;
                ofstream orderFile(ORDERS_FILE, ios::app);
                orderFile << orderData << endl;
                orderFile.close();

                cout << BOLD << GREEN << "Order placed successfully for " << quantity << " unit(s) of \"" << productName << "\".\n"
                     << RESET;
                cout << BOLD << CYAN << "Delivery Address: " << RESET << deliveryAddress << endl;
            }
            else
            {
                tempFile << line << "\n";
            }
        }

        file.close();
        tempFile.close();

        // Replace old product file with updated one
        remove(PRODUCTS_FILE.c_str());
        rename("temp.txt", PRODUCTS_FILE.c_str());

        if (!found)
        {
            cout << BOLD << RED << "Product not found or insufficient stock.\n"
                 << RESET;
        }
    }

    void checkOrderStatus()
    {
        ifstream file(NOTIFICATIONS_FILE);
        if (!file.is_open())
        {
            cout << BOLD << RED << "Error: Unable to open notifications file.\n"
                 << RESET;
            return;
        }

        string line;
        bool hasNotifications = false;

        cout << "\n"
             << BOLD << CYAN << "=== Order Status ===\n"
             << RESET;

        while (getline(file, line))
        {
            stringstream ss(line);
            string buyer, product, status, timestamp;
            getline(ss, buyer, ',');
            getline(ss, product, ',');
            getline(ss, status, ',');
            getline(ss, timestamp, ',');

            if (buyer == username)
            { // Match the logged-in buyer
                hasNotifications = true;

                // Convert timestamp to a readable format
                try
                {
                    time_t notificationTime = stoul(timestamp);
                    cout << BOLD << GREEN << "Product: " << RESET << product
                         << " | " << BOLD << GREEN << "Status: " << RESET << status
                         << " | " << BOLD << GREEN << "Updated At: " << RESET << ctime(&notificationTime); // Format as human-readable date
                }
                catch (const exception &e)
                {
                    cout << BOLD << RED << "Error processing notification timestamp: " << RESET << e.what() << "\n";
                }
            }
        }

        file.close();

        if (!hasNotifications)
        {
            cout << BOLD << YELLOW << "No updates on your orders.\n"
                 << RESET;
        }
    }

    void checkMyOrders()
    {
        cout << "\n"
             << BOLD << CYAN << "=== My Orders Status ===\n"
             << RESET;

        // Use the checkOrderStatus function
        checkOrderStatus(); // Shows order updates for the logged-in buyer

        cout << "\n"
             << BOLD << CYAN << "Press Enter to continue..." << RESET;
        cin.ignore();
        cin.get();
    }
};

// Seller

class Seller : public User
{
public:
    Seller(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "seller") {}

    void addProduct(const string &productName, double price, int quantity, const string &category)
    {
        string productData = username + "," + productName + "," + to_string(price) + "," + to_string(quantity) + "," + category;
        FileHandler::saveToFile(PRODUCTS_FILE, productData);
        cout << BOLD << GREEN << "Product \"" << productName << "\" added successfully in category \"" << category << "\".\n"
             << RESET;
    }

    void editProduct(const string &productName, double newPrice, int newQuantity, const string &newCategory)
    {
        ifstream file(PRODUCTS_FILE);
        string line;
        bool productExists = false;

        // Step 1: Check if the product exists for this seller
        while (getline(file, line))
        {
            stringstream ss(line);
            string seller, product, price, quantity;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');

            if (seller == username && product == productName)
            {
                productExists = true;
                break;
            }
        }
        file.close();

        // Step 2: If product exists, proceed with updating it
        if (productExists)
        {
            ifstream file(PRODUCTS_FILE);
            ofstream tempFile("temp.txt");
            bool updated = false;

            while (getline(file, line))
            {
                stringstream ss(line);
                string seller, product, price, quantity, category;
                getline(ss, seller, ',');
                getline(ss, product, ',');
                getline(ss, price, ',');
                getline(ss, quantity, ',');
                getline(ss, category, ',');

                if (seller == username && product == productName)
                {
                    tempFile << seller << "," << product << "," << newPrice << "," << newQuantity << "," << newCategory << endl;
                    updated = true;
                }
                else
                {
                    tempFile << line << endl;
                }
            }

            file.close();
            tempFile.close();

            remove(PRODUCTS_FILE.c_str());
            rename("temp.txt", PRODUCTS_FILE.c_str());

            if (updated)
            {
                cout << BOLD << GREEN << "Product \"" << productName << "\" updated successfully.\n"
                     << RESET;
            }
            else
            {
                cout << BOLD << RED << "Product \"" << productName << "\" not found for the seller \"" << username << "\".\n"
                     << RESET;
            }
        }
        else
        {
            cout << BOLD << RED << "Product \"" << productName << "\" does not exist.\n"
                 << RESET;
        }
    }

    void deleteProduct(const string &productName)
    {
        ifstream file(PRODUCTS_FILE);
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        while (getline(file, line))
        {
            stringstream ss(line);
            string seller, product, price, quantity, category;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');
            getline(ss, category, ',');

            if (seller == username && product == productName)
            {
                found = true;
            }
            else
            {
                tempFile << line << endl;
            }
        }
        file.close();
        tempFile.close();

        remove(PRODUCTS_FILE.c_str());
        rename("temp.txt", PRODUCTS_FILE.c_str());

        if (found)
        {
            cout << BOLD << GREEN << "Product \"" << productName << "\" deleted successfully.\n"
                 << RESET;
        }
        else
        {
            cout << BOLD << RED << "Product not found!\n"
                 << RESET;
        }
    }

    void viewDispatchNotifications()
    {
        ifstream file(ORDERS_FILE);
        if (!file.is_open())
        {
            cout << BOLD << RED << "Error: Unable to open orders file.\n"
                 << RESET;
            return;
        }

        ofstream tempFile("temp.txt");
        if (!tempFile.is_open())
        {
            cout << BOLD << RED << "Error: Unable to create temporary file.\n"
                 << RESET;
            return;
        }

        string line;
        bool hasNotifications = false;

        cout << BOLD << CYAN << "\n=== Dispatch Notifications ===\n"
             << RESET;

        vector<string> remainingOrders;

        while (getline(file, line))
        {
            stringstream ss(line);
            string seller, buyer, product, quantity, timestamp, address;
            getline(ss, seller, ',');
            getline(ss, buyer, ',');
            getline(ss, product, ',');
            getline(ss, quantity, ',');
            getline(ss, timestamp, ',');
            getline(ss, address, ',');

            if (seller == username)
            {
                hasNotifications = true;
                time_t orderTime = stoul(timestamp);

                cout << BOLD << "Order Details:\n"
                     << RESET;
                cout << "Buyer: " << buyer << "\n";
                cout << "Product: " << product << "\n";
                cout << "Quantity: " << quantity << "\n";
                cout << "Delivery Address: " << address << "\n";
                cout << "Ordered At: " << ctime(&orderTime);
                cout << BOLD << "Do you want to dispatch this order? (y/n): " << RESET;

                char choice;
                cin >> choice;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << BOLD << RED << "Invalid input! Please enter a valid number.\n"
                         << RESET;
                    continue;
                }
                cin.ignore();

                if (choice == 'y' || choice == 'Y')
                {
                    // Record the transaction
                    string transactionData = seller + "," + buyer + "," + product + "," + quantity + "," + to_string(time(0));
                    FileHandler::saveToFile(TRANSACTIONS_FILE, transactionData);
                    cout << BOLD << GREEN << "Order dispatched and transaction recorded.\n"
                         << RESET;

                    // Notify the buyer
                    string notification = buyer + "," + product + ",Dispatched," + to_string(time(0));
                    FileHandler::saveToFile(NOTIFICATIONS_FILE, notification);
                }
                else
                {
                    // Notify the buyer that the order is pending
                    string notification = buyer + "," + product + ",Pending," + to_string(time(0));
                    FileHandler::saveToFile(NOTIFICATIONS_FILE, notification);

                    // Keep the order in the remaining orders
                    remainingOrders.push_back(line);
                }
            }
            else
            {
                remainingOrders.push_back(line);
            }
        }

        file.close();

        // Rewrite the orders file with remaining orders
        ofstream outFile(ORDERS_FILE);
        for (const string &order : remainingOrders)
        {
            outFile << order << endl;
        }
        outFile.close();

        if (!hasNotifications)
        {
            cout << BOLD << YELLOW << "No pending orders for dispatch.\n"
                 << RESET;
        }
    }

    void viewListedProducts()
    {
        ifstream file(PRODUCTS_FILE);
        string line;
        cout << BOLD << CYAN << "\nYour Listed Products:\n"
             << RESET;
        while (getline(file, line))
        {
            stringstream ss(line);
            string seller, product, price, quantity, category;
            getline(ss, seller, ',');
            getline(ss, product, ',');
            getline(ss, price, ',');
            getline(ss, quantity, ',');
            getline(ss, category, ',');

            if (seller == username)
            {
                cout << BOLD << "Product: " << RESET << product
                     << " | " << BOLD << "Price: " << RESET << price
                     << " | " << BOLD << "Quantity: " << RESET << quantity
                     << " | " << BOLD << "Category: " << RESET << category << "\n";
            }
        }
        file.close();
    }
};

// Admin

class Admin : public User
{
public:
    Admin(string uname, string pwd, string mail, string name, string phone)
        : User(uname, pwd, mail, name, phone, "admin") {}

    void viewAllUsers()
    {
        ifstream file(USERS_FILE);
        string line;

        if (!file.is_open())
        {
            cout << RED << "Error opening file." << RESET << endl;
            return;
        }

        cout << BOLD << CYAN << "\nAll Users:\n"
             << RESET;
        cout << BOLD << "Username" << RESET << " | " << BOLD << "Full Name" << RESET
             << " | " << BOLD << "Role" << RESET << " | " << BOLD << "Email" << RESET
             << " | " << BOLD << "Phone No" << RESET << " | " << BOLD << "Encrypted Password" << RESET << endl;
        cout << "--------------------------------------------------------\n";

        while (getline(file, line))
        {
            stringstream ss(line);
            string username, role, email, phone, password, encryptedPassword, fullname;

            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, email, ',');
            getline(ss, role, ',');
            getline(ss, fullname, ',');
            getline(ss, phone, ',');

            encryptedPassword = string(password.length(), '*');

            cout << username << " | " << fullname << " | " << role << " | " << email
                 << " | " << phone << " | " << encryptedPassword << endl;
        }

        file.close();
    }

    void viewAllTransactions()
    {
        ifstream file(TRANSACTIONS_FILE);
        string line;
        bool hasTransactions = false;

        if (!file.is_open())
        {
            cout << RED << "Error: Unable to open transactions file." << RESET << endl;
            return;
        }

        cout << BOLD << CYAN << "\n=== All Transactions ===" << RESET << endl;
        cout << "----------------------------------------\n";

        while (getline(file, line))
        {
            stringstream ss(line);
            string seller, buyer, product, quantity, timestamp;

            try
            {
                if (!getline(ss, seller, ',') || seller.empty())
                    throw runtime_error("Invalid seller");
                if (!getline(ss, buyer, ',') || buyer.empty())
                    throw runtime_error("Invalid buyer");
                if (!getline(ss, product, ',') || product.empty())
                    throw runtime_error("Invalid product");
                if (!getline(ss, quantity, ',') || quantity.empty())
                    throw runtime_error("Invalid quantity");
                if (!getline(ss, timestamp, ',') || timestamp.empty())
                    throw runtime_error("Invalid timestamp");

                try
                {
                    time_t transactionTime = stoul(timestamp);
                    hasTransactions = true;

                    cout << BOLD << GREEN << "User-persona: " << seller << "\n"
                         << BOLD << GREEN << "Prodcut name/Buyer: " << buyer << "\n"
                         << BOLD << GREEN << "Quantity/Product: " << product << "\n"
                         << BOLD << GREEN << "Price/Quantity: " << quantity << "\n"
                         << BOLD << GREEN << "Transaction At: " << ctime(&transactionTime)
                         << "----------------------------------------\n";
                }
                catch (const exception &e)
                {
                    cout << YELLOW << "Warning: Invalid timestamp format for transaction between " << seller << " and " << buyer << RESET << "\n";
                    continue;
                }
            }
            catch (const exception &e)
            {
                cout << RED << "Error processing transaction: " << e.what() << RESET << "\n";
                continue;
            }
        }

        file.close();

        if (!hasTransactions)
        {
            cout << RED << "No transactions recorded yet.\n";
        }
    }
};

// Static method implementation for User login
User *User::login(const string &username, const string &password)
{
    ifstream file(USERS_FILE);
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string uname, pwd, email, type, name, phone;

        getline(ss, uname, ',');
        getline(ss, pwd, ',');
        getline(ss, email, ',');
        getline(ss, type, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');

        if (uname == username && pwd == password)
        {
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
bool isValidEmail(const string &email)
{
    return email.find('@') != string::npos && email.find('.') != string::npos;
}

bool isValidPassword(const string &password)
{
    return password.length() >= 6;
}

bool isValidPhone(const string &phone)
{
    return phone.length() == 10 && phone.find_first_not_of("0123456789") == string::npos;
}

int main()
{
    while (true)
    {
        cout << BLUE << "\n========================================\n"
             << RESET;
        cout << BOLD << BLUE << "           E-COMMERCE SYSTEM           \n"
             << RESET;
        cout << BLUE << "========================================\n"
             << RESET;
        cout << BOLD << YELLOW << setw(4) << "1. " << "Sign Up\n"
             << RESET;
        cout << BOLD << YELLOW << setw(4) << "2. " << "Login\n"
             << RESET;
        cout << BOLD << YELLOW << setw(4) << "3. " << "Exit\n"
             << RESET;
        cout << BLUE << "----------------------------------------\n"
             << RESET;
        cout << YELLOW << "Choose an option: ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {                                                        // Check for invalid input
            cin.clear();                                         // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Flush the input buffer
            cout << RED << "Invalid input! Please enter a valid number.\n"
                 << RESET;
            continue;
        }

        cin.ignore();

        if (choice == 1)
        {
            cout << BLUE << "\n========================================\n"
                 << RESET;
            cout << BLUE << "                 SIGN UP                \n"
                 << RESET;
            cout << BLUE << "========================================\n"
                 << RESET;
            cout << BOLD << YELLOW << setw(4) << "1. " << "Buyer\n"
                 << RESET;
            cout << BOLD << YELLOW << setw(4) << "2. " << "Seller\n"
                 << RESET;
            cout << BOLD << YELLOW << setw(4) << "3. " << "Admin\n"
                 << RESET;
            cout << BLUE << "----------------------------------------\n"
                 << RESET;
            cout << YELLOW << "Choose user type: " << RESET;

            int userType;
            cin >> userType;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << BOLD << RED << "Invalid input! Please enter a valid number.\n"
                     << RESET;
                continue;
            }

            cin.ignore();

            string username, password, email, fullName, phoneNumber;

            cout << BOLD << YELLOW << "Enter username: " << RESET;
            getline(cin, username);
            if (FileHandler::doesUserExist(username))
            {
                cout << BOLD << RED << "Username already exists!\n"
                     << RESET;
                continue;
            }

            cout << BOLD << YELLOW << "Enter password (min 6 characters): " << RESET;
            getline(cin, password);
            if (!isValidPassword(password))
            {
                cout << BOLD << RED << "Invalid password!\n"
                     << RESET;
                continue;
            }

            cout << BOLD << YELLOW << "Enter email: " << RESET;
            getline(cin, email);
            if (!isValidEmail(email))
            {
                cout << BOLD << RED << "Invalid email format!\n"
                     << RESET;
                continue;
            }

            cout << BOLD << YELLOW << "Enter full name: " << RESET;
            getline(cin, fullName);

            cout << BOLD << YELLOW << "Enter phone number (10 digits): " << RESET;
            getline(cin, phoneNumber);
            if (!isValidPhone(phoneNumber))
            {
                cout << BOLD << RED << "Invalid phone number!\n"
                     << RESET;
                continue;
            }

            User *newUser = nullptr;
            switch (userType)
            {
            case 1:
                newUser = new Buyer(username, password, email, fullName, phoneNumber);
                break;
            case 2:
                newUser = new Seller(username, password, email, fullName, phoneNumber);
                break;
            case 3:
                newUser = new Admin(username, password, email, fullName, phoneNumber);
                break;
            default:
                cout << BOLD << RED << "Invalid user type!\n"
                     << RESET;
                continue;
            }

            newUser->saveToFile();
            cout << BOLD << GREEN << "Registration successful!\n"
                 << RESET;
            delete newUser;
        }
        else if (choice == 2)
        {
            cout << BOLD << CYAN << "\n=== Login ===\n"
                 << RESET;
            string username, password;

            cout << BOLD << YELLOW << "Enter username: " << RESET;
            getline(cin, username);
            cout << BOLD << YELLOW << "Enter password: " << RESET;
            getline(cin, password);

            User *user = User::login(username, password);
            if (user)
            {
                cout << BOLD << GREEN << "Login successful!\n"
                     << RESET;

                if (Buyer *buyer = dynamic_cast<Buyer *>(user))
                {
                    while (true)
                    {
                        cout << BOLD << CYAN << "\n=== Buyer Menu ===\n"
                             << RESET;
                        cout << BOLD << YELLOW << "1. View All Products\n"
                             << "2. Place Order\n"
                             << "3. View Products by Category\n"
                             << "4. My Orders\n"
                             << "5. Logout\n"
                             << "Choose an option: " << RESET;

                        int buyerChoice;
                        cin >> buyerChoice;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << BOLD << RED << "Invalid input! Please enter a valid number.\n"
                                 << RESET;
                            continue;
                        }
                        cin.ignore();

                        if (buyerChoice == 1)
                        {
                            buyer->viewAvailableProducts();
                        }
                        else if (buyerChoice == 2)
                        {
                            string productName;
                            int quantity;

                            cout << BOLD << YELLOW << "Enter product name: " << RESET;
                            getline(cin, productName);
                            cout << BOLD << YELLOW << "Enter quantity: " << RESET;
                            cin >> quantity;
                            if (cin.fail())
                            {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << BOLD << RED << "Invalid input! Please enter a valid number.\n"
                                     << RESET;
                                continue;
                            }
                            cin.ignore();

                            buyer->placeOrder(username, productName, quantity);
                        }
                        else if (buyerChoice == 3)
                        {
                            cout << BOLD << CYAN << "\nSelect a category:\n"
                                 << RESET
                                 << BOLD << YELLOW << "1. Electronics\n"
                                 << "2. Clothing\n"
                                 << "3. Home\n"
                                 << "4. Books\n"
                                 << "Choose a category: " << RESET;

                            int categoryChoice;
                            cin >> categoryChoice;
                            if (cin.fail())
                            {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << BOLD << RED << "Invalid input! Please enter a valid number.\n"
                                     << RESET;
                                continue;
                            }
                            cin.ignore();

                            string selectedCategory;
                            switch (categoryChoice)
                            {
                            case 1:
                                selectedCategory = "Electronics";
                                break;
                            case 2:
                                selectedCategory = "Clothing";
                                break;
                            case 3:
                                selectedCategory = "Home";
                                break;
                            case 4:
                                selectedCategory = "Books";
                                break;
                            default:
                                cout << BOLD << RED << "Invalid category!\n"
                                     << RESET;
                                continue;
                            }

                            buyer->viewProductsByCategory(selectedCategory);
                        }

                        else if (buyerChoice == 4)
                        {
                            buyer->checkMyOrders();
                        }

                        else if (buyerChoice == 5)
                        {
                            cout << BOLD << GREEN << "Logging out...\n"
                                 << RESET;
                            break;
                        }
                        else
                        {
                            cout << BOLD << RED << "Invalid choice! Try again.\n"
                                 << RESET;
                        }
                    }
                }
                else if (Seller *seller = dynamic_cast<Seller *>(user))
                {
                    while (true)
                    {
                        cout << BOLD << CYAN << "\n=== Seller Menu ===\n"
                             << RESET;
                        cout << BOLD << YELLOW << "1. Add Product\n"
                             << "2. Edit Product\n"
                             << "3. Delete Product\n"
                             << "4. View Listed Products\n"
                             << "5. View Dispatch Notifications\n"
                             << "6. Logout\n"
                             << "Choose an option: " << RESET;

                        int sellerChoice;
                        cin >> sellerChoice;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << BOLD << RED << "Invalid input! Please enter a valid number.\n"
                                 << RESET;
                            continue;
                        }
                        cin.ignore();

                        if (sellerChoice == 1)
                        {
                            string productName;
                            double price;
                            int quantity;
                            string category;

                            cout << BOLD << YELLOW << "Enter product name: " << RESET;
                            getline(cin, productName);
                            cout << BOLD << YELLOW << "Enter price: " << RESET;
                            cin >> price;
                            cout << BOLD << YELLOW << "Enter quantity: " << RESET;
                            cin >> quantity;
                            cin.ignore();

                            cout << BOLD << YELLOW << "Select category (1. Electronics, 2. Clothing, 3. Home, 4. Books): " << RESET;
                            int categoryChoice;
                            cin >> categoryChoice;
                            cin.ignore();

                            switch (categoryChoice)
                            {
                            case 1:
                                category = "Electronics";
                                break;
                            case 2:
                                category = "Clothing";
                                break;
                            case 3:
                                category = "Home";
                                break;
                            case 4:
                                category = "Books";
                                break;
                            default:
                                category = "Others";
                                break;
                            }

                            seller->addProduct(productName, price, quantity, category);
                        }
                        else if (sellerChoice == 2)
                        {
                            string productName;
                            double newPrice;
                            int newQuantity;
                            string newCategory;

                            cout << BOLD << YELLOW << "Enter product name to edit: " << RESET;
                            getline(cin, productName);

                            cout << BOLD << YELLOW << "Enter new price: " << RESET;
                            cin >> newPrice;
                            cout << BOLD << YELLOW << "Enter new quantity: " << RESET;
                            cin >> newQuantity;
                            cin.ignore();
                            cout << BOLD << YELLOW << "Enter new category: " << RESET;
                            getline(cin, newCategory);

                            seller->editProduct(productName, newPrice, newQuantity, newCategory);
                        }
                        else if (sellerChoice == 3)
                        {
                            string productName;

                            cout << BOLD << YELLOW << "Enter product name to delete: " << RESET;
                            getline(cin, productName);

                            seller->deleteProduct(productName);
                        }
                        else if (sellerChoice == 4)
                        {
                            seller->viewListedProducts();
                        }
                        else if (sellerChoice == 5)
                        {
                            seller->viewDispatchNotifications();
                        }
                        else if (sellerChoice == 6)
                        {
                            cout << BOLD << GREEN << "Logging out...\n"
                                 << RESET;
                            break;
                        }
                        else
                        {
                            cout << BOLD << RED << "Invalid choice! Try again.\n"
                                 << RESET;
                        }
                    }
                }
                else if (Admin *admin = dynamic_cast<Admin *>(user))
                {
                    while (true)
                    {
                        cout << BOLD << CYAN << "\n=== Admin Menu ===\n"
                             << RESET;
                        cout << BOLD << YELLOW << "1. View All Users\n"
                             << "2. View All Transactions\n"
                             << "3. Logout\n"
                             << "Choose an option: " << RESET;

                        int adminChoice;
                        cin >> adminChoice;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << BOLD << RED << "Invalid input! Please enter a valid number.\n"
                                 << RESET;
                            continue;
                        }
                        cin.ignore();

                        if (adminChoice == 1)
                        {
                            admin->viewAllUsers();
                        }
                        else if (adminChoice == 2)
                        {
                            admin->viewAllTransactions();
                        }
                        else if (adminChoice == 3)
                        {
                            cout << BOLD << GREEN << "Logging out...\n"
                                 << RESET;
                            break;
                        }
                        else
                        {
                            cout << BOLD << RED << "Invalid choice! Try again.\n"
                                 << RESET;
                        }
                    }
                }
            }
            else
            {
                cout << BOLD << RED << "Invalid username or password!\n"
                     << RESET;
            }
        }
        else if (choice == 3)
        {
            cout << BOLD << CYAN << "Exiting the program...\n"
                 << RESET;
            break;
        }
        else
        {
            cout << BOLD << RED << "Invalid choice! Try again.\n"
                 << RESET;
        }
    }

    return 0;
}
