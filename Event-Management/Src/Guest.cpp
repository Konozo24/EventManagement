#include "Guest.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>
using namespace std;


vector<Guest> guests;

Guest::Guest() : guestID(0), name(""), eventName(""), checkedIn(false), checkInTime("") {}
Guest::Guest(int id, const string& guestName, const string& event)
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

// Load guests from registration file
void loadGuestsFromFile() {
    ifstream file(REGISTRATION_FILE);
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
            guest.guestID = stoi(tokens[0]);
            guest.name = tokens[1];
            guest.eventName = tokens[2];
            guest.checkedIn = (tokens.size() > 3) ? (tokens[3] == "1") : false;
            guest.checkInTime = (tokens.size() > 4) ? tokens[4] : "";
            guests.push_back(guest);
        }
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

    for (const auto& guest : guests) {
        cout << left << setw(10) << guest.guestID
            << setw(20) << guest.name
            << setw(25) << guest.eventName
            << setw(12) << (guest.checkedIn ? "Checked In" : "Not Checked")
            << setw(20) << guest.checkInTime << endl;
    }
    cout << string(80, '=') << endl;
}

// Validate guest ID input 
int validateGuestIDInput() {
    int guestID;
    bool validInput = false;

    do {
        cout << "Enter Guest ID to check in (0 to return to menu): ";
        if (!(cin >> guestID)) {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (guestID == 0) return 0;

        if (guestID < 0) {
            cout << "Guest ID cannot be negative!" << endl;
            continue;
        }

        validInput = true;

    } while (!validInput);

    return guestID;
}

