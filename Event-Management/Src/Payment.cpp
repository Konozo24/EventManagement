#include "Payment.h"
#include "Constants.h"
#include "Registration.h"
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
void saveReceipt(const Registration& reg, const vector<Product>& selectedProducts, const string& method) {

    double productTotal = 0.0;

    int receiptCount = getNextReceiptNumber();
    ofstream out(RECEIPT_FILE, ios::app);

    out << "Receipt #" << receiptCount << ":\n";
    out << "Registration ID        : " << reg.registrationID << "\n";
    out << "Name            : " << reg.userName << "\n";
    out << "Event           : " << reg.eventName << " (" << reg.eventID << ")\n";
    out << "Tickets         : " << reg.ticketsBought << "\n";
    out << "Ticket Price    : RM" << fixed << setprecision(2)
        << reg.registrationCost / reg.ticketsBought << "\n";
    out << "Ticket Total    : RM" << fixed << setprecision(2) << reg.registrationCost << "\n";

    if (selectedProducts.empty()) {
        out << "Product         : None\n";  
    }
    else {
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

    
    double grandTotal = reg.registrationCost + productTotal;

    out << "Ticket Price    : RM" << fixed << setprecision(2) << reg.registrationCost << "\n";
    out << "Product Total   : RM" << fixed << setprecision(2) << productTotal << "\n";
    out << "Total           : RM" << fixed << setprecision(2) << grandTotal << "\n";
    out << "Payment Method  : " << method << "\n";
    out << "----------------------------------\n\n";

    out.close();

    cout << "\nPayment successful! Receipt saved.\n";
}


// ===== Main: Checkout process =====
void processPayment(Registration& reg) {
    // TODO: Replace with real lookup from registration.txt later
    string name = "TEMP_NAME";
    string eventName = "TEMP_EVENT";
    int ticketAmount = 1;
    double ticketPrice = 30.0;

    // Load registrations from file
    loadRegistrationFromFile();

    // Dummy selected products (later link with Registration module)
    vector<Product> selectedProducts = { {"SmartWatch Pro", 299.0, 1} };

    double productTotal = 0.0;

    cout << "\n--- Payment Summary ---\n";
    cout << "Registration ID    : " << reg.registrationID << endl;
    cout << "Name        : " << reg.userName << endl;
    cout << "Event       : " << reg.eventName << " (" << reg.eventID << ")\n";
    cout << "Tickets     : " << reg.ticketsBought << endl;
    cout << "Ticket Price: RM" << fixed << setprecision(2)
        << reg.registrationCost / reg.ticketsBought << endl;
    cout << "Ticket Total: RM" << fixed << setprecision(2) << reg.registrationCost << endl;

    if (selectedProducts.empty()) {
        cout << "Product         : None\n";
    }
    else {
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

    double grandTotal = reg.registrationCost + productTotal;


    cout << "Ticket Price    : RM" << fixed << setprecision(2) << reg.registrationCost << "\n";
    cout << "Product Total   : RM" << fixed << setprecision(2) << productTotal << "\n";
    cout << "Total           : RM" << fixed << setprecision(2) << grandTotal << "\n";

    string method;
    bool valid = false;

    while (!valid) {
        cout << "\nChoose payment method (Cash / Credit / E-Wallet): ";
        getline(cin, method);

        if (method == "Cash" || method == "Credit" || method == "E-Wallet") {
            valid = true;
        }
        else {
            cout << "Invalid payment method! Try again.\n";
        }
    }

    // Save receipt
    saveReceipt(reg, selectedProducts, method);

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

    string regID;
    cout << "Enter Registration ID to view receipt (e.g., R1): ";
    getline(cin, regID);

    bool found = false;
    string line;
    vector<string> currentReceipt;

    while (getline(in, line)) {
        if (line.find("----------------------------------") != string::npos) {
            // End of a receipt
            for (const auto& l : currentReceipt) {
                if (l.find("Registration ID") != string::npos && l.find(regID) != string::npos) {
                    found = true;
                    cout << "\n===== Payment Receipt =====\n";
                    for (const auto& rline : currentReceipt) cout << rline << "\n";
                    cout << "----------------------------------\n";
                    break;
                }
            }
            currentReceipt.clear();
        }
        else {
            currentReceipt.push_back(line);
        }

        if (found) break;
    }

    if (!found) cout << "No receipt found for Registration ID " << regID << ".\n";

    in.close();
    cout << "\nPress Enter to return to menu...";
    cin.get();
    clearScreen();
}
