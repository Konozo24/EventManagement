#include <iostream>
#include "Guest.h"
#include "Event.h"
#include "Venue.h"
#include "Utils.h"
#include "Constants.h"
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>


using namespace std;

void displayCheckInStatistics() {
    int totalGuests = guests.size();
    int checkedInCount = 0;

    for (const auto& g : guests) {
        if (g.checkedIn) checkedInCount++;
    }

    cout << "\n" << string(50, '-') << endl;
    cout << "CHECK-IN STATISTICS" << endl;
    cout << string(50, '-') << endl;
    cout << "Total Registered Guests : " << totalGuests << endl;
    cout << "Guests Checked In       : " << checkedInCount << endl;
    cout << "Guests Not Checked In   : " << (totalGuests - checkedInCount) << endl;
    cout << string(50, '-') << endl;
}

void markVenueAsAvailable() {
    loadVenuesFromFile();
    loadEventsFromFile();

    if (venues.empty()) {
        cout << "No venues found.\n";
        return;
    }

    displayAllVenues();

    string venueID;
    bool success = false;

    do {
        venueID = validateBookedVenueSelection(); // returns 0 if user cancels
        if (venueID == "0") {
            cout << "Cancelled. Returning to menu...\n";
            return;
        }

        // Find the venue
        for (auto& venue : venues) {
            if (venue.venueID == venueID) {
                if (!venue.isBooked) {
                    cout << "Venue " << venue.name << " is already available.\n";
                }
                else {
                    venue.isBooked = false; // mark as available

                    
                    events.erase(
                        remove_if(events.begin(), events.end(),
                            [&](const Event& ev) { return ev.venueID == venueID; }),
                        events.end()
                    );

                    saveVenuesToFile();
                    saveEventsToFile();

                    cout << "Venue " << venue.name << " has been marked as available.\n";
                    cout << "Associated events using this venue have been removed.\n";
                }
                success = true; // valid operation done
                break;
            }
        }

        if (!success) {
            cout << "Invalid Venue ID. Please try again.\n";
        }

    } while (!success);
}

// MAIN FUNCTION: Event Monitoring
void monitorEvent() {
    int choice;

    do {
        clearScreen();

        // Load latest guest data
        loadGuestsFromFile();

        // Display current guest status
        displayRegisteredGuests();

        // Show menu
        cout << "\nEvent Monitoring Options:" << endl;
        cout << "1. Check in a guest" << endl;
        cout << "2. Refresh guest list" << endl;
        cout << "3. View check-in statistics" << endl;
        cout << "4. Mark venue as available" << endl;
        cout << "0. Return to main menu" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\nPress Enter to continue...";
            cin.get();
            cin.get(); // consume newline
            continue;
        }
        cin.ignore();

        switch (choice) {
        case 1: {
            string guestID = validateGuestIDInput();
            if (guestID == "0") break;  // user cancelled

            bool found = false;
            for (auto& guest : guests) {
                if (guest.guestID == guestID) {
                    found = true;
                    if (guest.checkedIn) {
                        cout << "\nGuest " << guest.name << " (ID: " << guestID
                            << ") is already checked in at " << guest.checkInTime << endl;
                    }
                    else {
                        guest.checkIn();
                        saveGuestsToFile(); // save changes immediately

                        cout << "\n" << string(50, '*') << endl;
                        cout << "CHECK-IN SUCCESSFUL!" << endl;
                        cout << "Guest: " << guest.name << endl;
                        cout << "ID: " << guest.guestID << endl;
                        cout << "Event: " << guest.eventName << endl;
                        cout << "Time: " << guest.checkInTime << endl;
                        cout << string(50, '*') << endl;
                    }
                    break;
                }
            }

            if (!found) {
                cout << "\nGuest ID " << guestID << " not found!" << endl;
            }

            cout << "\nPress Enter to continue...";
            cin.get();
            break;
        }

        case 2: // Refresh guest list
            cout << "\nGuest list refreshed." << endl;
            cout << "Press Enter to continue...";
            cin.get();
            break;

        case 3:
            displayCheckInStatistics();
            cout << "\nPress Enter to continue...";
            cin.get();
            break;

        case 4:
            // Mark venue as available
            clearScreen();
            markVenueAsAvailable();
            cout << "\nPress Enter to continue...";
            cin.get();
            break;

        case 0:
            cout << "Returning to main menu..." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            break;

        default:
            cout << "Invalid choice! Please select 0-3." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 0);
}


