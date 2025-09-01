#include <iostream>
#include "Guest.h"
#include "Event.h"
#include "Venue.h"
#include "Utils.h"
#include "Registration.h"
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

        loadGuestsFromFile();
        loadEventsFromFile();
        loadRegistrationFromFile();

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
            string regID = validateRegistrationIDInput();
            if (regID == "0") break;  // user cancelled

            bool found = false;
            for (auto& reg : registrations) {
                if (reg.registrationID == regID) {
                    found = true;

                    Guest* g = findGuestByID(reg.guestID);
                    Event* e = findEventByID(reg.eventID);

                    if (g) {
                        if (g->checkedIn) {
                            cout << "\nGuest " << g->name << " (ID: " << g->guestID
                                << ") is already checked in at " << g->checkInTime << endl;
                        }
                        else {
                            g->checkIn();
                            saveGuestsToFile(); // save changes immediately

                            cout << "\n" << string(50, '*') << endl;
                            cout << "CHECK-IN SUCCESSFUL!" << endl;
                            cout << "Guest: " << g->name << endl;
                            cout << "Guest ID: " << g->guestID << endl;
                            if (e) cout << "Event: " << e->eventName << endl;
                            cout << "Time: " << g->checkInTime << endl;
                            cout << string(50, '*') << endl;
                        }
                    }
                    else {
                        cout << "\nGuest not found for Registration ID " << reg.registrationID << endl;
                    }
                    
                    break;


                }


            }

            if (!found) {
                cout << "\nRegistration ID " << regID << " not found!" << endl;
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


