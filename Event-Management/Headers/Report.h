#pragma once
#ifndef REPORT_H
#define REPORT_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct BookingSummary {
    string eventName;
    string date;
    string venue;
    int totalGuests;
    int checkedInGuests;
};

class Report {
private:
    vector<BookingSummary> summaries;

public:
    void loadRegistrationData(const string& filename);
    void loadCheckInData(const string& filename);
    void loadVenueData(const string& filename);
    void displayAllEventsSummary();
    void displayAttendanceReport();
    void displayVenueUsage();
    void exportReportToFile(const string& filename);
};

// function to show the menu
void displayReportMenu();

#endif
