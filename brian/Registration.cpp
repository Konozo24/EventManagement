#include "Registration.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

vector<Registration> registrations;

Registration::Registration() : registrationID(0), registrationName(""), ticketAmount(0), registrationCost(0.0) {}

Registration::Registration(int regId, const string& regName, int ticAmt, double regCost)
    : registrationID(regId), registrationName(regName), ticketAmount(ticAmt), registrationCost(regCost) {
}

void initializeDefaultRegistration() {
    registrations.clear();

    registrations.push_back(Registration(1, "iPhone 28 Launch", 800, 100.00));
    registrations.push_back(Registration(2, "Samsung Galaxy S30 Launch", 600, 80.00));
    registrations.push_back(Registration(3, "Sony Headphones Launch", 300, 50.00));
    registrations.push_back(Registration(4, "PlayStation 6 Launch", 400, 90.00));
    registrations.push_back(Registration(5, "Nintendo Switch 3 Launch", 250, 70.00));
    registrations.push_back(Registration(6, "Yamaha Bike Launch", 150, 30.00));
    registrations.push_back(Registration(7, "Daikin Air Conditioner Launch", 50, 20.00));
    saveRegistrationToFile();
}

void loadRegistrationFromFile() {
    ifstream file(REGISTRATION_FILE);
    if (!file.is_open()) {
        cout << "Creating new registration file with default events..." << endl;
        initializeDefaultRegistration();
        return;
    }

    registrations.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, name, tickets, cost, guestList;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, tickets, '|');
        getline(ss, cost, '|');
        getline(ss, guestList, '|');

        Registration reg;
        reg.registrationID = stoi(id);
        reg.registrationName = name;
        reg.ticketAmount = stoi(tickets);
        reg.registrationCost = stod(cost);

        // Load guest IDs if present
        reg.guestIDs.clear();
        stringstream gs(guestList);
        string gID;
        while (getline(gs, gID, ',')) {
            if (!gID.empty()) reg.guestIDs.push_back(stoi(gID));
        }

        registrations.push_back(reg);
    }
    file.close();
}

void saveRegistrationToFile() {
    ofstream file(REGISTRATION_FILE);
    for (const auto& reg : registrations) {
        file << reg.registrationID << "|"
            << reg.registrationName << "|"
            << reg.ticketAmount << "|"
            << reg.registrationCost << "|";

        // Save guest IDs as comma-separated
        for (size_t i = 0; i < reg.guestIDs.size(); i++) {
            file << reg.guestIDs[i];
            if (i != reg.guestIDs.size() - 1) file << ",";
        }

        file << endl;
    }
    file.close();
}

void displayRegistration() {
    cout << "\n" << string(60, '=') << endl;
    cout << "                   AVAILABLE EVENTS" << endl;
    cout << string(60, '=') << endl;
    cout << left << setw(5) << "ID"
        << setw(35) << "Event Name"
        << setw(10) << "Tickets"
        << setw(10) << "Cost (RM)" << endl;
    cout << string(60, '-') << endl;

    for (const auto& reg : registrations) {
        if (reg.ticketAmount > 0) {
            cout << left << setw(5) << reg.registrationID
                << setw(35) << reg.registrationName
                << setw(10) << reg.ticketAmount
                << "RM" << fixed << setprecision(2) << reg.registrationCost << endl;
        }
    }
    cout << string(60, '=') << endl;
}

Registration* findRegistrationByName(const string& name) {
    for (auto& reg : registrations) {
        if (reg.registrationName == name) {
            return &reg;
        }
    }
    return nullptr;
}
