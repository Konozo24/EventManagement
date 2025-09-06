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

class VenueManager {
private:
	vector<Venue> venues;

public:
    // File handling
    void loadVenuesFromFile();
    void saveVenuesToFile();
    void initializeDefaultVenues();

    // Validation
    bool isValidVenueFormat(const string& input) const;
    string validateVenueSelection() const;
    string validateBookedVenueSelection() const;

    // Global accessors
    bool isVenueAvailable(const string& venueID);
    Venue getVenueDetails(const string& venueID);
    vector<Venue>& getVenues();

    // Display
    void displayAvailableVenues();
    void displayAllVenues();
};



#endif
