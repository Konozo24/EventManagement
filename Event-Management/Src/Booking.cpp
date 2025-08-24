#include <iostream>
#include "Venue.h"
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

    // Load venues from file
    loadVenuesFromFile();

    // Display available venues
    displayAvailableVenues();

    // Check if any venues are available
    bool hasAvailable = false;
    for (const auto& venue : venues) {
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
        break;
    } while (true);

    // Event date validation
    do {
        cout << "Enter Event Date (DD-MM-YYYY): ";
        getline(cin, eventDate);

        if (!isValidDateFormat(eventDate)) {
            cout << "Invalid date format! Please use DD-MM-YYYY format." << endl;
            cout << "Example: 15-08-2025" << endl;
            continue;
        }
        break;
    } while (true);

    // Venue selection with validation
    int selectedVenueID = validateVenueSelection();
    if (selectedVenueID == 0) {
        cout << "Booking cancelled." << endl;
        return;
    }

    // Get vendor details
    cout << "\nEnter Vendor Details (press Enter for none): ";
    getline(cin, vendorDetails);
    if (vendorDetails.empty()) {
        vendorDetails = "No vendors assigned";
    }

    // Update venue booking status
    for (auto& venue : venues) {
        if (venue.venueID == selectedVenueID) {
            venue.isBooked = true;
            venue.usageCount++;
            break;
        }
    }

    // Save booking to events file
    ofstream bookingFile(EVENTS_FILE, ios::app);
    if (bookingFile.is_open()) {
        bookingFile << eventName << "|" << eventDate << "|"
            << selectedVenueID << "|" << vendorDetails << endl;
        bookingFile.close();
    }

    // Save updated venues
    saveVenuesToFile();

    // Display booking confirmation
    cout << "\n" << string(60, '=') << endl;
    cout << "              BOOKING CONFIRMATION" << endl;
    cout << string(60, '=') << endl;
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
