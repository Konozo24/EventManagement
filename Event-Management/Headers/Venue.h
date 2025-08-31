#ifndef VENUE_H
#define VENUE_H

#include <vector> 
#include <string>
using namespace std;

struct Venue {
    string venueID;
    string name;
    int capacity;
    string location;
    double cost;
    bool isBooked;
    int usageCount;

    Venue();
    Venue(const string& id, const string& venueName, int cap, const string& loc, double venueCost);
};

extern vector<Venue> venues;   // declare global

// Global accessors
bool isVenueAvailable(int venueID);
Venue getVenueDetails(int venueID);

// File handling
void loadVenuesFromFile();
void saveVenuesToFile();
void initializeDefaultVenues();
bool isValidVenueFormat(const string& input);
string validateVenueSelection();
string validateBookedVenueSelection();

// Display
void displayAvailableVenues();
void displayAllVenues();

#endif
