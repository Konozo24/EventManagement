#include "Venue.h"
#include "Utils.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;


vector<Venue> venues;

namespace {
    
    int nextVenueID = 1;
}



Venue::Venue() : venueID("0"), name(""), capacity(0), location(""), cost(0.0), isBooked(false), usageCount(0) {}

Venue::Venue(const string& id, const string& venueName, int cap, const string& loc, double venueCost)
    : venueID(id), name(venueName), capacity(cap), location(loc), cost(venueCost), isBooked(false), usageCount(0) {
}

void initializeDefaultVenues() {
    venues.clear();
    nextVenueID = 1; // reset when creating defaults

    venues.push_back(Venue("V" + to_string(nextVenueID++), "Skyline Grand Ballroom", 400, "Downtown City Center", 18000.0));
    venues.push_back(Venue("V" + to_string(nextVenueID++), "Innovation Hub Conference Hall", 250, "Tech Business Park", 12000.0));
    venues.push_back(Venue("V" + to_string(nextVenueID++), "Garden Pavilion", 300, "Botanical Gardens", 14000.0));
    venues.push_back(Venue("V" + to_string(nextVenueID++), "Executive Boardroom", 50, "Corporate Tower", 6000.0));
    venues.push_back(Venue("V" + to_string(nextVenueID++), "University Auditorium", 600, "City University Campus", 20000.0));
    venues.push_back(Venue("V" + to_string(nextVenueID++), "Rooftop Terrace", 150, "Luxury Hotel Downtown", 10000.0));
    venues.push_back(Venue("V" + to_string(nextVenueID++), "Tech Expo Hall", 800, "Convention Center", 25000.0));

    saveVenuesToFile();
}

void loadVenuesFromFile() {
    ifstream file(VENUES_FILE);
    if (!file.is_open()) {
        cout << "Creating new venues file with default venues..." << endl;
        initializeDefaultVenues();
        return;
    }

    venues.clear();
    string line;
    nextVenueID = 1;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string item;
        vector<string> tokens;

        while (getline(ss, item, '|')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 6) {
            Venue venue;
            venue.venueID = tokens[0];
            venue.name = tokens[1];
            venue.capacity = stoi(tokens[2]);
            venue.location = tokens[3];
            venue.cost = stod(tokens[4]);
            venue.isBooked = (tokens[5] == "1");
            venue.usageCount = (tokens.size() > 6) ? stoi(tokens[6]) : 0;
            venues.push_back(venue);

            // Extract numeric part of ID to update nextVenueID
            if (venue.venueID.size() > 1 && (venue.venueID[0] == 'V' || venue.venueID[0] == 'v')) {
                int idNum = stoi(venue.venueID.substr(1));
                if (idNum >= nextVenueID) {
                    nextVenueID = idNum + 1;
                }
            }
        }
    }
    file.close();
}

void saveVenuesToFile() {
    ofstream file(VENUES_FILE);
    if (!file.is_open()) {
        cout << "Error: Cannot save venues to file!" << endl;
        return;
    }

    for (const auto& venue : venues) {
        file << venue.venueID << "|"
            << venue.name << "|"
            << venue.capacity << "|"
            << venue.location << "|"
            << venue.cost << "|"
            << (venue.isBooked ? 1 : 0) << "|"
            << venue.usageCount << endl;
    }
    file.close();
}

void displayAvailableVenues() {
    cout << "\n" << string(80, '=') << endl;
    cout << "                      AVAILABLE VENUES" << endl;
    cout << string(80, '=') << endl;
    cout << left << setw(5) << "ID"
        << setw(40) << "Venue Name"
        << setw(10) << "Capacity"
        << setw(30) << "Location"
        << setw(15) << "Cost (RM)"
        << setw(10) << "Status" << endl;
    cout << string(80, '-') << endl;

    bool hasAvailable = false;
    for (const auto& venue : venues) {
        if (!venue.isBooked) {
            cout << left << setw(6) << venue.venueID
                << setw(40) << venue.name
                << setw(10) << venue.capacity
                << setw(30) << venue.location
                << "RM" << setw(10) << fixed << setprecision(2) << venue.cost
                << setw(10) << "Available" << endl;
            hasAvailable = true;
        }
    }

    if (!hasAvailable) {
        cout << "No venues are currently available for booking." << endl;
    }
    cout << string(80, '=') << endl;
}

void displayAllVenues() {
    cout << "\n" << string(80, '=') << endl;
    cout << "                      VENUES" << endl;
    cout << string(80, '=') << endl;
    cout << left << setw(5) << "ID"
        << setw(40) << "Venue Name"
        << setw(10) << "Capacity"
        << setw(30) << "Location"
        << setw(15) << "Cost (RM)"
        << setw(10) << "Status" << endl;
    cout << string(80, '-') << endl;

    for (const auto& venue : venues) {
        cout << left << setw(6) << venue.venueID
            << setw(40) << venue.name
            << setw(10) << venue.capacity
            << setw(30) << venue.location
            << "RM" << setw(10) << fixed << setprecision(2) << venue.cost
            << setw(10) << (venue.isBooked ? "Booked" : "Available") << endl;
    }

    cout << string(80, '=') << endl;
}


// Check if venue is available (for other modules to use)
bool isVenueAvailable(const string& venueID) {
    loadVenuesFromFile();
    for (const auto& venue : venues) {
        if (venue.venueID == venueID && !venue.isBooked) {
            return true;
        }
    }
    return false;
}

// Get venue details (for other modules to use)
Venue getVenueDetails(const string& venueID) {
    loadVenuesFromFile();
    for (const auto& venue : venues) {
        if (venue.venueID == venueID) {
            return venue;
        }
    }
    return Venue(); // Return empty venue if not found
}

bool isValidVenueFormat(const string& input) {
    if (input.size() < 2 || toupper(input[0]) != 'V') {
        return false;
    }
    for (size_t i = 1; i < input.size(); i++) {
        if (!isdigit(input[i])) return false;
    }
    return true;
}

// Validate venue selection - Your input validation
string validateVenueSelection() {
    string input;
    bool validInput = false;

    do {
        cout << "Enter Venue ID to book (e.g., V1) or 0 to cancel: ";
        getline(cin, input);

        if (input == "0") {
            clearScreen();
            return "0";
        }

        if (!isValidVenueFormat(input)) {
            cout << "Invalid format! Please enter Venue ID like V1, V2...\n";
            continue;
        }

        
        bool found = false;
        for (const auto& venue : venues) {
            if (venue.venueID == input) {
                found = true;
                if (venue.isBooked) {
                    cout << "Sorry, this venue is already booked. Please select another venue." << endl;
                    break;
                }
                else {
                    validInput = true;
                    break;
                }
            }
        }

        if (!found) {
            cout << "Venue ID not found! Please enter a valid Venue ID." << endl;
        }

    } while (!validInput);

    return input;
}

string validateBookedVenueSelection() {
    string input;
 
    bool validInput = false;

    do {
        cout << "Enter Venue ID to mark as available (e.g., V1) or 0 to cancel: ";
        getline(cin, input);

        if (input == "0") {
            clearScreen();
            return "0";
        }

        if (!isValidVenueFormat(input)) {
            cout << "Invalid format! Please enter Venue ID like V1, V2...\n";
            continue;
        }

        bool found = false;
        for (const auto& venue : venues) {
            if (venue.venueID == input) {
                found = true;
                if (!venue.isBooked) {
                    cout << "Venue is already available. Please select a booked venue." << endl;
                    break;
                }
                else {
                    validInput = true;
                    break;
                }
            }
        }

        if (!found) {
            cout << "Venue ID not found! Please enter a valid Venue ID." << endl;
        }

    } while (!validInput);

    return input;
}
