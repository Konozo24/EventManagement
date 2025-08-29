#pragma once
#ifndef REPORT_H
#define REPORT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct BookingSummary {
    string eventName;
    string eventDate;
    int totalGuests;
    int checkedInGuests;
    string venueName;

    BookingSummary(string name = "", string date = "", int guests = 0, int checkin = 0, string venue = "")
        : eventName(name), eventDate(date), totalGuests(guests), checkedInGuests(checkin), venueName(venue) {
    }
};

class Report {
private:
    vector<BookingSummary> summaries;

public:
    void loadRegistrations(const string& filename);
    void loadCheckins(const string& filename);

    void viewSummary();
    void viewAttendance();
    void viewVenueUsage();

    void exportReport(const string& filename);
};

#endif
