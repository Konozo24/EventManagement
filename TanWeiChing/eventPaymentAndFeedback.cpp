
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

// ===== Files =====
const string RECEIPT_FILE  = "receipt.txt";
const string FEEDBACK_FILE = "feedback.txt";

// ===== Product struct =====
struct Product {
    string name;
    double price;
    int quantity;

    Product() : name(""), price(0.0), quantity(0) {}
    Product(const string& n, double p, int q) : name(n), price(p), quantity(q) {}
};

// ===== Global Vector for selected products (optional) =====
vector<Product> selectedProducts;

// ===== Utility =====
void clearScreen() {
#ifdef _WIN32
    system("cls");
#endif
}

// ===== Helper: Get next receipt number =====
int getNextReceiptNumber() {
    ifstream in(RECEIPT_FILE);
    string line;
    int lastNum = 0;

    while (getline(in, line)) {
        if (line.find("Receipt #") != string::npos) {
            lastNum = stoi(line.substr(9));
        }
    }
    return lastNum + 1;
}

// ===== Save receipt =====
void saveReceipt(const string& name, const string& eventName,
                 const vector<Product>& products,
                 int ticketAmount, double ticketPrice,
                 const string& method) {
    double productTotal = 0.0;
    int receiptCount = getNextReceiptNumber();
    ofstream out(RECEIPT_FILE, ios::app);

    out << "Receipt #" << receiptCount << ":\n";
    out << "Name            : " << name << "\n";
    out << "Event           : " << eventName << "\n";

    if (products.empty()) {
        out << "Product         : None\n";
    } else {
        for (const auto& p : products) {
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

// ===== Process payment =====
void processPayment() {
    string guestID, name, eventName;
    int ticketAmount;
    double ticketPrice;
    string method;

    cout << "\n====== PAYMENT MODULE ======\n";

    cout << "Enter Guest Name: ";
    getline(cin, name);

    cout << "Enter Event Name: ";
    getline(cin, eventName);

    cout << "Enter Ticket Amount: ";
    cin >> ticketAmount;
    cin.ignore();
    cout << "Enter Ticket Price (RM): ";
    cin >> ticketPrice;
    cin.ignore();

    // Dummy products selection for demo
    selectedProducts = { {"SmartWatch Pro", 299.0, 1} };

    cout << "\nChoose Payment Method (Cash / Credit / E-Wallet): ";
    getline(cin, method);

    if (method == "Cash" || method == "Credit" || method == "E-Wallet") {
        saveReceipt(name, eventName, selectedProducts, ticketAmount, ticketPrice, method);
    } else {
        cout << "\nInvalid payment method!\n";
    }

    cout << "\nPress Enter to return to menu...";
    cin.get();
    clearScreen();
}

// ===== View all receipts =====
void viewReceipts() {
    ifstream in(RECEIPT_FILE);
    if (!in.is_open()) {
        cout << "\nNo payment records found.\n";
        return;
    }

    cout << "\n======= ALL RECEIPTS =======\n";
    string line;
    while (getline(in, line)) {
        cout << line << endl;
    }
    in.close();

    cout << "\nPress Enter to return to menu...";
    cin.get();
    clearScreen();
}

// ===== Feedback Struct =====
struct Feedback {
    string guestID;
    string name;
    string eventName;
    int rating;
    string comment;
};

// ===== Helper: Get next feedback number =====
int getNextFeedbackNumber() {
    ifstream in(FEEDBACK_FILE);
    string line;
    int lastNum = 0;

    while (getline(in, line)) {
        if (line.find("Feedback #") != string::npos) {
            lastNum = stoi(line.substr(10));
        }
    }
    return lastNum + 1;
}

// ===== Submit feedback =====
void submitFeedback() {
    Feedback fb;
    cout << "\n====== SUBMIT FEEDBACK ======\n";

    cout << "Enter Guest ID: ";
    getline(cin, fb.guestID);

    cout << "Enter Name: ";
    getline(cin, fb.name);

    cout << "Enter Event Name: ";
    getline(cin, fb.eventName);

    cout << "Enter Rating (1-5): ";
    cin >> fb.rating;
    cin.ignore();
    cout << "Enter Comment: ";
    getline(cin, fb.comment);

    int count = getNextFeedbackNumber();
    ofstream out(FEEDBACK_FILE, ios::app);
    out << "Feedback #" << count << ":\n"
        << "Guest ID   : " << fb.guestID << "\n"
        << "Name       : " << fb.name << "\n"
        << "Event Name : " << fb.eventName << "\n"
        << "Rating     : " << fb.rating << "/5\n"
        << "Comment    : " << fb.comment << "\n"
        << "----------------------------------\n\n";
    out.close();

    cout << "\nFeedback submitted successfully!\n";
    cout << "Press Enter to return to menu...";
    cin.get();
    clearScreen();
}

// ===== View all feedback =====
void viewFeedback() {
    ifstream in(FEEDBACK_FILE);
    if (!in.is_open()) {
        cout << "\nNo feedback records found.\n";
        return;
    }

    cout << "\n======= ALL FEEDBACK =======\n";
    string line;
    while (getline(in, line)) {
        cout << line << endl;
    }
    in.close();

    cout << "\nPress Enter to return to menu...";
    cin.get();
    clearScreen();
}

// ===== Payment & Feedback Menu =====
void paymentMenu() {
    int choice;
    do {
        cout << "\n" << string(50, '=') << endl;
        cout << "           PAYMENT & FEEDBACK MENU" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Process Payment" << endl;
        cout << "2. View Receipts" << endl;
        cout << "3. Submit Feedback" << endl;
        cout << "4. View Feedback" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        clearScreen();
        switch (choice) {
        case 1:
            processPayment();
            break;
        case 2:
            viewReceipts();
            break;
        case 3:
            submitFeedback();
            break;
        case 4:
            viewFeedback();
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice! Please select 0-4." << endl;
        }

    } while (choice != 0);
}

// ===== Demo main =====
int main() {
    int choice;
    do {
        cout << "\n" << string(50, '=') << endl;
        cout << "      PRODUCT LAUNCH EVENT SYSTEM" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Payment & Feedback" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();
        clearScreen();

        switch (choice) {
        case 1:
            paymentMenu();
            break;
        case 0:
            cout << "\nThank you for using the system!" << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-1." << endl;
        }

    } while (choice != 0);

    return 0;
}
