#include "Registration.h"
#include "Constants.h"
#include "Event.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

vector<Registration> registrations;
static int nextRegistrationID = 1; // auto increment counter

// Constructors
Registration::Registration()
    : registrationID(""), eventID(""), eventName(""), eventDate(""),
    userName(""), ticketsBought(0), registrationCost(0.0) {
}

Registration::Registration(const string& regId, const string& evId, const string& evName,
    const string& evDate, const string& uName,
    int tickets, double cost)
    : registrationID(regId), eventID(evId), eventName(evName),
    eventDate(evDate), userName(uName),
    ticketsBought(tickets), registrationCost(cost) {
}


// Initialize dummy events if file not found
void initializeDefaultRegistration() {
    registrations.clear();

    registrations.push_back(Registration("R" + to_string(nextRegistrationID++), "E1", "Concert A", "10-12-2025", "Alice", 2, 200.0));
    registrations.push_back(Registration("R" + to_string(nextRegistrationID++), "E2", "Concert B", "11-12-2025", "Bob", 1, 120.0));
    registrations.push_back(Registration("R" + to_string(nextRegistrationID++), "E3", "Concert C", "12-12-2025", "Charlie", 3, 450.0));

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
    int maxID = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string regId, evId, evName, evDate, userName, tickets, cost;

        getline(ss, regId, '|');
        getline(ss, evId, '|');
        getline(ss, evName, '|');
        getline(ss, evDate, '|');
        getline(ss, userName, '|');
        getline(ss, tickets, '|');
        getline(ss, cost, '|');

        Registration reg(regId, evId, evName, evDate, userName, stoi(tickets), stod(cost));
        registrations.push_back(reg);

        // keep track of largest numeric part of regId
        if (regId.size() > 1 && regId[0] == 'R') {
            int num = stoi(regId.substr(1));
            if (num > maxID) maxID = num;
        }
    }

    file.close();
}

void saveRegistrationToFile() {
    ofstream file(REGISTRATION_FILE);
    for (const auto& reg : registrations) {
        file << reg.registrationID << "|"
            << reg.eventID << "|"
            << reg.eventName << "|"
            << reg.eventDate << "|"
            << reg.userName << "|"
            << reg.ticketsBought << "|"
            << fixed << setprecision(2) << reg.registrationCost
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
        << setw(12) << "Guest"
        << setw(8) << "Tickets"
        << setw(10) << "Cost(RM)" << endl;
    cout << string(80, '-') << endl;

    for (const auto& reg : registrations) {
        cout << left << setw(8) << reg.registrationID
            << setw(6) << reg.eventID
            << setw(20) << reg.eventName
            << setw(12) << reg.eventDate
            << setw(12) << reg.userName
            << setw(8) << reg.ticketsBought
            << setw(10) << fixed << setprecision(2) << reg.registrationCost
            << endl;
    }
    cout << string(80, '=') << endl;
}

string generateRegistrationID() {
    // Caller should have called loadRegistrationFromFile() earlier so nextRegistrationID is correct.
    string id = "R" + to_string(nextRegistrationID++);
    return id;
}
