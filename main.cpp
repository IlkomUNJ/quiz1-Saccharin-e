#include <iostream>
#include <vector>
#include <string>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"

enum PrimaryPrompt{LOGIN, REGISTER, EXIT};
enum UserAction{CHECK_STATUS = 1, UPGRADE_SELLER, BANK_ACCOUNT, LOGOUT, DELETE_ACCOUNT};

using namespace std;

vector<BankCustomer> bankCustomers;
vector<Buyer> buyers;
vector<seller> sellers;
int nextId = 1;

BankCustomer* findBankCustomerByName(const string& name) {
    for (auto& customer : bankCustomers) {
        if (customer.getName() == name) {
            return &customer;
        }
    }
    return nullptr;
}

Buyer* findBuyerByName(const string& name) {
    for (auto& buyer : buyers) {
        if (buyer.getName() == name) {
            return &buyer;
        }
    }
    return nullptr;
}

seller* findSellerByName(const string& name) {
    for (auto& s : sellers) {
        if (s.getName() == name) {
            return &s;
        }
    }
    return nullptr;
}

void displayUserMenu(const string& username) {
    seller* s = findSellerByName(username);
    cout << "\n=== Welcome " << username << " ===" << endl;
    cout << "1. Check Account Status" << endl;
    if (s) {
        cout << "2. Upgrade to Seller Account (Already Upgraded)" << endl;
    } else {
        cout << "2. Upgrade to Seller Account" << endl;
    }
    cout << "3. Bank Account Functions" << endl;
    cout << "4. Logout" << endl;
    cout << "5. Delete Account" << endl;
    cout << "Enter your choice: ";
}

void checkAccountStatus(const string& username) {
    Buyer* buyer = findBuyerByName(username);
    seller* s = findSellerByName(username);
    BankCustomer* bankCustomer = findBankCustomerByName(username);
    
    cout << "\n=== Account Status ===" << endl;
    cout << "Username: " << username << endl;
    
    if (buyer) {
        cout << "Buyer Account: Active" << endl;
        cout << "Buyer ID: " << buyer->getId() << endl;
        cout << "Bank Account: " << (bankCustomer ? "Linked" : "Not Linked") << endl;
        if (bankCustomer) {
            cout << "Bank Balance: $" << bankCustomer->getBalance() << endl;
        }
    }
    
    if (s) {
        cout << "Seller Account: Active" << endl;
        cout << "Seller ID: " << s->getId() << endl;
    } else {
        cout << "Seller Account: Not Active" << endl;
    }
    cout << endl;
}

void upgradeToSeller(const string& username) {
    Buyer* buyer = findBuyerByName(username);
    if (!buyer) {
        cout << "Error: Buyer account not found!" << endl;
        return;
    }
    
    seller* existingSeller = findSellerByName(username);
    if (existingSeller) {
        cout << "You already have a seller account!" << endl;
        return;
    }
    
    BankCustomer* bankCustomer = findBankCustomerByName(username);
    if (!bankCustomer) {
        cout << "Error: You must have a bank account linked to upgrade to seller!" << endl;
        cout << "Please create a bank account first." << endl;
        return;
    }
    
    cout << "\n=== Seller Account Upgrade ===" << endl;
    string storeName, storeAddress, storePhone, storeEmail;
    
    cout << "Enter Store Name: ";
    cin.ignore();
    getline(cin, storeName);
    
    cout << "Enter Store Address: ";
    getline(cin, storeAddress);
    
    cout << "Enter Store Phone: ";
    getline(cin, storePhone);
    
    cout << "Enter Store Email: ";
    getline(cin, storeEmail);
    
    seller newSeller(*buyer, nextId++, storeName);
    sellers.push_back(newSeller);
    
    cout << "Seller account created successfully!" << endl;
    cout << "Store Name: " << storeName << endl;
    cout << "You can now sell items on the platform." << endl;
}

void bankAccountFunctions(const string& username) {
    Buyer* buyer = findBuyerByName(username);
    if (!buyer) {
        cout << "Error: Buyer account not found!" << endl;
        return;
    }
    
    BankCustomer* bankCustomer = findBankCustomerByName(username);
    
    if (!bankCustomer) {
        cout << "\n=== Create Bank Account ===" << endl;
        cout << "You don't have a bank account. Let's create one." << endl;
        
        string address, phone, email;
        double initialDeposit;
        
        cout << "Enter your address: ";
        cin.ignore();
        getline(cin, address);
        
        cout << "Enter your phone number: ";
        getline(cin, phone);
        
        cout << "Enter your email: ";
        getline(cin, email);
        
        cout << "Enter initial deposit amount: $";
        cin >> initialDeposit;
        
        if (initialDeposit < 0) {
            cout << "Invalid deposit amount!" << endl;
            return;
        }
        
        BankCustomer newBankCustomer(nextId++, username, initialDeposit);
        bankCustomers.push_back(newBankCustomer);
        
        cout << "Bank account created successfully!" << endl;
        cout << "Initial balance: $" << initialDeposit << endl;
        
        // Update bankCustomer pointer to the newly created account
        bankCustomer = &bankCustomers.back();
    }
    
    // Bank account functions loop
    bool inBankMenu = true;
    while (inBankMenu) {
        cout << "\n=== Bank Account Functions ===" << endl;
        cout << "1. Check Balance" << endl;
        cout << "2. Deposit Money" << endl;
        cout << "3. Withdraw Money" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Current Balance: $" << bankCustomer->getBalance() << endl;
                break;
            case 2: {
                double amount;
                cout << "Enter deposit amount: $";
                cin >> amount;
                if (amount > 0) {
                    bankCustomer->addBalance(amount);
                    cout << "Deposited $" << amount << " successfully!" << endl;
                    cout << "New balance: $" << bankCustomer->getBalance() << endl;
                } else {
                    cout << "Invalid deposit amount!" << endl;
                }
                break;
            }
            case 3: {
                double amount;
                cout << "Enter withdrawal amount: $";
                cin >> amount;
                if (bankCustomer->withdrawBalance(amount)) {
                    cout << "Withdrew $" << amount << " successfully!" << endl;
                    cout << "New balance: $" << bankCustomer->getBalance() << endl;
                } else {
                    cout << "Insufficient funds or invalid amount!" << endl;
                }
                break;
            }
            case 4:
                inBankMenu = false;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

void handleLogin() {
    string username, password;
    cout << "\n=== Login ===" << endl;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    
    Buyer* buyer = findBuyerByName(username);
    if (!buyer) {
        cout << "Invalid username or password!" << endl;
        return;
    }
    
    cout << "Login successful!" << endl;
    bool loggedIn = true;
    while (loggedIn) {
        displayUserMenu(username);
        int choice;
        cin >> choice;
        
        switch (choice) {
            case CHECK_STATUS:
                checkAccountStatus(username);
                break;
            case UPGRADE_SELLER:
                upgradeToSeller(username);
                break;
            case BANK_ACCOUNT:
                bankAccountFunctions(username);
                break;
            case LOGOUT:
                cout << "Logging out..." << endl;
                loggedIn = false;
                break;
            case DELETE_ACCOUNT:
                cout << "Are you sure you want to delete your account? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    cout << "Account deleted successfully!" << endl;
                    loggedIn = false;
                }
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
        cout << endl;
    }
}

void handleRegister() {
    string username, password, fullName, address, phone, email;
    
    cout << "\n=== Registration ===" << endl;
    cout << "Enter username: ";
    cin >> username;
    
    if (findBuyerByName(username)) {
        cout << "Username already exists! Please choose a different username." << endl;
        return;
    }
    
    cout << "Enter password: ";
    cin >> password;
    
    cout << "Enter your full name: ";
    cin.ignore();
    getline(cin, fullName);
    
    cout << "Enter home address: ";
    getline(cin, address);
    
    cout << "Enter phone number: ";
    getline(cin, phone);
    
    cout << "Enter email: ";
    getline(cin, email);
    
    BankCustomer tempBankCustomer(0, "", 0.0);
    
    Buyer newBuyer(nextId++, username, tempBankCustomer);
    buyers.push_back(newBuyer);
    
    cout << "Buyer account created successfully!" << endl;
    cout << "Full Name: " << fullName << endl;
    cout << "Username: " << username << endl;
    cout << "You can now login with your username and password." << endl;
    cout << "Note: You can upgrade to seller later after creating a bank account." << endl;
}

int main() {
    PrimaryPrompt prompt = LOGIN;
    while (prompt != EXIT) {
        cout << "Select an option: " << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        int choice;
        cin >> choice;
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        switch (prompt) {
            case LOGIN:
                handleLogin();
                break;
            case REGISTER:
                handleRegister();
                break;
            case EXIT:
                cout << "Exiting." << std::endl;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
        cout << endl;
    }

    return 0;
}