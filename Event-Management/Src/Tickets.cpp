#include "Tickets.h"
#include "Registration.h"
#include "Utils.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
using namespace std;

// Function to generate unique guest IDs
vector<int> generateGuestIDs(int lastID, int numTickets) {
    vector<int> ids;
    for (int i = 0; i < numTickets; i++) {
        lastID++;
        ids.push_back(lastID);
    }
    return ids;
}

void tickets() {
    loadRegistrationFromFile();
    Registration* reg = nullptr;
    string eventName;

    displayRegistration();

    while (!reg) {
        cout << "\nEnter Event Name (or 'cancel' to exit): ";
        getline(cin, eventName);
        if (eventName == "cancel") {
            clearScreen();
            return;
        }
        reg = findRegistrationByName(eventName);
        if (!reg) cout << "Invalid event name! Please try again.\n";
    }

    int ticketsRequested;
    bool validInput = false;

    do {
        cout << "Enter number of tickets: ";
        string input;
        getline(cin, input);

        validInput = !input.empty();
        for (char c : input) if (!isdigit(c)) validInput = false;

        if (!validInput) {
            cout << "Invalid input! Enter a number only.\n";
            continue;
        }

        ticketsRequested = stoi(input);
        if (ticketsRequested <= 0) {
            cout << "Ticket amount must be > 0!\n";
            validInput = false;
        }
        else if (ticketsRequested > reg->ticketAmount) {
            cout << "Not enough tickets! Only " << reg->ticketAmount << " left.\n";
            validInput = false;
        }

    } while (!validInput);

    // Deduct tickets
    reg->ticketAmount -= ticketsRequested;

    // Generate guest IDs
    int lastID = reg->guestIDs.empty() ? 1000 : reg->guestIDs.back();
    vector<int> newGuestIDs = generateGuestIDs(lastID, ticketsRequested);
    reg->guestIDs.insert(reg->guestIDs.end(), newGuestIDs.begin(), newGuestIDs.end());

    // Save to file
    saveRegistrationToFile();

    // Display confirmation
    clearScreen();
    cout << "\n" << string(60, '=') << endl;
    cout << "              REGISTRATION CONFIRMATION" << endl;
    cout << string(60, '=') << endl;
    cout << "Event:        " << reg->registrationName << endl;
    cout << "Tickets:      " << ticketsRequested << endl;
    cout << "Total Cost:   RM" << fixed << setprecision(2)
        << reg->registrationCost * ticketsRequested << endl;

    cout << "\nGenerated Guest IDs:\n";
    for (int id : newGuestIDs) cout << " - " << id << endl;

    cout << string(60, '=') << endl;
    cout << "\nPress Enter to continue...";
    cin.get();
    clearScreen();
}
