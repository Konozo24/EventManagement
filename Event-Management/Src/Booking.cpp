#include <iostream>
#include "Venue.h"
#include "Event.h"
#include "Booking.h"
#include "Utils.h"
#include "Constants.h"
#include <vector>
#include <string>
#include <fstream> 

using namespace std;


// MAIN FUNCTION: Event Booking on Dates & Logistics
void bookEvent() {
    cout << "\n" << string(60, '=') << endl;
    cout << "               EVENT BOOKING SYSTEM" << endl;
    cout << string(60, '=') << endl;

    // Create a VenueManager and EventManager instance
    VenueManager venueManager;
	EventManager eventManager;

    // Load venues from file
    venueManager.loadVenuesFromFile();

    // Display available venues
    venueManager.displayAvailableVenues();

    // Check if any venues are available
    bool hasAvailable = false;
    for (const auto& venue : venueManager.getVenues()) {
        if (!venue.isBooked) {
            hasAvailable = true;
            break;  
        }
    }

    if (!hasAvailable) {
        cout << "No venues are currently available for booking." << endl;
        cout << "Press Enter to return to main menu...";
        cin.get();
        return;
    }

    // Get event details with validation
    string eventName, eventDate, vendorDetails;

    // Event name validation
    do {
        cout << "\nEnter Event Name (or 'cancel' to exit): ";
        getline(cin, eventName);

        if (eventName == "cancel") {
            clearScreen();
            return;
        }

        if (eventName.empty() || eventName.length() < 3) {
            cout << "Event name must be at least 3 characters long!" << endl;
            continue;
        }

        // valid -> stop loop
        break;

    } while (eventName.empty() || eventName.length() < 3);


    // Event date validation
    do {
        cout << "Enter Event Date (DD-MM-YYYY): ";
        getline(cin, eventDate);

        if (!isValidDateFormat(eventDate)) {
            cout << "Invalid date format! Please use DD-MM-YYYY format." << endl;
            cout << "Example: 15-08-2025" << endl;
            continue;
        }

        // valid -> stop loop
        break;

    } while (!isValidDateFormat(eventDate));

    // Venue selection with validation
    string selectedVenueID = venueManager.validateVenueSelection();
    if (selectedVenueID == "0") {
        cout << "Booking cancelled." << endl;
        return;
    }

    // Get vendor details
    cout << "\nEnter Vendor Details (press Enter for none): ";
    getline(cin, vendorDetails);
    if (vendorDetails.empty()) {
        vendorDetails = "No vendors assigned";
    }

    double ticketPrice = 0.0;
    bool validPrice = false;

    do {
        cout << "Enter Ticket Price: ";
        string priceInput;
        getline(cin, priceInput);

        try {
            size_t idx;
            ticketPrice = stod(priceInput, &idx); // string → double

            if (idx != priceInput.size()) {
                throw invalid_argument("Invalid characters after number");
            }

            if (ticketPrice > 0) {
                validPrice = true; 
            }
            else {
                cout << "Ticket price must be greater than 0!\n";
            }
        }
        catch (invalid_argument&) {
            cout << "Invalid input! Enter a valid number (e.g., 50 or 49.90).\n";
        }
        catch (out_of_range&) {
            cout << "Number too large! Try again.\n";
        }

    } while (!validPrice);
    


    // Find venue capacity & Update venue booking status
    int ticketAmount = 0;
    for (auto& venue : venueManager.getVenues()) {
        if (venue.venueID == selectedVenueID) {
            venue.isBooked = true;
            venue.usageCount++;
            ticketAmount = venue.capacity;  // capacity comes from Venue
            break;
        }
    }

    // Generate event ID (auto-increment)
    int nextEventNum = eventManager.getEvents().empty() ? 1 : stoi(eventManager.getEvents().back().eventID.substr(1)) + 1;
    string newEventID = "E" + to_string(nextEventNum);

    // Create new Event object
    Event newEvent(newEventID, eventName, eventDate, selectedVenueID,
        vendorDetails, ticketPrice, ticketAmount);

    // Add to global vector
    eventManager.getEvents().push_back(newEvent);

    // Save all events to file
    eventManager.saveEventsToFile();

    // Save updated venues
    venueManager.saveVenuesToFile();

    // Display booking confirmation
    cout << "\n" << string(60, '=') << endl;
    cout << "              BOOKING CONFIRMATION" << endl;
    cout << string(60, '=') << endl;
    cout << "Event ID: " << newEventID << endl;
    cout << "Event Name: " << eventName << endl;
    cout << "Event Date: " << eventDate << endl;
    cout << "Venue ID: " << selectedVenueID << endl;
    cout << "Vendor Details: " << vendorDetails << endl;
    cout << "Status: Successfully Booked!" << endl;
    cout << string(60, '=') << endl;

    cout << "\nPress Enter to continue...";
    cin.get();
    clearScreen();
}
