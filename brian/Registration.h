#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>
#include <vector>
using namespace std;

struct Registration {
    int registrationID;
    string registrationName;
    int ticketAmount; 
    double registrationCost;

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
