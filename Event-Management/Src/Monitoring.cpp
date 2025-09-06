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

void displayCheckInStatistics(RegistrationManager& regManager) {
	

    regManager.loadRegistrationFromFile();
    int totalRegistrations = regManager.getRegistrations().size();
    int checkedInCount = 0;

    for (const auto& reg : regManager.getRegistrations()) {
        if (reg.checkedIn) checkedInCount++;
    }

    cout << "\n" << string(50, '-') << endl;
    cout << "CHECK-IN STATISTICS" << endl;
    cout << string(50, '-') << endl;
    cout << "Total Registered Guests : " << totalRegistrations << endl;
    cout << "Guests Checked In       : " << checkedInCount << endl;
    cout << "Guests Not Checked In   : " << (totalRegistrations - checkedInCount) << endl;
    cout << string(50, '-') << endl;
}

// Display all registered guests per event
void displayRegisteredGuests(RegistrationManager& regManager, EventManager& eventManager, GuestManager& guestManager) {
    regManager.loadRegistrationFromFile();
    guestManager.loadGuestsFromFile();
    eventManager.loadEventsFromFile();

    cout << "\n" << string(100, '=') << endl;
    cout << "                REGISTERED GUESTS (All Events)" << endl;
    cout << string(100, '=') << endl;

    if (regManager.getRegistrations().empty()) {
        cout << "No registrations found.\n";
        cout << string(100, '=') << endl;
        return;
    }

    cout << left << setw(8) << "RegID"
        << setw(10) << "GuestID"
        << setw(20) << "Guest Name"
        << setw(20) << "Event"
        << setw(10) << "Tickets"
        << setw(12) << "Status"
        << setw(25) << "Check-in Time" << endl;
    cout << string(100, '-') << endl;

    for (const auto& reg : regManager.getRegistrations()) {
        Guest* g = guestManager.findGuestByID(reg.guestID);
        Event* e = eventManager.findEventByID(reg.eventID);

        cout << left << setw(8) << reg.registrationID
            << setw(10) << reg.guestID
            << setw(20) << (g ? g->name : "(Unknown)")
            << setw(20) << (e ? e->eventName : "(Unknown)")
            << setw(10) << reg.ticketsBought
            << setw(12) << (reg.checkedIn ? "Checked In" : "Not Checked")
            << setw(25) << (reg.checkedIn ? reg.checkInTime : "-")
            << endl;
    }

    cout << string(100, '=') << endl;
}

void markVenueAsAvailable(VenueManager& venueManager, EventManager& eventManager) {
	  

    venueManager.loadVenuesFromFile();
    eventManager.loadEventsFromFile();

    if (venueManager.getVenues().empty()) {
        cout << "No venues found.\n";
        return;
    }

    venueManager.displayAllVenues();

    string venueID;
    bool success = false;

    do {
        venueID = venueManager.validateBookedVenueSelection(); // returns 0 if user cancels
        if (venueID == "0") {
            cout << "Cancelled. Returning to menu...\n";
            return;
        }

        // Find the venue
        for (auto& venue : venueManager.getVenues()) {
            if (venue.venueID == venueID) {
                if (!venue.isBooked) {
                    cout << "Venue " << venue.name << " is already available.\n";
                }
                else {
                    venue.isBooked = false; // mark as available

                    
                    eventManager.getEvents().erase(
                        remove_if(eventManager.getEvents().begin(), eventManager.getEvents().end(),
                            [&](const Event& ev) { return ev.venueID == venueID; }),
                        eventManager.getEvents().end()
                    );

                    venueManager.saveVenuesToFile();
                    eventManager.saveEventsToFile();

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
	RegistrationManager regManager;
	EventManager eventManager;
	VenueManager venueManager;
	GuestManager guestManager;

    int choice;

    do {
        clearScreen();

        guestManager.loadGuestsFromFile();
        eventManager.loadEventsFromFile();
        regManager.loadRegistrationFromFile();

        // Display current guest status
        displayRegisteredGuests(regManager, eventManager, guestManager);

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
            string regID = regManager.validateRegistrationIDInput();
            if (regID == "0") break;  // user cancelled

            bool found = false;
            for (auto& reg : regManager.getRegistrations()) {
                if (reg.registrationID == regID) {
                    found = true;

                    Guest* g = guestManager.findGuestByID(reg.guestID);
                    Event* e = eventManager.findEventByID(reg.eventID);

                    if (!e) {
                        cout << "\nCannot check in. The event for this registration no longer exists (venue was freed).\n";
                        break;
                    }

                    if (reg.checkedIn) {
                        cout << "\nGuest " << (g ? g->name : "(Unknown)")
                            << " (ID: " << reg.guestID
                            << ") is already checked in at " << reg.checkInTime << endl;
                    }
                    else {
                        // Mark as checked in
                        reg.checkedIn = true;
                        time_t now = time(0);
                        char buf[26];
                        ctime_s(buf, sizeof(buf), &now);
                        reg.checkInTime = string(buf);
                        if (!reg.checkInTime.empty() && reg.checkInTime.back() == '\n')
                            reg.checkInTime.pop_back();

                        regManager.saveRegistrationToFile();

                        cout << "\n" << string(50, '*') << endl;
                        cout << "CHECK-IN SUCCESSFUL!" << endl;
                        cout << "Guest: " << (g ? g->name : "(Unknown)") << endl;
                        cout << "Guest ID: " << reg.guestID << endl;
                        if (e) cout << "Event: " << e->eventName << endl;
                        cout << "Time: " << reg.checkInTime << endl;
                        cout << string(50, '*') << endl;

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
            displayCheckInStatistics(regManager);
            cout << "\nPress Enter to continue...";
            cin.get();
            break;

        case 4:
            // Mark venue as available
            clearScreen();
            markVenueAsAvailable(venueManager, eventManager);
            cout << "\nPress Enter to continue...";
            cin.get();
            break;

        case 0:
            cout << "Returning to main menu..." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            break;

        default:
            cout << "Invalid choice! Please select 0-3." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 0);
}


