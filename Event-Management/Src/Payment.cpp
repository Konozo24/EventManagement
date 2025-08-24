#include "Payment.h"
#include "Constants.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
using namespace std;

// ===== Helper: Build product summary string =====
string buildProductSummary(const vector<Product>& selectedProducts, double& totalProductPrice) {
    stringstream summary;
    bool first = true;
    totalProductPrice = 0.0;

    for (const auto& p : selectedProducts) {
        if (p.quantity > 0) {
            if (!first) summary << ", ";
            summary << p.name << " x " << p.quantity;
            totalProductPrice += p.price * p.quantity;
            first = false;
        }
    }
    return summary.str();
}

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
                 const string& productSummary, int ticketAmount,
                 double ticketPrice, double productPrice, const string& method) {

    double total = (ticketAmount * ticketPrice) + productPrice;

    int receiptCount = getNextReceiptNumber();
    ofstream out(RECEIPT_FILE, ios::app);
    out << "Receipt #" << receiptCount << ":\n";
    out << "Name            : " << name << "\n";
    out << "Event           : " << eventName << "\n";
    out << "Product         : " << (productSummary.empty() ? "None" : productSummary) << "\n";
    out << "Ticket          : RM" << fixed << setprecision(2) << ticketPrice 
        << " x " << ticketAmount << "\n";
    out << "Product Total   : RM" << fixed << setprecision(2) << productPrice << "\n";
    out << "Total           : RM" << fixed << setprecision(2) << total << "\n";
    out << "Payment Method  : " << method << "\n";
    out << "----------------------------------\n\n";
    out.close();

    cout << "\nPayment successful! Receipt saved.\n";
}

// ===== Main: Checkout process =====
void userCheckout(const string& name, const string& eventName,
                  int ticketAmount, double ticketPrice,
                  const vector<Product>& selectedProducts) {
    double totalProductPrice;
    string productSummary = buildProductSummary(selectedProducts, totalProductPrice);
    double totalAmount = (ticketPrice * ticketAmount) + totalProductPrice;

    // Handle case where user buys nothing
    if (ticketAmount == 0 && selectedProducts.empty()) {
        cout << "\nYou did not purchase tickets or products. Returning to menu...\n";
        return;
    }
    
    cout << "\n--- Payment Summary ---\n";
    cout << "Name        : " << name << endl;
    cout << "Event       : " << eventName << endl;
    cout << "Tickets     : " << ticketAmount << " x RM" << fixed << setprecision(2)
         << ticketPrice << " = RM" << ticketPrice * ticketAmount << endl;

    if (!productSummary.empty())
        cout << "Product     : " << productSummary << " = RM" << fixed << setprecision(2)
             << totalProductPrice << endl;
    else
        cout << "Product     : None\n";

    cout << "Total       : RM" << fixed << setprecision(2) << totalAmount << endl;

    string method;
    cout << "\nChoose payment method (Cash / Credit / E-Wallet): ";
    cin >> method;

    // Convert to uppercase
    for (char& c : method) c = toupper(c);

    if (method == "CASH" || method == "CREDIT" || method == "E-WALLET") {
        saveReceipt(name, eventName, productSummary, ticketAmount, 
                    ticketPrice, totalProductPrice, method);
    }
    else {
        cout << "\nInvalid payment method.\n";
    }

    cout << "\nPress Enter to return to menu...";
    cin.ignore();
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
