#include "Tickets.h"
#include "Event.h"
#include "Guest.h"
#include "Registration.h"
#include "Utils.h"
#include "Constants.h"
#include "Payment.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
using namespace std;



void tickets() {
    loadEventsFromFile();
    Event* ev = nullptr;
    string eventName, userName, guestID;

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

    // --- Ask if they already have a Guest ID ---
    string hasGuestID;
    do {
        cout << "Do you already have a Guest ID? (Y/N): ";
        getline(cin, hasGuestID);
        

        // Convert to uppercase for consistency
        for (auto& c : hasGuestID) c = toupper(c);

        if (hasGuestID != "Y" && hasGuestID != "N") {
            cout << "Invalid choice! Please enter Y or N only.\n";
        }
    } while (hasGuestID != "Y" && hasGuestID != "N");

    

    if (hasGuestID == "Y") {
        bool validGuest = false;
        while (!validGuest) {
            cout << "Enter your Guest ID: ";
            getline(cin, guestID);

            loadGuestsFromFile();
            Guest* existingGuest = findGuestByID(guestID);
            if (existingGuest) {
                // Check if entered name matches the Guest ID
                if (existingGuest->name != userName) {
                    cout << "Error: Guest ID does not match the entered name!\n";
                    cout << "Please enter the correct Guest ID or type 'new' to create a new profile.\n";

                    string choice;
                    getline(cin, choice);
                    if (choice == "new" || choice == "NEW") {
                        guestID = generateGuestID();
                        cout << "Your new Guest ID is: " << guestID << endl;
                        cout << "\nPress Enter to continue...";
                        cin.get();
                        clearScreen();

                        Guest newGuest(guestID, userName, ev->eventName);
                        guests.push_back(newGuest);
                        saveGuestsToFile();
                        validGuest = true;
                    }
                }
                else {
                    cout << "Welcome back, " << existingGuest->name << "!\n";
                    userName = existingGuest->name;
                    validGuest = true;
                }
            }
            else {
                cout << "Guest ID not found! Try again (or type 'new' to create a profile): ";
                string choice;
                getline(cin, choice);
                if (choice == "new" || choice == "NEW") {
                    guestID = generateGuestID();
                    cout << "Your new Guest ID is: " << guestID << endl;
                    cout << "\nPress Enter to continue...";
                    cin.get();
                    clearScreen();

                    Guest newGuest(guestID, userName, "");
                    guests.push_back(newGuest);
                    saveGuestsToFile();
                    validGuest = true;
                }
            }
        }
    }
    else {
        // New guest directly
        guestID = generateGuestID();
        cout << "Your new Guest ID is: " << guestID << endl;
        cout << "\nPress Enter to continue...";
        cin.get();
        clearScreen();

        Guest newGuest(guestID, userName, "");
        guests.push_back(newGuest);
        saveGuestsToFile();
    }

    // --- Check if this guest already registered for the event ---
    loadRegistrationFromFile();
    bool alreadyRegistered = false;

    for (const auto& r : registrations) {
        if (r.guestID == guestID && r.eventID == ev->eventID) {
            alreadyRegistered = true;
            break;
        }
    }

    if (alreadyRegistered) {
        cout << "\nError: You (" << userName << ") are already registered for this event!\n";
        cout << "Duplicate registrations are not allowed.\n";
        cout << "\nPress Enter to continue...";
        cin.get();
        clearScreen();
        return; // exit before ticket asking
    }

    // Deduct tickets
    ev->ticketAmount -= ticketsRequested;
    saveEventsToFile();   // update event list with reduced tickets

    // When registering a user
    loadRegistrationFromFile(); // make sure nextRegistrationID is correct
    // Generate Registration ID
    string newRegID = generateRegistrationID(); // "R#" auto-generated

    


    // Create and save Registration
    double totalCost = ev->ticketPrice * ticketsRequested; // total cost

    Registration newReg(
        newRegID,       // registrationID (string "R#")
        ev->eventID,    // eventID ("E#")
        ev->eventName,  // eventName
        ev->eventDate,  // eventDate
		guestID,    // guestID
        userName,       // userName
        ticketsRequested,
        totalCost
    );


    // Process payment immediately
    clearScreen();
    processPayment(newReg, ev);

    // Save to file
    registrations.push_back(newReg);
    saveRegistrationToFile();

    // Display confirmation
    clearScreen();
    cout << "\n" << string(60, '=') << "\n";
    cout << "         REGISTRATION CONFIRMATION\n";
    cout << string(60, '=') << "\n";
    cout << "Registration ID: " << newReg.registrationID << "\n";
    cout << "Guest ID:        " << newReg.guestID << "\n";  
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
