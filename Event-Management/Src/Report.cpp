#include "Report.h"
#include <fstream>
#include <iomanip>
#include <map>

// load registration.txt
void Report::loadRegistrationData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    summaries.clear();
    string event, date, venue;
    int guests;
    while (file >> event >> date >> venue >> guests) {
        BookingSummary summary{event, date, venue, guests, 0};
        summaries.push_back(summary);
    }
    file.close();
}

// load checkin_log.txt
void Report::loadCheckInData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    string event;
    int checkedIn;
    while (file >> event >> checkedIn) {
        for (auto& s : summaries) {
            if (s.eventName == event) {
                s.checkedInGuests = checkedIn;
                break;
            }
        }
    }
    file.close();
}

// load venues.txt
void Report::loadVenueData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open " << filename << endl;
        return;
    }
    map<string, int> venueCount;
    string venue;
    while (file >> venue) {
        venueCount[venue]++;
    }
    cout << "\nVenue Usage Statistics\n";
    cout << left << setw(15) << "Venue" << setw(10) << "Usage Count" << endl;
    cout << string(25, '-') << endl;
    for (auto& v : venueCount) {
        cout << left << setw(15) << v.first << setw(10) << v.second << endl;
    }
    file.close();
}

// summary of events
void Report::displayAllEventsSummary() {
    if (summaries.empty()) {
        cout << "No registration data available.\n";
        return;
    }
    cout << "\nSummary of All Events\n";
    cout << left << setw(15) << "Event Name" 
         << setw(15) << "Date" 
         << setw(15) << "Venue" 
         << setw(15) << "Registered" 
         << setw(15) << "Checked-In" << endl;
    cout << string(70, '-') << endl;
    for (const auto& s : summaries) {
        cout << left << setw(15) << s.eventName 
             << setw(15) << s.date 
             << setw(15) << s.venue 
             << setw(15) << s.totalGuests 
             << setw(15) << s.checkedInGuests << endl;
    }
}

// attendance report
void Report::displayAttendanceReport() {
    if (summaries.empty()) {
        cout << "No attendance data available.\n";
        return;
    }
    cout << "\nAttendance Report\n";
    for (const auto& s : summaries) {
        cout << "Event: " << s.eventName 
             << " | Registered: " << s.totalGuests 
             << " | Checked-In: " << s.checkedInGuests << endl;
    }
}

// export to file
void Report::exportReportToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not create " << filename << endl;
        return;
    }
    file << "Event Report Summary\n";
    file << left << setw(15) << "Event Name" 
         << setw(15) << "Date" 
         << setw(15) << "Venue" 
         << setw(15) << "Registered" 
         << setw(15) << "Checked-In" << endl;
    file << string(70, '-') << endl;
    for (const auto& s : summaries) {
        file << left << setw(15) << s.eventName 
             << setw(15) << s.date 
             << setw(15) << s.venue 
             << setw(15) << s.totalGuests 
             << setw(15) << s.checkedInGuests << endl;
    }
    file.close();
    cout << "Report exported to " << filename << endl;
}

// menu function
void displayReportMenu() {
    Report report;
    int choice;
    do {
        cout << "\n===== Event Reporting Module =====\n";
        cout << "1. View Summary of All Events\n";
        cout << "2. View Attendance Report\n";
        cout << "3. View Venue Usage Statistics\n";
        cout << "4. Export Report to File\n";
        cout << "5. Return to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                report.loadRegistrationData("registration.txt");
                report.loadCheckInData("checkin_log.txt");
                report.displayAllEventsSummary();
                break;
            case 2:
                report.loadRegistrationData("registration.txt");
                report.loadCheckInData("checkin_log.txt");
                report.displayAttendanceReport();
                break;
            case 3:
                report.loadVenueData("venues.txt");
                break;
            case 4:
                report.loadRegistrationData("registration.txt");
                report.loadCheckInData("checkin_log.txt");
                report.exportReportToFile("report.txt");
                break;
            case 5:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);
}
