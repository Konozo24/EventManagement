#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>
#include <vector>
using namespace std;

struct Registration {
    int registrationID;
    string registrationName;
    int ticketAmount;       // tickets available
    double registrationCost;
    vector<int> guestIDs;   // store all guest IDs for this event

    Registration();
    Registration(int regId, const string& regName, int ticAmt, double regCost);
};

extern vector<Registration> registrations;

// File handling
void initializeDefaultRegistration();
void loadRegistrationFromFile();
void saveRegistrationToFile();

// Display
void displayRegistration();

// Validation
Registration* findRegistrationByName(const string& name);

#endif
