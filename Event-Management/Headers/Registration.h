#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>
#include <vector>
#include "Marketing.h"
using namespace std;

struct Registration {
    string registrationID;   // e.g. "R1", "R2"
    string eventID;          // link to Event (e.g. "E1")
    string eventName;
    string eventDate;
    string guestID;
    string userName;         // who registered
    int ticketsBought;       // number of tickets bought
    double registrationCost; // total cost

    bool checkedIn;
    string checkInTime;

    Registration();
    Registration(const string& regId, const string& eventId, const string& eventName,
        const string& eventDate, const string& guestID, const string& userName,
        int tickets, double cost, bool checkedIn = false, const string& checkInTime = "");
};

class RegistrationManager {
private:
    vector<Registration> registrations;

public:

    // File handling
    void loadRegistrationFromFile();
    void saveRegistrationToFile();

    // Display
    void displayRegistration() const;

    // GenerateID
    string generateRegistrationID();

    //Validation
    string validateRegistrationIDInput() const;

    // Accessors
    vector<Registration>& getRegistrations() { return registrations; }
};


#endif
