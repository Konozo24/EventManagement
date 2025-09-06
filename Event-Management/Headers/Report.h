#ifndef REPORT_H
#define REPORT_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Registration.h"
#include "Guest.h"

using namespace std;

struct BookingSummary {
    string eventName;
    string date;
    int totalGuests;
    int checkedInGuests;
    string venue;
};

class Report {
private:
    vector<BookingSummary> summaries;

    void loadRegistration(const string& filename);
    void loadGuests();
    void loadVenues(const string& filename);
    void loadEvents(const string& filename);

    unordered_map<string, string> venueMap;      // venueID -> venueName
    unordered_map<string, string> eventVenueMap; // eventID -> venueID

public:
    void generateReport();
    void displayReportMenu();
    void displaySummary();
    void displayAttendance(RegistrationManager& regManager, GuestManager& guestManager);
    void displayVenueStats();
};

#endif
