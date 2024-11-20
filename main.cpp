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
            cout << "Error: Unable to open products file.\n";
            return;
        }

        string line;
        cout << "\nAvailable Products:\n";
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
                cout << "Product: " << product
                     << " | Price: " << price
                     << " | Quantity: " << stock
                     << " | Category: " << category
                     << " | Sold by: " << seller << "\n";
                found = true;
            }
        }
        file.close();

        if (!found)
        {
            cout << "No products available at the moment.\n";
        }
    }

    // View products filtered by category
    void viewProductsByCategory(const string &category)
    {
        ifstream file(PRODUCTS_FILE);
        if (!file.is_open())
        {
            cout << "Error: Unable to open products file.\n";
            return;
        }

        string line;
        bool found = false;
        cout << "\nProducts in Category: " << category << "\n";

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
                cout << "Product: " << product
                     << " | Price: " << price
                     << " | Quantity: " << stock
                     << " | Sold by: " << seller << "\n";
                found = true;
            }
        }
        file.close();

        if (!found)
        {
            cout << "No products found in this category.\n";
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
        cout << "\nChoose a payment method:\n";
        cout << "1. Card Payment\n";
        cout << "2. UPI\n";
        cout << "Enter your choice: ";
    }

    void processCardPayment()
    {
        string cardNumber, cardHolderName;
        cout << "\nEnter Card Number: ";
        cin >> cardNumber;
        cout << "Enter Card Holder Name: ";

        cin.ignore();
        getline(cin, cardHolderName);

        cout << "Your payment will be redirected in 10 seconds...\n";
        waitForSeconds(5);

        cout << "Processing payment...\n";
        waitForSeconds(5); // Simulate a delay for payment processing
        cout << "Payment successful!\n";
    }

    void processUPIPayment()
    {
        string upiID;
        cout << "\nEnter UPI ID: ";
        cin >> upiID;

        cout << "Your payment will be redirected in 10 seconds...\n";
        waitForSeconds(5);

        cout << "Processing payment...\n";
        waitForSeconds(5); // Simulate a delay for payment processing
        cout << "Payment successful!\n";
    }

    bool applyCoupon(const string &enteredCoupon, float &discountRate)
    {
        ifstream file(COUPONS_FILE);
        string coupon;

        // Check if the file was opened successfully
        if (!file)
        {
            cout << "Error opening coupon file!" << endl;
            return false;
        }

        // Read each line (coupon) from the file
        while (getline(file, coupon))
        {
            // Debugging statement: Print the coupon being read from the file and the entered coupon
            cout << "Checking coupon: '" << coupon << "' against entered: '" << enteredCoupon << "'\n";

            // Compare the entered coupon directly with the coupon from the file
            if (coupon == enteredCoupon)
            {
                // Apply a random discount between 15% and 20%
                discountRate = (rand() % 6 + 15) / 100.0; // Random discount between 15% and 20%
                cout << "Coupon matched! Discount applied: " << discountRate * 100 << "%\n";
                return true; // Coupon found, discount applied
            }
        }

        // If no coupon matched
        cout << "Coupon not found!\n";
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
        cout << "Product Name: " << productName << "\n";
        cout << "Quantity: " << quantity << "\n";
        cout << "Price per Unit: " << pricePerUnit << "\n";
        cout << "Total Amount: " << totalAmount << "\n";
        cout << "Discount Applied: " << discountAmount << " (" << discountRate * 100 << "%)\n";
        cout << "Subtotal after Discount: " << subTotal << "\n";
        cout << "CGST (9%): " << cgst << "\n";
        cout << "SGST (9%): " << sgst << "\n";
        cout << "Final Amount to Pay: " << finalAmount << "\n";
        cout << "-------------------\n";

        // Save transaction details to file
        ofstream transactionFile(TRANSACTIONS_FILE, ios::app);
        transactionFile << "User," << productName << "," << quantity << "," << finalAmount << "," << time(0) << endl;
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
        cout << "\nEnter your delivery address: ";
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
                cout << "\nEnter coupon code (or press Enter to skip): ";
                string enteredCoupon;
                getline(cin, enteredCoupon);

                if (!enteredCoupon.empty())
                {
                    if (applyCoupon(enteredCoupon, discountRate))
                    {
                        cout << "Coupon applied successfully. Discount of " << discountRate * 100 << "% will be applied.\n";
                    }
                    else
                    {
                        cout << "Invalid coupon code. No discount will be applied.\n";
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
                    cout << "Invalid payment choice. Transaction cancelled.\n";
                    return;
                }

                // Save the order along with the delivery address
                string orderData = sellerName + "," + username + "," + product + "," + to_string(quantity) + "," + to_string(time(0)) + "," + deliveryAddress;
                ofstream orderFile(ORDERS_FILE, ios::app);
                orderFile << orderData << endl;
                orderFile.close();

                cout << "Order placed successfully for " << quantity << " unit(s) of \"" << productName << "\".\n";
                cout << "Delivery Address: " << deliveryAddress << endl;
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
            cout << "Product not found or insufficient stock.\n";
        }
    }

    void checkOrderStatus()
    {
        ifstream file(NOTIFICATIONS_FILE);
        if (!file.is_open())
        {
            cout << "Error: Unable to open notifications file.\n";
            return;
        }

        string line;
        bool hasNotifications = false;

        cout << "\n=== Order Status ===\n";

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
                    cout << "Product: " << product
                         << " | Status: " << status
                         << " | Updated At: " << ctime(&notificationTime); // Format as human-readable date
                }
                catch (const exception &e)
                {
                    cout << "Error processing notification timestamp: " << e.what() << "\n";
                }
            }
        }

        file.close();

        if (!hasNotifications)
        {
            cout << "No updates on your orders.\n";
        }
    }

    void checkMyOrders()
    {
        cout << "\n=== My Orders Status ===\n";

        // Use the checkOrderStatus function
        checkOrderStatus(); // Shows order updates for the logged-in buyer

        cout << "\nPress Enter to continue...";
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
        cout << "Product \"" << productName << "\" added successfully in category \"" << category << "\".\n";
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
                cout << "Product \"" << productName << "\" updated successfully.\n";
            }
            else
            {
                cout << "Product \"" << productName << "\" not found for the seller \"" << username << "\".\n";
            }
        }
        else
        {
            cout << "Product \"" << productName << "\" does not exist.\n";
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
            cout << "Product \"" << productName << "\" deleted successfully.\n";
        }
        else
        {
            cout << "Product not found!\n";
        }
    }

    void viewDispatchNotifications()
    {
        ifstream file(ORDERS_FILE);
        if (!file.is_open())
        {
            cout << "Error: Unable to open orders file.\n";
            return;
        }

        ofstream tempFile("temp.txt");
        if (!tempFile.is_open())
        {
            cout << "Error: Unable to create temporary file.\n";
            return;
        }

        string line;
        bool hasNotifications = false;

        cout << "\n=== Dispatch Notifications ===\n";

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

                cout << "\nOrder Details:\n";
                cout << "Buyer: " << buyer << "\n";
                cout << "Product: " << product << "\n";
                cout << "Quantity: " << quantity << "\n";
                cout << "Delivery Address: " << address << "\n";
                cout << "Ordered At: " << ctime(&orderTime);
                cout << "Do you want to dispatch this order? (y/n): ";

                char choice;
                cin >> choice;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Please enter a valid number.\n";
                    continue;
                }
                cin.ignore();

                if (choice == 'y' || choice == 'Y')
                {
                    // Record the transaction
                    string transactionData = seller + "," + buyer + "," + product + "," + quantity + "," + to_string(time(0));
                    FileHandler::saveToFile(TRANSACTIONS_FILE, transactionData);
                    cout << "Order dispatched and transaction recorded.\n";

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
            cout << "No pending orders for dispatch.\n";
        }
    }

    void viewListedProducts()
    {
        ifstream file(PRODUCTS_FILE);
        string line;
        cout << "\nYour Listed Products:\n";
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
                cout << "Product: " << product
                     << " | Price: " << price
                     << " | Quantity: " << quantity
                     << " | Category: " << category << "\n";
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
            cout << "Error opening file." << endl;
            return;
        }

        cout << "\nAll Users:\n";
        cout << "Username | Full Name | Role | Email | Phone No | Encrypted Password\n";
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

            cout << username << " | " << fullname << " | " << role << " | " << email << " | " << phone << " | " << encryptedPassword << endl;
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
            cout << "Error: Unable to open transactions file.\n";
            return;
        }

        cout << "\n=== All Transactions ===\n";
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

                    cout << "Seller: " << seller << "\n"
                         << "Buyer: " << buyer << "\n"
                         << "Product: " << product << "\n"
                         << "Quantity: " << quantity << "\n"
                         << "Transaction At: " << ctime(&transactionTime)
                         << "----------------------------------------\n";
                }
                catch (const exception &e)
                {
                    cout << "Warning: Invalid timestamp format for transaction between " << seller << " and " << buyer << "\n";
                    continue;
                }
            }
            catch (const exception &e)
            {
                cout << "Error processing transaction: " << e.what() << "\n";
                continue;
            }
        }

        file.close();

        if (!hasTransactions)
        {
            cout << "No transactions recorded yet.\n";
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
        cout << "\n=== E-commerce System ===\n";
        cout << "1. Sign Up\n2. Login\n3. Exit\nChoose an option: ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {                                                        // Check for invalid input
            cin.clear();                                         // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Flush the input buffer
            cout << "Invalid input! Please enter a valid number.\n";
            continue;
        }

        cin.ignore();

        if (choice == 1)
        {
            cout << "\n=== Sign Up ===\n";
            cout << "1. Buyer\n2. Seller\n3. Admin\nChoose user type: ";
            int userType;
            cin >> userType;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a valid number.\n";
                continue;
            }

            cin.ignore();

            string username, password, email, fullName, phoneNumber;

            cout << "Enter username: ";
            getline(cin, username);
            if (FileHandler::doesUserExist(username))
            {
                cout << "Username already exists!\n";
                continue;
            }

            cout << "Enter password (min 6 characters): ";
            getline(cin, password);
            if (!isValidPassword(password))
            {
                cout << "Invalid password!\n";
                continue;
            }

            cout << "Enter email: ";
            getline(cin, email);
            if (!isValidEmail(email))
            {
                cout << "Invalid email format!\n";
                continue;
            }

            cout << "Enter full name: ";
            getline(cin, fullName);

            cout << "Enter phone number (10 digits): ";
            getline(cin, phoneNumber);
            if (!isValidPhone(phoneNumber))
            {
                cout << "Invalid phone number!\n";
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
                cout << "Invalid user type!\n";
                continue;
            }

            newUser->saveToFile();
            cout << "Registration successful!\n";
            delete newUser;
        }
        else if (choice == 2)
        {
            cout << "\n=== Login ===\n";
            string username, password;

            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            getline(cin, password);

            User *user = User::login(username, password);
            if (user)
            {
                cout << "Login successful!\n";

                if (Buyer *buyer = dynamic_cast<Buyer *>(user))
                {
                    while (true)
                    {
                        cout << "\n=== Buyer Menu ===\n";
                        cout << "1. View All Products\n"
                             << "2. Place Order\n"
                             << "3. View Products by Category\n"
                             << "4. My Orders\n"
                             << "5. Logout\n"
                             << "Choose an option: ";

                        int buyerChoice;
                        cin >> buyerChoice;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input! Please enter a valid number.\n";
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

                            cout << "Enter product name: ";
                            getline(cin, productName);
                            cout << "Enter quantity: ";
                            cin >> quantity;
                            if (cin.fail())
                            {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Invalid input! Please enter a valid number.\n";
                                continue;
                            }
                            cin.ignore();

                            buyer->placeOrder(username, productName, quantity);
                        }
                        else if (buyerChoice == 3)
                        {
                            cout << "\nSelect a category:\n"
                                 << "1. Electronics\n"
                                 << "2. Clothing\n"
                                 << "3. Home\n"
                                 << "4. Books\n"
                                 << "Choose a category: ";

                            int categoryChoice;
                            cin >> categoryChoice;
                            if (cin.fail())
                            {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Invalid input! Please enter a valid number.\n";
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
                                cout << "Invalid category!\n";
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
                            cout << "Logging out...\n";
                            break;
                        }
                        else
                        {
                            cout << "Invalid choice! Try again.\n";
                        }
                    }
                }
                else if (Seller *seller = dynamic_cast<Seller *>(user))
                {
                    while (true)
                    {
                        cout << "\n=== Seller Menu ===\n";
                        cout << "1. Add Product\n"
                             << "2. Edit Product\n"
                             << "3. Delete Product\n"
                             << "4. View Listed Products\n"
                             << "5. View Dispatch Notifications\n"
                             << "6. Logout\n"
                             << "Choose an option: ";

                        int sellerChoice;
                        cin >> sellerChoice;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input! Please enter a valid number.\n";
                            continue;
                        }
                        cin.ignore();

                        if (sellerChoice == 1)
                        {
                            string productName;
                            double price;
                            int quantity;
                            string category;

                            cout << "Enter product name: ";
                            getline(cin, productName);
                            cout << "Enter price: ";
                            cin >> price;
                            cout << "Enter quantity: ";
                            cin >> quantity;
                            cin.ignore();

                            cout << "Select category (1. Electronics, 2. Clothing, 3. Home, 4. Books): ";
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

                            cout << "Enter product name to edit: ";
                            getline(cin, productName);

                            cout << "Enter new price: ";
                            cin >> newPrice;
                            cout << "Enter new quantity: ";
                            cin >> newQuantity;
                            cin.ignore();
                            cout << "Enter new category: ";
                            getline(cin, newCategory);

                            seller->editProduct(productName, newPrice, newQuantity, newCategory);
                        }
                        else if (sellerChoice == 3)
                        {
                            string productName;

                            cout << "Enter product name to delete: ";
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
                            cout << "Logging out...\n";
                            break;
                        }
                        else
                        {
                            cout << "Invalid choice! Try again.\n";
                        }
                    }
                }
                else if (Admin *admin = dynamic_cast<Admin *>(user))
                {
                    while (true)
                    {
                        cout << "\n=== Admin Menu ===\n";
                        cout << "1. View All Users\n"
                             << "2. View All Transactions\n"
                             << "3. Logout\n"
                             << "Choose an option: ";

                        int adminChoice;
                        cin >> adminChoice;
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
                            cout << "Logging out...\n";
                            break;
                        }

                        else
                        {
                            cout << "Invalid choice! Try again.\n";
                        }
                    }
                }

                delete user;
            }
            else
            {
                cout << "Invalid username or password.\n";
            }
        }
        else if (choice == 3)
        {
            cout << "Goodbye!\n";
            break;
        }
        else
        {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
