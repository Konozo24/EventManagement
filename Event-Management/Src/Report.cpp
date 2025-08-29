#include "Report.h"

void Report::loadRegistrations(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string eventName, eventDate, venue;
        int guests;
        ss >> eventName >> eventDate >> venue >> guests;
        summaries.push_back(BookingSummary(eventName, eventDate, guests, 0, venue));
    }
    file.close();
}

void Report::loadCheckins(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string eventName;
        int checkedInCount;
        ss >> eventName >> checkedInCount;
        for (auto& summary : summaries) {
            if (summary.eventName == eventName) {
                summary.checkedInGuests += checkedInCount;
            }
        }
    }
    file.close();
}

void Report::viewSummary() {
    if (summaries.empty()) {
        cout << "No event data available.\n";
        return;
    }
    cout << left << setw(15) << "Event Name"
        << setw(15) << "Date"
        << setw(15) << "Venue"
        << setw(15) << "Registered"
        << setw(15) << "Checked-In" << endl;
    cout << string(70, '-') << endl;
    for (auto& s : summaries) {
        cout << left << setw(15) << s.eventName
            << setw(15) << s.eventDate
            << setw(15) << s.venueName
            << setw(15) << s.totalGuests
            << setw(15) << s.checkedInGuests << endl;
    }
}

void Report::viewAttendance() {
    if (summaries.empty()) {
        cout << "No attendance data available.\n";
        return;
    }
    cout << left << setw(15) << "Event Name"
        << setw(15) << "Registered"
        << setw(15) << "Checked-In"
        << setw(15) << "Attendance %" << endl;
    cout << string(60, '-') << endl;
    for (auto& s : summaries) {
        double percentage = (s.totalGuests > 0) ? (s.checkedInGuests * 100.0 / s.totalGuests) : 0.0;
        cout << left << setw(15) << s.eventName
            << setw(15) << s.totalGuests
            << setw(15) << s.checkedInGuests
            << setw(15) << fixed << setprecision(1) << percentage << "%" << endl;
    }
}

void Report::viewVenueUsage() {
    if (summaries.empty()) {
        cout << "No venue data available.\n";
        return;
    }
    vector<string> venues;
    for (auto& s : summaries) {
        venues.push_back(s.venueName);
    }
    sort(venues.begin(), venues.end());
    venues.erase(unique(venues.begin(), venues.end()), venues.end());

    cout << left << setw(20) << "Venue" << setw(15) << "Times Used" << endl;
    cout << string(40, '-') << endl;
    for (auto& v : venues) {
        int count = 0;
        for (auto& s : summaries) {
            if (s.venueName == v) count++;
        }
        cout << left << setw(20) << v << setw(15) << count << endl;
    }
}

void Report::exportReport(const string& filename) {
    
    string path = __FILE__;
    size_t pos = path.find_last_of("\\/");  // works for both Windows/Linux
    string folder = (pos == string::npos) ? "" : path.substr(0, pos + 1);

    
    string fullPath = folder + filename;

    ofstream out(fullPath);
    if (!out.is_open()) {
        cout << "Error: Cannot write to " << fullPath << endl;
        return;
    }

    out << "Event Report Summary\n";
    out << left << setw(15) << "Event Name"
        << setw(15) << "Date"
        << setw(15) << "Venue"
        << setw(15) << "Registered"
        << setw(15) << "Checked-In" << endl;
    out << string(70, '-') << endl;
    for (auto& s : summaries) {
        out << left << setw(15) << s.eventName
            << setw(15) << s.eventDate
            << setw(15) << s.venueName
            << setw(15) << s.totalGuests
            << setw(15) << s.checkedInGuests << endl;
    }
    out.close();

    cout << "Report exported to: " << fullPath << endl;
}
