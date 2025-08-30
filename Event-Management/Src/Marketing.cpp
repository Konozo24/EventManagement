#include "Marketing.h"
#include "Event.h"
#include "Utils.h"
#include "Constants.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

struct MarketingItem {
    string eventID;
    string name;
    double price;
    int quantity;
};

static vector<MarketingItem> products;

// ---------------- Helpers ----------------
static bool isAllDigits(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

// reads integer with validation; if allowCancel == true and user types "cancel" returns false
static bool readInt(const string& prompt, int& out, int minVal, int maxVal, bool allowCancel = false) {
    while (true) {
        cout << prompt;
        string line; getline(cin, line);

        if (allowCancel && line == "cancel") return false;

        if (!isAllDigits(line)) {
            clearScreen();
            cout << "Invalid input! Numbers only.\n";
            continue;
        }
        try {
            long long v = stoll(line);
            if (v < minVal || v > maxVal) {
                clearScreen();
                cout << "Invalid range! Enter between " << minVal << " and " << maxVal << ".\n";
                continue;
            }
            out = static_cast<int>(v);
            return true;
        }
        catch (...) {
            clearScreen();
            cout << "Invalid number! Try again.\n";
        }
    }
}

// read double with validation. If allowBlankKeep == true and user presses Enter (empty string), set out = currentVal and return true.
static bool readDouble(const string& prompt, double& out, double minVal, double maxVal, bool allowBlankKeep = false, double currentVal = 0.0) {
    while (true) {
        cout << prompt;
        string line; getline(cin, line);

        if (allowBlankKeep && line.empty()) {
            out = currentVal;
            return true;
        }

        try {
            size_t idx = 0;
            double v = stod(line, &idx);
            if (idx != line.size()) {
                clearScreen();
                cout << "Invalid input! Please enter a valid number.\n";
                continue;
            }
            if (v < minVal || v > maxVal) {
                clearScreen();
                cout << "Invalid range! Enter between " << minVal << " and " << maxVal << ".\n";
                continue;
            }
            out = v;
            return true;
        }
        catch (...) {
            clearScreen();
            cout << "Invalid number! Try again.\n";
        }
    }
}

static string findEventNameByID(const string& evID) {
    for (const auto& ev : events) {
        if (ev.eventID == evID) return ev.eventName;
    }
    return string();
}

// ---------------- File I/O ----------------
static void loadProductsFromFile() {
    products.clear();
    ifstream file(PRODUCTS_FILE);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string eventID, name, priceStr, qtyStr;
        getline(ss, eventID, '|');
        getline(ss, name, '|');
        getline(ss, priceStr, '|');
        getline(ss, qtyStr, '|');

        if (eventID.empty() || name.empty() || priceStr.empty() || qtyStr.empty()) continue;
        try {
            MarketingItem mi;
            mi.eventID = eventID;
            mi.name = name;
            mi.price = stod(priceStr);
            mi.quantity = stoi(qtyStr);
            if (mi.quantity < 0) mi.quantity = 0;
            products.push_back(mi);
        }
        catch (...) {
            
        }
    }
    file.close();
}

static void saveProductsToFile() {
    ofstream file(PRODUCTS_FILE);
    for (const auto& p : products) {
        int q = p.quantity < 0 ? 0 : p.quantity; 
        file << p.eventID << "|" << p.name << "|" << fixed << setprecision(2) << p.price << "|" << q << "\n";
    }
    file.close();
}

// ---------------- Admin ----------------
void marketingAdmin() {
    while (true) {
        clearScreen();
        cout << "\n" << string(60, '=') << endl;
        cout << "               MARKETING ADMIN MENU" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Edit Product" << endl;
        cout << "3. View All Products" << endl;
        cout << "cancel. Return to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter choice: ";

        string choiceLine; getline(cin, choiceLine);
        if (choiceLine == "cancel") { clearScreen(); return; }
        if (!isAllDigits(choiceLine)) { clearScreen(); continue; }
        int choice = stoi(choiceLine);

        loadProductsFromFile();
        loadEventsFromFile();

        if (choice == 1) {
            // Add product
            clearScreen();
            cout << "\n" << string(80, '=') << endl;
            cout << "                AVAILABLE EVENTS" << endl;
            cout << string(80, '=') << endl;
            displayEventsForRegistration();

            cout << "\nEnter Event Name to link product (or 'cancel'): ";
            string evName; getline(cin, evName);
            if (evName == "cancel") { clearScreen(); continue; }

            Event* ev = findEventByName(evName);
            if (!ev) { clearScreen(); cout << "Invalid event name! Press Enter..."; cin.get(); clearScreen(); continue; }

            cout << "Enter Product Name: ";
            string pname; getline(cin, pname);
            if (pname.empty()) { clearScreen(); cout << "Product name cannot be empty! Press Enter..."; cin.get(); clearScreen(); continue; }

            double price;
            if (!readDouble("Enter Price (RM): ", price, 0.01, 1000000.0)) { clearScreen(); continue; }

            int qty;
            if (!readInt("Enter Quantity: ", qty, 0, 1000000)) { clearScreen(); continue; }

            products.push_back({ ev->eventID, pname, price, qty });
            saveProductsToFile();

            clearScreen();
            cout << "Product added successfully!\nPress Enter...";
            cin.get();
            clearScreen();

        }
        else if (choice == 2) {
            // Edit product
            clearScreen();
            loadProductsFromFile();
            if (products.empty()) { cout << "No products to edit.\nPress Enter..."; cin.get(); clearScreen(); continue; }

            cout << "\n" << string(60, '=') << endl;
            cout << "               EDIT PRODUCTS" << endl;
            cout << string(60, '=') << endl;

            for (size_t i = 0; i < products.size(); ++i) {
                string evName = findEventNameByID(products[i].eventID);
                cout << (i + 1) << ". [" << products[i].eventID << "] " << products[i].name
                    << " | Event: " << (evName.empty() ? "<unknown>" : evName)
                    << " | RM" << fixed << setprecision(2) << products[i].price
                    << " | Qty: " << products[i].quantity << "\n";
            }

            cout << "\nChoose product number to edit (or 'cancel'): ";
            string pick; getline(cin, pick);
            if (pick == "cancel") { clearScreen(); continue; }
            if (!isAllDigits(pick)) { clearScreen(); continue; }
            int idx = stoi(pick) - 1;
            if (idx < 0 || idx >= (int)products.size()) { clearScreen(); continue; }

            MarketingItem& p = products[idx];

            cout << "\nEditing product: " << p.name << "\n";
            cout << "Enter new name (press Enter to keep): ";
            string newName; getline(cin, newName);
            if (!newName.empty()) p.name = newName;

            double newPrice;
            if (!readDouble("Enter new price (press Enter to keep): ", newPrice, 0.01, 1000000.0, true, p.price)) {
                clearScreen(); continue;
            }
            p.price = newPrice;

            cout << "Enter new quantity (press Enter to keep): ";
            string qtyLine; getline(cin, qtyLine);
            if (!qtyLine.empty()) {
                if (!isAllDigits(qtyLine)) { clearScreen(); cout << "Invalid number! Press Enter..."; cin.get(); clearScreen(); continue; }
                int newQty = stoi(qtyLine);
                if (newQty < 0) { clearScreen(); cout << "Quantity cannot be negative! Press Enter..."; cin.get(); clearScreen(); continue; }
                p.quantity = newQty;
            }

            saveProductsToFile();
            clearScreen();
            cout << "Product updated!\nPress Enter...";
            cin.get();
            clearScreen();

        }
        else if (choice == 3) {
            // View all
            clearScreen();
            loadProductsFromFile();
            if (products.empty()) { cout << "No products found.\nPress Enter..."; cin.get(); clearScreen(); continue; }

            cout << "\n" << string(60, '=') << endl;
            cout << "               ALL PRODUCTS" << endl;
            cout << string(60, '=') << endl;
            for (const auto& p : products) {
                string evName = findEventNameByID(p.eventID);
                cout << "[" << p.eventID << "] "
                    << left << setw(30) << p.name
                    << " RM" << fixed << setprecision(2) << setw(8) << p.price
                    << " Qty: " << p.quantity
                    << " | Event: " << (evName.empty() ? "<unknown>" : evName) << "\n";
            }
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
        }
        else {
            clearScreen();
            
        }
    }
}

// ---------------- User ----------------
void marketingUser() {
    while (true) {
        clearScreen();
        loadEventsFromFile();

        cout << "\n" << string(80, '=') << endl;
        cout << "                      AVAILABLE EVENTS" << endl;
        cout << string(80, '=') << endl;
        displayEventsForRegistration();

        cout << "\nEnter Event Name to view products (or 'cancel'): ";
        string evName; getline(cin, evName);
        if (evName == "cancel") { clearScreen(); return; }

        Event* ev = findEventByName(evName);
        if (!ev) {
            clearScreen();
            cout << "Invalid event name! Try again.\nPress Enter...";
            cin.get();
            continue;
        }

        loadProductsFromFile();
        vector<MarketingItem*> evProducts;
        for (auto& it : products) if (it.eventID == ev->eventID) evProducts.push_back(&it);

        clearScreen();
        cout << "\n" << string(60, '=') << endl;
        cout << "        PRODUCTS FOR EVENT " << ev->eventName << endl;
        cout << string(60, '=') << endl;

        if (evProducts.empty()) {
            cout << "No products available for this event.\n";
            cout << "\nPress Enter to return..."; cin.get(); clearScreen(); continue;
        }

        for (size_t i = 0; i < evProducts.size(); ++i) {
            auto* p = evProducts[i];
            cout << setw(3) << (i + 1) << ". "
                << left << setw(30) << p->name
                << " RM" << fixed << setprecision(2) << setw(8) << p->price
                << " Stock: " << p->quantity;
            if (p->quantity == 0) cout << " (OUT OF STOCK)";
            cout << "\n";
        }

        cout << "\nEnter product number to buy (or 'cancel'): ";
        string pick; getline(cin, pick);
        if (pick == "cancel") { clearScreen(); continue; }
        if (!isAllDigits(pick)) { clearScreen(); continue; }
        int idx = stoi(pick) - 1;
        if (idx < 0 || idx >= (int)evProducts.size()) { clearScreen(); continue; }

        MarketingItem* selected = evProducts[idx];
        if (selected->quantity == 0) { clearScreen(); cout << "This product is out of stock!\nPress Enter..."; cin.get(); continue; }

        int qty;
        if (!readInt("Enter quantity (or 'cancel'): ", qty, 1, selected->quantity, true)) { clearScreen(); continue; }

        selected->quantity -= qty;
        if (selected->quantity < 0) selected->quantity = 0; // clamp safety
        saveProductsToFile();

        // Confirmation screen
        clearScreen();
        cout << "\n" << string(60, '=') << endl;
        cout << "           PURCHASE CONFIRMATION" << endl;
        cout << string(60, '=') << endl;
        cout << "Event       : " << ev->eventName << " (" << ev->eventID << ")\n";
        cout << "Product     : " << selected->name << "\n";
        cout << "Unit Price  : RM" << fixed << setprecision(2) << selected->price << "\n";
        cout << "Quantity    : " << qty << "\n";
        cout << "Total Paid  : RM" << fixed << setprecision(2) << (qty * selected->price) << "\n";
        cout << string(60, '=') << endl;
        cout << "Status      : Purchase Successful!" << endl;
        cout << string(60, '=') << endl;

        cout << "\nPress Enter to continue...";
        cin.get();
        clearScreen();
    }
}
