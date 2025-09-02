#include "Registration.h"
#include "Constants.h"
#include "Event.h"
#include "Utils.h"
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

vector<Registration> registrations;
static int nextRegistrationID = 1; // auto increment counter

// Constructors
Registration::Registration()
    : registrationID(""), eventID(""), eventName(""), eventDate(""),
    guestID(""), userName(""), ticketsBought(0), registrationCost(0.0),
    checkedIn(false), checkInTime("") {
}
Registration::Registration(const string& regId, const string& evId, const string& evName,
    const string& evDate, const string& gID, const string& uName,
    int tickets, double cost, bool checkin, const string& timestamp)
    : registrationID(regId), eventID(evId), eventName(evName), eventDate(evDate),
    guestID(gID), userName(uName), ticketsBought(tickets), registrationCost(cost),
    checkedIn(checkin), checkInTime(timestamp) {
}


void loadRegistrationFromFile() {
    ifstream file(REGISTRATION_FILE);
    if (!file.is_open()) {
        // File doesn't exist yet
        registrations.clear();
        nextRegistrationID = 1;
        return;
    }

    registrations.clear();
    string line;
    int maxID = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string regId, evId, evName, evDate, gID, userName, tickets, cost, checkinFlag, checkinTime;

        getline(ss, regId, '|');
        getline(ss, evId, '|');
        getline(ss, evName, '|');
        getline(ss, evDate, '|');
        getline(ss, gID, '|');
        getline(ss, userName, '|');
        getline(ss, tickets, '|');
        getline(ss, cost, '|');
        getline(ss, checkinFlag, '|');
        getline(ss, checkinTime, '|');

        bool checked = (checkinFlag == "1");
        Registration reg(regId, evId, evName, evDate, gID, userName, stoi(tickets), stod(cost), checked, checkinTime);
        registrations.push_back(reg);

        // keep track of largest numeric part of regId
        if (regId.size() > 1 && regId[0] == 'R') {
            int num = stoi(regId.substr(1));
            if (num > maxID) maxID = num;
        }
    }

    file.close();

    // Update nextRegistrationID 
    nextRegistrationID = maxID + 1;
}

void saveRegistrationToFile() {
    ofstream file(REGISTRATION_FILE);
    for (const auto& reg : registrations) {
        file << reg.registrationID << "|"
            << reg.eventID << "|"
            << reg.eventName << "|"
            << reg.eventDate << "|"
            << reg.guestID << "|"
            << reg.userName << "|"
            << reg.ticketsBought << "|"
            << fixed << setprecision(2) << reg.registrationCost << "|"
            << (reg.checkedIn ? "1" : "0") << "|"
            << reg.checkInTime
            << endl;
    }
    file.close();
}

void displayRegistration() {
    cout << "\n" << string(80, '=') << endl;
    cout << "                   REGISTRATIONS" << endl;
    cout << string(80, '=') << endl;
    cout << left << setw(8) << "RegID"
        << setw(6) << "EvID"
        << setw(20) << "Event Name"
        << setw(12) << "Date"
        << setw(10) << "GuestID"
        << setw(12) << "Guest"
        << setw(8) << "Tickets"
        << setw(10) << "Cost(RM)"
        << setw(8) << "Checked In"
        << setw(20) << "Check-in Time" << endl;
    cout << string(100, '-') << endl;

    for (const auto& reg : registrations) {
        cout << left << setw(8) << reg.registrationID
            << setw(6) << reg.eventID
            << setw(20) << reg.eventName
            << setw(12) << reg.eventDate
            << setw(10) << reg.guestID
            << setw(12) << reg.userName
            << setw(8) << reg.ticketsBought
            << setw(10) << fixed << setprecision(2) << reg.registrationCost
            << setw(10) << (reg.checkedIn ? "Yes" : "No")
            << setw(20) << (reg.checkedIn ? reg.checkInTime : "-")
            << endl;
       
    }
    cout << string(100, '=') << endl;
}

string generateRegistrationID() {
  
    string id = "R" + to_string(nextRegistrationID++);
    return id;
}

void viewUserHistory(const string& guestID) {
    loadRegistrationFromFile();

    cout << "\n" << string(50, '=') << endl;
    cout << "        EVENT HISTORY FOR GUEST: " << guestID << endl;
    cout << string(50, '=') << endl << endl;

    // ===== Step 1: Read receipt.txt once into a map =====
    ifstream file("receipt.txt");
    map<string, vector<string>> purchases; // regID -> items
    map<string, string> paymentMethods;    // regID -> payment method

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
                    // trim spaces
                    currentRegID.erase(0, currentRegID.find_first_not_of(" \t"));
                }
            }

            if (!currentRegID.empty()) {
                if (line.find("Product") != string::npos &&
                    line.find("Product Total") == string::npos) // ignore totals
                {
                    size_t pos = line.find(":");
                    if (pos != string::npos) {
                        string item = line.substr(pos + 1);
                        item.erase(0, item.find_first_not_of(" \t")); // trim left spaces

                        if (item == "None") {
                            purchases[currentRegID].push_back("None");
                        }
                        else if (!item.empty()) {
                            purchases[currentRegID].push_back(item);
                        }
                    }
                }
                else if (productSection) {
                    if (line.find("Ticket Price") != string::npos ||
                        line.find("Product Total") != string::npos ||
                        line.find("Total") != string::npos) {
                        productSection = false;
                    }
                    else {
                        purchases[currentRegID].push_back(line);
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
                    if (i > 0) cout << setw(15) << ""; // indent for subsequent items
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

    cout << "\nPress Enter to continue...";
    cin.get();
    clearScreen();
}

string validateRegistrationIDInput() {
    string regID;
    bool valid = false;

    do {
        cout << "\nEnter Registration ID (or 0 to cancel): ";
        getline(cin, regID);

        if (regID == "0") {
            return "0";  // cancel
        }

        // search in registrations
        if (regID.size() >= 2 && regID[0] == 'R') {
            valid = true;
            for (size_t i = 1; i < regID.size(); ++i) {
                if (!isdigit(regID[i])) {
                    valid = false;
                    break;
                }
            }
        }

        // If format correct, also check if it actually exists
        if (valid) {
            valid = false;
            for (const auto& reg : registrations) {
                if (reg.registrationID == regID) {
                    valid = true;
                    break;
                }
            }
        }

        if (!valid) {
            cout << "Invalid Registration ID. Please try again.\n";
        }

    } while (!valid);

    return regID;
}
