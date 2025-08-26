#include "Payment.h"
#include "Constants.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
using namespace std;

// ===== Helper: Get next receipt number =====
int getNextReceiptNumber() {
    ifstream in(RECEIPT_FILE);
    string line;
    int lastNum = 0;

    while (getline(in, line)) {
        if (line.find("Receipt #") != string::npos) {
            lastNum = stoi(line.substr(9)); // "Receipt #" is 9 chars
        }
    }
    return lastNum + 1;
}

// ===== Helper: Save receipt to file =====
void saveReceipt(const string& name, const string& eventName,
                 const vector<Product>& selectedProducts,
                 int ticketAmount, double ticketPrice,
                 const string& method) {
    
    double productTotal = 0.0;

    int receiptCount = getNextReceiptNumber();
    ofstream out(RECEIPT_FILE, ios::app);

    out << "Receipt #" << receiptCount << ":\n";
    out << "Name            : " << name << "\n";
    out << "Event           : " << eventName << "\n";

    if (selectedProducts.empty()) {
        out << "Product         : None\n";
    } else {
        for (const auto& p : selectedProducts) {
            if (p.quantity > 0) {
                double subtotal = p.price * p.quantity;
                out << "Product         : " 
                    << setw(12) << left << p.name 
                    << " x " << p.quantity 
                    << " = RM" << fixed << setprecision(2) << subtotal << "\n";
                productTotal += subtotal;
            }
        }
    }

    double ticketTotal = ticketAmount * ticketPrice;
    double grandTotal = ticketTotal + productTotal;

    out << "Ticket          : RM" << fixed << setprecision(2) << ticketPrice
        << " x " << ticketAmount 
        << " = RM" << fixed << setprecision(2) << ticketTotal << "\n";
    out << "Product Total   : RM" << fixed << setprecision(2) << productTotal << "\n";
    out << "Total           : RM" << fixed << setprecision(2) << grandTotal << "\n";
    out << "Payment Method  : " << method << "\n";
    out << "----------------------------------\n\n";

    out.close();

    cout << "\nPayment successful! Receipt saved.\n";
}


// ===== Main: Checkout process =====
void processPayment(const string& guestID) {
    // TODO: Replace with real lookup from registration.txt later
    string name = "TEMP_NAME";
    string eventName = "TEMP_EVENT";
    int ticketAmount = 1;
    double ticketPrice = 30.0;

    // Dummy selected products (later link with Registration module)
    vector<Product> selectedProducts = { {"SmartWatch Pro", 299.0, 1} };

    double productTotal = 0.0;
    
    cout << "\n--- Payment Summary ---\n";
    cout << "Guest ID    : " << guestID << endl;
    cout << "Name        : " << name << endl;
    cout << "Event       : " << eventName << endl;
    
    if (selectedProducts.empty()) {
        cout << "Product         : None\n";
    } else {
        for (const auto& p : selectedProducts) {
            if (p.quantity > 0) {
                double subtotal = p.price * p.quantity;
                cout << "Product         : " 
                    << setw(12) << left << p.name 
                    << " x " << p.quantity 
                    << " = RM" << fixed << setprecision(2) << subtotal << "\n";
                productTotal += subtotal;
            }
        }
    }

    double ticketTotal = ticketAmount * ticketPrice;
    double grandTotal = ticketTotal + productTotal;

    cout << "Ticket          : RM" << fixed << setprecision(2) << ticketPrice
        << " x " << ticketAmount 
        << " = RM" << fixed << setprecision(2) << ticketTotal << "\n";
    cout << "Product Total   : RM" << fixed << setprecision(2) << productTotal << "\n";
    cout << "Total           : RM" << fixed << setprecision(2) << grandTotal << "\n";

    string method;
    cout << "\nChoose payment method (Cash / Credit / E-Wallet): ";
    getline(cin, method);

    if (method == "Cash" || method == "Credit" || method == "E-Wallet") {
        saveReceipt(name, eventName, products, ticketAmount, ticketPrice, method);
    } else {
        cout << "\nInvalid payment method.\n";
    }

    cout << "\nPress Enter to return to menu...";
    cin.get();
    clearScreen();
}

// ===== Main: View receipts =====
void viewReceipts() {
    ifstream in(RECEIPT_FILE);
    if (!in) {
        cout << "\nNo payment records found.\n";
        return;
    }

    cout << "\n===== All Payment Records =====\n\n";
    string line;
    while (getline(in, line)) {
        cout << line << "\n";
    }
    in.close();

    cout << "\nPress Enter to return to menu...";
    cin.ignore();
    cin.get();
    clearScreen();
}
