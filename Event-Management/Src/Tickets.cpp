#include "Tickets.h"
#include "Event.h"
#include "Registration.h"
#include "Utils.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
using namespace std;



void tickets() {
    loadEventsFromFile();
    Event* ev = nullptr;
    string eventName, userName;

	displayEventsForRegistration();

  
    do {
        cout << "\nEnter Event Name (or 'cancel' to exit): ";
        getline(cin, eventName);

        if (eventName == "cancel") {
            clearScreen();
            return;
        }

        ev = findEventByName(eventName);   
        if (!ev) cout << "Invalid event name! Please try again.\n";
            
    } while (!ev);

    do {
        cout << "Enter your name (or 'cancel'): ";
        getline(cin, userName);

        if (userName == "cancel") { 
            clearScreen(); 
            return; 
        }

        if (userName.empty()) cout << "Name cannot be empty.\n";
    } while (userName.empty());


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
        else if (ticketsRequested > ev->ticketAmount) {
            cout << "Not enough tickets! Only " << ev->ticketAmount << " left.\n";
            validInput = false;
        }

    } while (!validInput);

    // Deduct tickets
    ev->ticketAmount -= ticketsRequested;
    saveEventsToFile();   // update event list with reduced tickets


    // Generate Registration ID
    string newRegID = generateRegistrationID(); // "R#" auto-generated


    // Create and save Registration
    double totalCost = ev->ticketPrice * ticketsRequested; // total cost
    Registration newReg(
        newRegID,       // registrationID (string "R#")
        ev->eventID,    // eventID ("E#")
        ev->eventName,  // eventName
        ev->eventDate,  // eventDate
        userName,       // userName
        ticketsRequested,
        totalCost
    );


    

    // Save to file
    registrations.push_back(newReg);
    saveRegistrationToFile();

    // Display confirmation
    clearScreen();
    cout << "\n" << string(60, '=') << "\n";
    cout << "         REGISTRATION CONFIRMATION\n";
    cout << string(60, '=') << "\n";
    cout << "Registration ID: " << newReg.registrationID << "\n";
    cout << "Event:           " << newReg.eventName << " (" << newReg.eventID << ")\n";
	cout << "Date:            " << newReg.eventDate << "\n";
    cout << "Name:            " << newReg.userName << "\n";
    cout << "Tickets:         " << newReg.ticketsBought << "\n";
    cout << "Total Paid:      RM" << fixed << setprecision(2) << newReg.registrationCost << "\n";

    cout << string(60, '=') << endl;
    cout << "\nPress Enter to continue...";
    cin.get();
    clearScreen();
}
