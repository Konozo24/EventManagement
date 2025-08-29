#include "Tickets.h"
#include "Registration.h"
#include "Utils.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

void tickets() {
    cout << "\n" << string(60, '=') << endl;
    cout << "               EVENT REGISTRATION SYSTEM" << endl;
    cout << string(60, '=') << endl;

    // Load events from file
    loadRegistrationFromFile();

    Registration* reg = nullptr;
    string eventName;

    // Display all available events once at the start
    displayRegistration();

    // Prompt user for a valid event
    while (!reg) {
        cout << "\nEnter Event Name (or 'cancel' to exit): ";
        getline(cin, eventName);

        if (eventName == "cancel") {
            clearScreen();
            return;
        }

        reg = findRegistrationByName(eventName);
        if (!reg) {
            cout << "Invalid event name! Please try again.\n";
        }
    }

    // Prompt user for ticket quantity
    int ticketsRequested;
    while (true) {
        cout << "Enter number of tickets: ";
        if (!(cin >> ticketsRequested)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        cin.ignore();

        if (ticketsRequested <= 0) {
            cout << "Ticket amount must be greater than 0!\n";
        }
        else if (ticketsRequested > reg->ticketAmount) {
            cout << "Error: Not enough tickets available! Only "
                << reg->ticketAmount << " left.\n";
        }
        else {
            break;
        }
    }

    // Deduct tickets and save
    reg->ticketAmount -= ticketsRequested;
    saveRegistrationToFile();

    // Clear screen before showing confirmation
    clearScreen();

    // Display confirmation
    cout << "\n" << string(60, '=') << endl;
    cout << "              REGISTRATION CONFIRMATION" << endl;
    cout << string(60, '=') << endl;
    cout << "Event:        " << reg->registrationName << endl;
    cout << "Tickets:      " << ticketsRequested << endl;
    cout << "Total Cost:   RM" << fixed << setprecision(2)
        << reg->registrationCost * ticketsRequested << endl;
    cout << string(60, '=') << endl;

    cout << "\nPress Enter to continue...";
    cin.get();
    clearScreen();
}
