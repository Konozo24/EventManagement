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
    int totalGuests;
    int checkedInGuests;
    string venue;
};

class Report {
private:
    vector<BookingSummary> summaries;

    void loadRegistration(const string &filename);
    void loadGuests(const string &filename);
    void loadVenues(const string &filename);

public:
    void generateReport();
    void showMenu();
};

#endif

