#include "Event.h"
#include "Constants.h"  // define EVENTS_FILE here
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

vector<Event> events;



void loadEventsFromFile() {
    ifstream file(EVENTS_FILE);
    if (!file.is_open()) {
        cout << "No events found. A new event file will be created when saving.\n";
        return;
    }

    events.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, name, date, venue, vendor, price, amount;

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, date, '|');
        getline(ss, venue, '|');
        getline(ss, vendor, '|');
        getline(ss, price, '|');
        getline(ss, amount, '|');

        Event ev;
        ev.eventID = id;
        ev.eventName = name;
        ev.eventDate = date;
        ev.venueID = venue;
        ev.vendorDetails = vendor;
        ev.ticketPrice = stod(price);
        ev.ticketAmount = stoi(amount);

        events.push_back(ev);
    }
    file.close();
}

void saveEventsToFile() {
    ofstream file(EVENTS_FILE);
    if (!file.is_open()) {
        cout << "Error saving events to file.\n";
        return;
    }

    for (const auto& ev : events) {
        file << ev.eventID << "|"
            << ev.eventName << "|"
            << ev.eventDate << "|"
            << ev.venueID << "|"
            << ev.vendorDetails << "|"
            << ev.ticketPrice << "|"
            << ev.ticketAmount << endl;
    }

    file.close();
}

void displayEvents() {
    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    cout << "\nAvailable Events:\n";
    for (const auto& ev : events) {
        cout << "[" << ev.eventID << "] " << ev.eventName
            << " | Date: " << ev.eventDate
            << " | Venue ID: " << ev.venueID
            << " | Vendor: " << ev.vendorDetails
            << " | Price: RM" << ev.ticketPrice
            << " | Tickets: " << ev.ticketAmount << endl;
    }
}

void displayEventsForRegistration() {
    cout << "\n" << string(80, '=') << endl;
    cout << "                      AVAILABLE EVENTS" << endl;
    cout << string(80, '=') << endl;
    cout << left << setw(8) << "ID"
        << setw(35) << "Event Name"
        << setw(15) << "Date"
        << setw(12) << "Price (RM)"
        << setw(10) << "Tickets" << endl;
    cout << string(80, '-') << endl;

    bool hasAvailable = false;
    for (const auto& ev : events) {
        if (ev.ticketAmount > 0) {
            cout << left << setw(8) << ev.eventID
                << setw(35) << ev.eventName
                << setw(15) << ev.eventDate
                << "RM" << setw(10) << fixed << setprecision(2) << ev.ticketPrice
                << setw(10) << ev.ticketAmount << endl;
            hasAvailable = true;
        }
    }

    if (!hasAvailable) {
        cout << "\n" << string(80, '=') << endl;
        cout << "                      AVAILABLE EVENTS" << endl;
        cout << string(80, '=') << endl;
        cout << "No events available for registration.\n";
        cout << string(80, '-') << endl;
    }

    cout << string(80, '=') << endl;
}

Event* findEventByName(const string& name) {
    for (auto& ev : events) {
        if (ev.eventName == name) {
            return &ev;
        }
    }
    return nullptr; // Not found
}
