#include "Report.h"
#include "Guest.h"
#include "Registration.h"
#include "Venue.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>



// Load Venues
void Report::loadVenues(const string& filename) {
    VenueManager venueManager;
    // Make sure global venues are loaded
    venueManager.loadVenuesFromFile();

    // Clear and rebuild venueMap
    venueMap.clear();
    for (const auto& v : venueManager.getVenues()) {
        venueMap[v.venueID] = v.name;
    }
    
}

// Load Events
void Report::loadEvents(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string eventID, eventName, eventDate, venueID, vendorDetails, ticketPrice, ticketAmount;

        getline(ss, eventID, '|');
        getline(ss, eventName, '|');
        getline(ss, eventDate, '|');
        getline(ss, venueID, '|');
        getline(ss, vendorDetails, '|');
        getline(ss, ticketPrice, '|');
        getline(ss, ticketAmount, '|');

        eventVenueMap[eventID] = venueID;
    }
    file.close();
}

void Report::loadRegistration(const string& filename) {
	RegistrationManager regManager;

    regManager.loadRegistrationFromFile();
    summaries.clear();

    for (const auto& reg : regManager.getRegistrations()) {
        string fullEventName = reg.eventName + " (" + reg.eventID + ")";

        // Check if this event is already in summaries
        auto it = find_if(summaries.begin(), summaries.end(),
            [&](const BookingSummary& s) { return s.eventName == fullEventName; });

        if (it == summaries.end()) {
            BookingSummary summary;
            summary.eventName = fullEventName;
            summary.date = reg.eventDate;

            string venueID = eventVenueMap[reg.eventID];
            summary.venue = (venueID.empty() || venueMap.find(venueID) == venueMap.end())
                ? "N/A" : venueMap[venueID];

            summary.totalGuests = 0;
            summary.checkedInGuests = 0;

            summaries.push_back(summary);
        }
    }
}

void Report::loadGuests() {
	RegistrationManager regManager;
	GuestManager guestManager;
    // Ensure global guest list is loaded
    guestManager.loadGuestsFromFile();

    // Reset summary counts first
    for (auto& s : summaries) {
        s.totalGuests = 0;
        s.checkedInGuests = 0;
    }

    // Count based on loaded guests
    for (const auto& reg : regManager.getRegistrations()) {
        string fullEventName = reg.eventName + " (" + reg.eventID + ")";
        for (auto& s : summaries) {
            if (s.eventName == fullEventName) {
                s.totalGuests++;
                if (reg.checkedIn) s.checkedInGuests++;
                break;
            }
        }
    }
}

void Report::displaySummary() {
    cout << left << setw(20) << "Event"
        << setw(15) << "Date"
        << setw(35) << "Venue"
        << setw(15) << "Total Guests"
        << setw(20) << "Checked-in Guests" << endl;
    cout << string(100, '-') << endl;

    for (const auto& s : summaries) {
        string venueDisplay = s.venue;
        if (venueDisplay == "N/A" || venueDisplay.empty()) {
            venueDisplay = "CANCELLED (Venue Released)";
        }


        cout << left << setw(20) << s.eventName
            << setw(15) << s.date
            << setw(35) << venueDisplay
            << setw(15) << s.totalGuests
            << setw(20) << s.checkedInGuests << endl;
    }
}

void Report::displayAttendance(RegistrationManager& regManager, GuestManager& guestManager) {
	
    regManager.loadRegistrationFromFile();
    guestManager.loadGuestsFromFile();

    cout << "\nAttendance Report:\n";
    cout << left << setw(10) << "RegID"
        << setw(10) << "GuestID"
        << setw(20) << "Name"
        << setw(25) << "Event"
        << setw(12) << "Tickets"
        << setw(15) << "Checked In"
        << setw(25) << "Time" << endl;
    cout << string(115, '-') << endl;

    for (const auto& reg : regManager.getRegistrations()) {
        Guest* g = guestManager.findGuestByID(reg.guestID);
        cout << left << setw(10) << reg.registrationID
            << setw(10) << reg.guestID
            << setw(20) << (g ? g->name : "(Unknown)")
            << setw(25) << reg.eventName
            << setw(12) << reg.ticketsBought
            << setw(15) << (reg.checkedIn ? "Yes" : "No")
            << setw(25) << (reg.checkedIn ? reg.checkInTime : "-")
            << endl;
    }
}

void Report::displayVenueStats() {
	VenueManager venueManager;
    venueManager.loadVenuesFromFile();
    cout << "\nVenue Usage Statistics:\n";
    cout << left << setw(10) << "VenueID"
        << setw(35) << "Venue Name"
        << setw(10) << "Capacity"
        << setw(30) << "Location"
        << setw(10) << "Booked"
        << setw(15) << "Usage Count" << endl;
    cout << string(90, '-') << endl;

    for (const auto& v : venueManager.getVenues()) {
        cout << left << setw(10) << v.venueID
            << setw(35) << v.name
            << setw(10) << v.capacity
            << setw(30) << v.location
            << setw(10) << (v.isBooked ? "Yes" : "No")
            << setw(15) << v.usageCount << endl;
    }
}

void Report::generateReport() {
    ofstream report("report.txt");
    if (!report) {
        cerr << "Error: Cannot create report.txt" << endl;
        return;
    }

    report << left << setw(20) << "Event"
        << setw(15) << "Date"
        << setw(35) << "Venue"
        << setw(15) << "Total Guests"
        << setw(20) << "Checked-in Guests" << endl;
    report << string(90, '-') << endl;

    for (const auto& s : summaries) {
        string venueDisplay = s.venue;
        if (venueDisplay == "N/A" || venueDisplay.empty()) {
            venueDisplay = "CANCELLED (Venue Released)";
        }

        report << left << setw(20) << s.eventName
            << setw(15) << s.date
            << setw(35) << venueDisplay
            << setw(15) << s.totalGuests
            << setw(20) << s.checkedInGuests << endl;
    }

    report.close();
    cout << "\nReport generated successfully: report.txt\n";
}

void Report::displayReportMenu() {
	RegistrationManager regManager;
	GuestManager guestManager;

    loadVenues("venues.txt");
    loadEvents("events.txt");
    loadRegistration("registration.txt");
    loadGuests();

    int choice;
    do {
        clearScreen();
        cout << "\n===== Event Reporting Menu =====\n";
        cout << "1. View Summary of All Events\n";
        cout << "2. View Attendance Report\n";
        cout << "3. View Venue Usage Statistics\n";
        cout << "4. Generate Report File\n";
        cout << "5. Return to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            displaySummary();
            break;

        case 2: {
			displayAttendance(regManager, guestManager);
            break;
        }

        case 3:
            displayVenueStats();
            break;

        case 4:
            generateReport();
            break;

        case 5:
            cout << "Returning to main menu...\n";
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            break;

        default:
            cout << "Invalid choice!\n";
        }

        if (choice != 5) {
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
        }

    } while (choice != 5);
}
