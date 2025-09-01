#include "Report.h"
#include "Guest.h"

#include "Venue.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>



// Load Venues
void Report::loadVenues(const string& filename) {
    // Make sure global venues are loaded
    loadVenuesFromFile();

    // Clear and rebuild venueMap
    venueMap.clear();
    for (const auto& v : venues) {
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
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string regId, eventId, eventName, date, username, tickets, cost;

        getline(ss, regId, '|');
        getline(ss, eventId, '|');
        getline(ss, eventName, '|');
        getline(ss, date, '|');
        getline(ss, username, '|');
        getline(ss, tickets, '|');
        getline(ss, cost, '|');

        // Format as "EventName (EventID)"
        string fullEventName = eventName + " (" + eventId + ")";

        // Find if this event already exists in summaries
        bool found = false;
        for (auto& s : summaries) {
            if (s.eventName == fullEventName) {
                found = true;
                break;
            }
        }

        if (!found) {
            BookingSummary summary;
            summary.eventName = fullEventName;
            summary.date = date;

            // Resolve venue
            string venueID = eventVenueMap[eventId];
            if (!venueID.empty() && venueMap.find(venueID) != venueMap.end()) {
                summary.venue = venueMap[venueID];
            }
            else {
                summary.venue = "N/A";
            }

            summary.totalGuests = 0;
            summary.checkedInGuests = 0;

            summaries.push_back(summary);
        }
    }
    file.close();
}

void Report::loadGuests() {
    // Ensure global guest list is loaded
    loadGuestsFromFile();

    // Reset summary counts first
    for (auto& s : summaries) {
        s.totalGuests = 0;
        s.checkedInGuests = 0;
    }

    // Count based on loaded guests
    for (const auto& g : guests) {
        for (auto& s : summaries) {
            string summaryNameOnly = s.eventName.substr(0, s.eventName.find(" ("));
            if (summaryNameOnly == g.eventName) {
                s.totalGuests++;
                if (g.checkedIn) s.checkedInGuests++;
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
        cout << left << setw(20) << s.eventName
            << setw(15) << s.date
            << setw(35) << s.venue
            << setw(15) << s.totalGuests
            << setw(20) << s.checkedInGuests << endl;
    }
}

void Report::displayAttendance() {
    cout << "\nAttendance Report:\n";
    cout << left << setw(10) << "GuestID"
        << setw(20) << "Name"
        << setw(20) << "Event"
        << setw(15) << "Checked In"
        << setw(20) << "Time" << endl;
    cout << string(85, '-') << endl;

    for (const auto& g : guests) {
        cout << left << setw(10) << g.guestID
            << setw(20) << g.name
            << setw(20) << g.eventName
            << setw(15) << (g.checkedIn ? "Yes" : "No")
            << setw(20) << g.checkInTime << endl;
    }
}

void Report::displayVenueStats() {
    cout << "\nVenue Usage Statistics:\n";
    cout << left << setw(10) << "VenueID"
        << setw(35) << "Venue Name"
        << setw(10) << "Capacity"
        << setw(30) << "Location"
        << setw(10) << "Booked"
        << setw(15) << "Usage Count" << endl;
    cout << string(90, '-') << endl;

    for (const auto& v : venues) {
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
        report << left << setw(20) << s.eventName
            << setw(15) << s.date
            << setw(35) << s.venue
            << setw(15) << s.totalGuests
            << setw(20) << s.checkedInGuests << endl;
    }

    report.close();
    cout << "\nReport generated successfully: report.txt\n";
}

void Report::displayReportMenu() {
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
			displayAttendance();
            break;
        }

        case 3:
            loadVenuesFromFile();
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
