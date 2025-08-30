#include "Report.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

void Report::loadRegistration(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    summaries.clear();
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        BookingSummary summary;
        getline(ss, summary.eventName, ',');
        getline(ss, summary.date, ',');
        getline(ss, summary.venue, ',');
        summary.totalGuests = 0;
        summary.checkedInGuests = 0;
        summaries.push_back(summary);
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
        int id;
        string name, event, checkedStr, time;
        ss >> id;
        ss.ignore();
        getline(ss, name, ',');
        getline(ss, event, ',');
        getline(ss, checkedStr, ',');
        getline(ss, time, ',');

        bool checked = (checkedStr == "1");

        for (auto& s : summaries) {
            if (s.eventName == event) {
                s.totalGuests++;
                if (checked) s.checkedInGuests++;
            }
        }
    }
    file.close();
}

void Report::loadVenues(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open " << filename << endl;
        return;
    }

    cout << "\nVenue Usage Statistics:\n";
    cout << left << setw(20) << "Venue" << setw(10) << "Times Used" << endl;
    cout << string(30, '-') << endl;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string venue;
        int times;
        getline(ss, venue, ',');
        ss >> times;
        cout << left << setw(20) << venue << setw(10) << times << endl;
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
        << setw(15) << "Venue"
        << setw(15) << "Total Guests"
        << setw(20) << "Checked-in Guests" << endl;
    report << string(80, '-') << endl;

    for (const auto& s : summaries) {
        report << left << setw(20) << s.eventName
            << setw(15) << s.date
            << setw(15) << s.venue
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
            loadRegistration("registration.txt");
            loadGuests("guest.txt");
            cout << left << setw(20) << "Event"
                << setw(15) << "Date"
                << setw(15) << "Venue"
                << setw(15) << "Total Guests"
                << setw(20) << "Checked-in Guests" << endl;
            cout << string(80, '-') << endl;
            for (const auto& s : summaries) {
                cout << left << setw(20) << s.eventName
                    << setw(15) << s.date
                    << setw(15) << s.venue
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
