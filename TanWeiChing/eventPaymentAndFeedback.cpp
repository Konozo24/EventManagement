#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

// ===== FILES =====
const string RECEIPT_FILE  = "receipt.txt";
const string FEEDBACK_FILE = "feedback.txt";

// ===== STRUCTS =====
struct Product {
    string name;
    double price;
    int quantity;
};

struct Feedback {
    string guestID;
    string name;
    string eventName;
    int rating;
    string comment;
};

vector<Product> selectedProducts;

void processPayment();        // Payment & Checkout
void viewReceipts();          // Admin view all receipts
void submitFeedback();        // Feedback & Review
void viewFeedback();          // Admin view all feedback
void paymentMenu();           // Menu wrapper
void clearScreen();           // Clear console

// ===== HELPER FUNCTIONS =====
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

void clearScreen() {
#ifdef _WIN32
    system("cls");
}

// ===== PAYMENT MODULE =====
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
        saveReceipt(name, eventName, selectedProducts, ticketAmount, ticketPrice, method);
    } else {
        cout << "\nInvalid payment method.\n";
    }

    cout << "\nPress Enter to return to menu...";
    cin.get();
    clearScreen();
}

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

// ===== FEEDBACK MODULE =====
void submitFeedback() {
    string guestID, name, eventName, comment;
    int rating;

    cout << "\n====== Feedback ======";
    cout << "\nEnter Guest ID: ";
    cin >> guestID;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);

    // Verify Guest ID + Name
    if (!verifyGuest(stoi(guestID), name)) {
        cout << "Guest ID and Name do not match our records.\n";
        return;
    }

    cout << "Enter Event Name: ";
    getline(cin, eventName);
    cout << "Enter Rating (1-5): ";
    cin >> rating;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Comment: ";
    getline(cin, comment);

    int count = getNextFeedbackNumber();
    ofstream out(FEEDBACK_FILE, ios::app);
    out << "Feedback #" << count << ":\n"
        << "Guest ID   : " << guestID << "\n"
        << "Name       : " << name << "\n"
        << "Event Name : " << eventName << "\n"
        << "Rating     : " << rating << "/5\n"
        << "Comment    : " << comment << "\n"
        << "----------------------------------\n\n";
    out.close();

    cout << "\nFeedback submitted successfully.\n";

    cin.get();     // Wait for Enter
    clearScreen();
}

void viewFeedback() {
    ifstream in(FEEDBACK_FILE);
    if (!in) {
        cout << "\nNo feedback found.\n";
        return;
    }

    cout << "\n======= All Feedback Entries =======\n";
    string line;
    while (getline(in, line)) {
        cout << line << "\n";
    }
    in.close();

    cin.get();     // Wait for Enter
    clearScreen();
}

// ===== Demo main =====
int main() {
    int choice;
    do {
        cout << "\n" << string(50, '=') << endl;
        cout << "      PRODUCT LAUNCH EVENT SYSTEM" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Payment and Checkout" << endl;
        cout << "2. Feedback and Review" << endl;
        cout << "3. View Receipts (Admin)" << endl;
        cout << "4. View Feedbacks (Admin)" << endl;
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
            processPayment(); 
            break;
        case 2: 
            submitFeedback(); 
            break;
        case 3: 
            viewReceipts(); 
            break;
        case 4: 
            viewFeedback(); 
            break;
        case 0:
            cout << "\nThank you for using the system!" << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-4." << endl;
        }

    } while (choice != 0);

    return 0;
}
