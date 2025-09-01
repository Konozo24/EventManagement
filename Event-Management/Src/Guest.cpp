#include "Guest.h"
#include "Constants.h"
#include "Registration.h"
#include "Event.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>
using namespace std;


vector<Guest> guests;

Guest::Guest() : guestID("0"), name(""), eventName(""), checkedIn(false), checkInTime("") {}
Guest::Guest(const string& id, const string& guestName, const string& event)
    : guestID(id), name(guestName), eventName(event), checkedIn(false), checkInTime("") {
}

void Guest::checkIn() {
    checkedIn = true;
    time_t now = time(0);
    char buf[26];
    ctime_s(buf, sizeof(buf), &now);
    checkInTime = string(buf);
    if (!checkInTime.empty() && checkInTime.back() == '\n')
        checkInTime.pop_back();
}

// Load guests from guest file
void loadGuestsFromFile() {
    ifstream file(GUESTS_FILE);
    if (!file.is_open()) {
        return; // File doesn't exist yet
    }

    guests.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string item;
        vector<string> tokens;

        while (getline(ss, item, '|')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 3) {
            Guest guest;
            guest.guestID = tokens[0];
            guest.name = tokens[1];
            guest.eventName = tokens[2];
            guest.checkedIn = (tokens.size() > 3) ? (tokens[3] == "1") : false;
            guest.checkInTime = (tokens.size() > 4) ? tokens[4] : "";
            guests.push_back(guest);
        }
    }
    file.close();
}

// Save guests to file
void saveGuestsToFile() {
    ofstream file(GUESTS_FILE);
    if (!file.is_open()) {
        cout << "Error saving guests to file.\n";
        return;
    }

    for (const auto& guest : guests) {
        file << guest.guestID << "|"
            << guest.name << "|"
            << guest.eventName << "|"
            << (guest.checkedIn ? "1" : "0") << "|"
            << guest.checkInTime
            << "\n";
    }

    file.close();
}

// Display registered guests in formatted table
void displayRegisteredGuests() {
    cout << "\n" << string(80, '=') << endl;
    cout << "                    REGISTERED GUESTS" << endl;
    cout << string(80, '=') << endl;

    if (guests.empty()) {
        cout << "No guests are currently registered for any events." << endl;
        cout << string(80, '=') << endl;
        return;
    }

    cout << left << setw(10) << "Guest ID"
        << setw(20) << "Name"
        << setw(25) << "Event"
        << setw(12) << "Status"
        << setw(20) << "Check-in Time" << endl;
    cout << string(80, '-') << endl;

    for (const auto& reg : registrations) {
        Guest* g = findGuestByID(reg.guestID);
        Event* e = findEventByID(reg.eventID);

        if (g) {
            cout << left << setw(10) << g->guestID
                << setw(20) << g->name
                << setw(25) << (e ? e->eventName : "(Event not found)")
                << setw(12) << (g->checkedIn ? "Checked In" : "Not Checked")
                << setw(20) << g->checkInTime << endl;
        }
    }
    cout << string(80, '=') << endl;
}
// Validate guest ID input
string validateGuestIDInput() {
    string guestID;
    bool validInput = false;

    do {
        cout << "Enter Guest ID to check in (0 to return to menu): ";
        getline(cin, guestID);

        if (guestID == "0") return "0"; // exit signal

        // Simple validation: must start with 'G' and followed by digits
        if (guestID.size() >= 2 && guestID[0] == 'G') {
            validInput = true;
            for (size_t i = 1; i < guestID.size(); ++i) {
                if (!isdigit(guestID[i])) {
                    validInput = false;
                    break;
                }
            }
        }

        if (!validInput) {
            cout << "Invalid Guest ID format! Example: G1, G2, etc.\n";
        }

    } while (!validInput);

    return guestID;
}

// Verify if Guest ID and Name match an existing guest
bool verifyGuest(const string& guestID, const string& name) {
    for (const auto& guest : guests) {
        if (guest.guestID == guestID && guest.name == name) {
            return true;
        }
    }
    return false;
}

string generateGuestID() {
    loadGuestsFromFile();
    int nextID = 1;
    if (!guests.empty()) {
        try {
            nextID = stoi(guests.back().guestID.substr(1)) + 1;
        }
        catch (...) {
            nextID = guests.size() + 1; // fallback
        }
    }
    return "G" + to_string(nextID);
}

Guest* findGuestByID(const string& id) {
    for (auto& g : guests) {
        if (g.guestID == id) {
            return &g;
        }
    }
    return nullptr; // not found
}
