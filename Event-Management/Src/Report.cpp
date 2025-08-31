#include "Report.h"

#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

// Load Venues
void Report::loadVenues(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string venueID, venueName, capacity, location, cost, booked, eventCount;

        getline(ss, venueID, '|');
        getline(ss, venueName, '|');
        getline(ss, capacity, '|');
        getline(ss, location, '|');
        getline(ss, cost, '|');
        getline(ss, booked, '|');
        getline(ss, eventCount, '|');

        venueMap[venueID] = venueName;  // store mapping
    }
    file.close();
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

void Report::loadGuests(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string guestId, name, event, checkedStr, time;

        getline(ss, guestId, '|');   // G1
        getline(ss, name, '|');      // Ming
        getline(ss, event, '|');     // Lebron Tales
        getline(ss, checkedStr, '|');// 1
        getline(ss, time, '|');      // Sun Aug ...

        bool checked = (checkedStr == "1");

        for (auto& s : summaries) {
            // Compare only the event name part (before " (E1)")
            string summaryNameOnly = s.eventName.substr(0, s.eventName.find(" ("));
            if (summaryNameOnly == event) {
                s.totalGuests++;
                if (checked) s.checkedInGuests++;
            }
        }
    }
    file.close();
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
    report << string(80, '-') << endl;

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
    int choice;
    do {
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
            loadVenues("venues.txt");       
            loadEvents("events.txt");       
            loadRegistration("registration.txt"); 
            loadGuests("guests.txt");       


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
            break;

        case 2: {
            // Attendance Report
            ifstream file("guest.txt");
            if (!file) {
                cerr << "Error: Cannot open guest.txt\n";
                break;
            }

            cout << "\nAttendance Report (guest.txt):\n";
            cout << left << setw(10) << "GuestID"
                << setw(20) << "Name"
                << setw(20) << "Event"
                << setw(15) << "Checked In"
                << setw(20) << "Time" << endl;
            cout << string(85, '-') << endl;

            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                int id;
                string name, event, checkedStr, time;
                ss >> id;
                ss.ignore();
                getline(ss, name, ',');
                getline(ss, event, ',');
                getline(ss, checkedStr, ',');
                getline(ss, time, ',');

                string checked = (checkedStr == "1") ? "Yes" : "No";

                cout << left << setw(10) << id
                    << setw(20) << name
                    << setw(20) << event
                    << setw(15) << checked
                    << setw(20) << time << endl;
            }
            file.close();
            break;
        }

        case 3:
            loadVenues("venues.txt");
            break;

        case 4:
            generateReport();
            break;

        case 5:
            cout << "Returning to main menu...\n";
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
