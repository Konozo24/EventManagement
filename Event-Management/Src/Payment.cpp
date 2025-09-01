#include "Payment.h"
#include "Constants.h"
#include "Registration.h"
#include "Utils.h"
#include "Marketing.h"
#include "Product.h"
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
void saveReceipt(const Registration& reg, const vector<MarketingItem>& selectedProducts, const string& method) {

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
        const int labelWidth = 16; // width of "Product         : "
        bool first = true;
        for (const auto& p : selectedProducts) {
            if (p.quantity > 0) {
                double subtotal = p.price * p.quantity;

                if (first) {
                    out << "Product         : ";
                    first = false;
                }
                else {
                    out << string(labelWidth, ' '); // indent to align with first line
                }

                out << setw(20) << left << p.name  // name column width
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
void processPayment(Registration& reg, Event* ev) {
    
    // Load registrations from file
    loadRegistrationFromFile();
    loadProductsFromFile();

    vector<MarketingItem> selectedProducts;
    

    cout << "\n--- Payment Summary ---\n";
    cout << "Registration ID : " << reg.registrationID << endl;
    cout << "Name        : " << reg.userName << endl;
    cout << "Event       : " << reg.eventName << " (" << reg.eventID << ")\n";
    cout << "Tickets     : " << reg.ticketsBought << endl;
    cout << "Ticket Price: RM" << fixed << setprecision(2)
        << reg.registrationCost / reg.ticketsBought << endl;
    cout << "Ticket Total: RM" << fixed << setprecision(2) << reg.registrationCost << endl;

    // --- Dynamic 2D Array for product payment details ---
    int maxProducts = products.size();
    double** paymentData = new double* [maxProducts];
    for (int i = 0; i < maxProducts; i++) {
        paymentData[i] = new double[3]; // [price, qty, subtotal]
        paymentData[i][0] = 0;
        paymentData[i][1] = 0;
        paymentData[i][2] = 0;
    }

    // === Let user select products ===
    if (!products.empty()) {
        vector<int> availableIndices; // maps displayed number to actual product index
        int displayIndex = 1;

        cout << "\nAvailable Products:\n";
       
        for (size_t i = 0; i < products.size(); ++i) {
            if (products[i].quantity > 0) {
                cout << displayIndex << ". " << products[i].name
                    << " (RM" << fixed << setprecision(2) << products[i].price
                    << ") | Stock: " << products[i].quantity << "\n";
                availableIndices.push_back(i);
                displayIndex++;
            }
        }

        if (availableIndices.empty()) {
            cout << "All products are out of stock.\n";
        }
        else {
            int choice = -1;
            string input;
            while (choice != 0) {
                cout << "\nEnter product number to add (0 to finish): ";
                getline(cin, input);

                // Validate numeric input
                bool validChoice = !input.empty();
                for (char c : input) if (!isdigit(c)) validChoice = false;

                if (!validChoice) {
                    cout << "Invalid input! Enter a number.\n";
                    continue;
                }

                choice = stoi(input);
                if (choice == 0) break;

                if (choice < 1 || choice >(int)availableIndices.size()) {
                    cout << "Invalid choice!\n";
                    continue;
                }

                MarketingItem& item = products[availableIndices[choice - 1]];

                // Quantity input
                int qty = 0;
                bool validQty = false;
                do {
                    cout << "Enter quantity: ";
                    getline(cin, input);

                    validQty = !input.empty();
                    for (char c : input) if (!isdigit(c)) validQty = false;

                    if (!validQty) {
                        cout << "Invalid input! Enter a number.\n";
                        continue;
                    }

                    qty = stoi(input);
                    if (qty <= 0 || qty > item.quantity) {
                        cout << "Quantity must be >0 and ≤ stock (" << item.quantity << ").\n";
                        validQty = false;
                    }

                } while (!validQty);

                // === Update 2D array ===
                int idx = availableIndices[choice - 1];
                paymentData[idx][0] = item.price;            // price
                paymentData[idx][1] += qty;                  // add qty
                paymentData[idx][2] = paymentData[idx][0] * paymentData[idx][1]; // subtotal

                // Add to selected products
                bool found = false;
                for (auto& sel : selectedProducts) {
                    if (sel.name == item.name && sel.eventID == item.eventID) {
                        sel.quantity += qty;
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    MarketingItem selected = item;
                    selected.quantity = qty;
                    selectedProducts.push_back(selected);
                }

                // Reduce stock
                item.quantity -= qty;
                if (item.quantity == 0) {
                    cout << "Note: " << item.name << " is now out of stock.\n";
                    // Rebuild availableIndices for next display
                    availableIndices.clear();
                    displayIndex = 1;
                    for (size_t i = 0; i < products.size(); ++i) {
                        if (products[i].quantity > 0) {
                            availableIndices.push_back(i);
                            displayIndex++;
                        }
                    }
                }
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // === Calculate totals ===
    double productTotal = 0.0;
    for (int i = 0; i < products.size(); i++) {
        productTotal += paymentData[i][2];
    }


    double grandTotal = reg.registrationCost + productTotal;


    cout << "Ticket Price    : RM" << fixed << setprecision(2) << reg.registrationCost << "\n";
    cout << "Product Total   : RM" << fixed << setprecision(2) << productTotal << "\n";
    cout << "Total           : RM" << fixed << setprecision(2) << grandTotal << "\n";


	// === Choose payment method =====
    string method;
    bool validMethod = false;

    do {
        cout << "\nChoose payment method (Cash / Credit / E-Wallet): ";
        getline(cin, method);

        if (method == "Cash" || method == "Credit" || method == "E-Wallet")
            validMethod = true;
        else
            cout << "Invalid payment method! Try again.\n";
    } while (!validMethod);

    // Save updated product stock
    saveProductsToFile();

    // Save receipt
    saveReceipt(reg, selectedProducts, method);

    cout << "\nPayment successful! Press Enter to return to menu...";
    cin.get();
    clearScreen();

    // Free 2D array
    for (int i = 0; i < maxProducts; ++i)
        delete[] paymentData[i];

    delete[] paymentData;
}


// ===== Main: View receipts =====
void viewReceipts() {
    ifstream in(RECEIPT_FILE);
    if (!in) {
        cout << "\nNo payment records found.\n";
        return;
    }

    string regID;
    cout << "Enter Registration ID to view receipt (e.g., R1) or 0 to return: ";
    getline(cin, regID);

    if (regID == "0") {
        clearScreen();
        return; 
    }

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
