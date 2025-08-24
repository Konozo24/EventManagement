#include <iostream>
#include "Guest.h"
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

// MAIN FUNCTION: Event Monitoring
void monitorEvent() {
    cout << "\n" << string(70, '=') << endl;
    cout << "                EVENT MONITORING SYSTEM" << endl;
    cout << string(70, '=') << endl;

    // Load latest guest data
    loadGuestsFromFile();

    // Display current guest status
    displayRegisteredGuests();

    if (guests.empty()) {
        cout << "\nPress Enter to return to main menu...";
        cin.get();
        clearScreen();
        return;
    }

    // Event monitoring menu loop
    int choice;
    do {
        cout << "\nEvent Monitoring Options:" << endl;
        cout << "1. Check in a guest" << endl;
        cout << "2. Refresh guest list" << endl;
        cout << "3. View check-in statistics" << endl;
        cout << "0. Return to main menu" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        switch (choice) {
        case 1: {
            int guestID = validateGuestIDInput();
            if (guestID == 0) break;

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

                        // Log check-in activity
                        ofstream logFile(CHECKIN_LOG_FILE, ios::app);
                        if (logFile.is_open()) {
                            logFile << guestID << "|" << guest.name << "|"
                                << guest.eventName << "|" << guest.checkInTime << endl;
                            logFile.close();
                        }

                        cout << "\n" << string(50, '*') << endl;
                        cout << "CHECK-IN SUCCESSFUL!" << endl;
                        cout << "Guest: " << guest.name << endl;
                        cout << "ID: " << guestID << endl;
                        cout << "Event: " << guest.eventName << endl;
                        cout << "Time: " << guest.checkInTime << endl;
                        cout << string(50, '*') << endl;
                    }
                    break;
                }
            }

            if (!found) {
                cout << "\nGuest ID " << guestID << " not found in the registration list!" << endl;
                cout << "Please verify the ID and try again." << endl;
            }
            break;
        }
        case 2:
            loadGuestsFromFile();
            displayRegisteredGuests();
            break;
        case 3:
            displayCheckInStatistics();
            break;
        case 0:
            cout << "Returning to main menu..." << endl;
            clearScreen();
            break;
        default:
            cout << "Invalid choice! Please select 0-3." << endl;
        }

    } while (choice != 0);


}
