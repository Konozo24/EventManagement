#include "History.h"
#include "Registration.h"

// ========== EXISTING CODE (slightly cleaned up) ==========
void viewUserHistory(const string& guestID) {
    loadRegistrationFromFile();

    cout << "\n" << string(50, '=') << endl;
    cout << "        EVENT HISTORY FOR GUEST: " << guestID << endl;
    cout << string(50, '=') << endl << endl;

    // ===== Step 1: Read receipt.txt once into a map =====
    ifstream file("receipt.txt");
    map<string, vector<string>> purchases;
    map<string, string> paymentMethods;

    if (file.is_open()) {
        string line, currentRegID;
        bool productSection = false;

        while (getline(file, line)) {
            if (line.find("Receipt #") != string::npos) {
                currentRegID.clear();
                productSection = false;
            }

            if (line.find("Registration ID") != string::npos) {
                size_t pos = line.find(":");
                if (pos != string::npos) {
                    currentRegID = line.substr(pos + 1);
                    currentRegID.erase(0, currentRegID.find_first_not_of(" \t"));
                }
            }

            if (!currentRegID.empty()) {
                if (line.find("Product") != string::npos && line.find("Product Total") == string::npos) {
                    size_t pos = line.find(":");
                    if (pos != string::npos) {
                        string item = line.substr(pos + 1);
                        item.erase(0, item.find_first_not_of(" \t"));
                        if (item == "None") {
                            purchases[currentRegID].push_back("None");
                        }
                        else if (!item.empty()) {
                            purchases[currentRegID].push_back(item);
                        }
                    }
                }

                if (line.find("Payment Method") != string::npos) {
                    size_t pos = line.find(":");
                    if (pos != string::npos) {
                        string method = line.substr(pos + 1);
                        method.erase(0, method.find_first_not_of(" \t"));
                        paymentMethods[currentRegID] = method;
                    }
                }
            }
        }
        file.close();
    }

    bool found = false;
    bool headerPrinted = false;

    for (const auto& reg : registrations) {
        if (reg.guestID == guestID) {
            if (!headerPrinted) {
                cout << left << setw(8) << "RegID"
                    << setw(6) << "EvID"
                    << setw(20) << "Event Name"
                    << setw(12) << "Date"
                    << setw(8) << "Tickets"
                    << setw(12) << "Cost(RM)"
                    << setw(10) << "Checked"
                    << setw(20) << "Check-in Time" << endl;
                cout << string(100, '-') << endl;
                headerPrinted = true;
            }

            cout << left << setw(8) << reg.registrationID
                << setw(6) << reg.eventID
                << setw(20) << reg.eventName
                << setw(12) << reg.eventDate
                << setw(8) << reg.ticketsBought
                << setw(12) << fixed << setprecision(2) << reg.registrationCost
                << setw(10) << (reg.checkedIn ? "Yes" : "No")
                << setw(20) << (reg.checkedIn ? reg.checkInTime : "-")
                << endl;

            // Show purchases
            cout << setw(15) << "\n" << "Purchased Items: ";
            if (purchases.count(reg.registrationID) && !purchases[reg.registrationID].empty()) {
                for (size_t i = 0; i < purchases[reg.registrationID].size(); ++i) {
                    if (i > 0) cout << setw(15) << "";
                    cout << purchases[reg.registrationID][i] << endl;
                }
            }
            else {
                cout << "None\n";
            }

            // Show payment method
            cout << setw(15) << "" << "Payment Method: ";
            if (paymentMethods.count(reg.registrationID)) {
                cout << paymentMethods[reg.registrationID] << endl;
            }
            else {
                cout << "N/A\n";
            }

            cout << string(100, '-') << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No event registrations found for this guest.\n";
    }

    // Flush buffer BEFORE showing the prompt
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nPress Enter to continue...";
    cin.get();   // smooth pause, no double-stop
    clearScreen();

}

// ========== NEW CODE FOR ADMIN ==========
void viewAllHistory() {
    loadRegistrationFromFile();

    cout << "\n" << string(50, '=') << endl;
    cout << "        FULL EVENT HISTORY (ADMIN)" << endl;
    cout << string(50, '=') << endl << endl;

    if (registrations.empty()) {
        cout << "No registration records found.\n";
        return;
    }

    // Print all guest histories (no filtering)
    for (const auto& reg : registrations) {
        cout << "Guest ID: " << reg.guestID << endl;
        cout << "Registration ID: " << reg.registrationID << endl;
        cout << "Event: " << reg.eventName << " on " << reg.eventDate << endl;
        cout << "Tickets: " << reg.ticketsBought << " | Cost: RM " << reg.registrationCost << endl;
        cout << "Checked In: " << (reg.checkedIn ? "Yes" : "No")
            << " | Time: " << (reg.checkedIn ? reg.checkInTime : "-") << endl;
        cout << string(50, '-') << endl;
    }

    // Flush buffer BEFORE showing the prompt
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nPress Enter to continue...";
    cin.get();   // smooth pause, no double-stop
    clearScreen();


}

void adminHistoryMenu() {
    int choice;
    do {
        cout << "\n===== ADMIN HISTORY MENU =====" << endl;
        cout << "1. View All Guest Histories" << endl;
        cout << "2. Filter by Guest ID" << endl;
        cout << "0. Back" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            viewAllHistory();
        }
        else if (choice == 2) {
            string guestID;
            cout << "Enter Guest ID: ";
            getline(cin, guestID);
            viewUserHistory(guestID);
        }
    } while (choice != 0);
}
