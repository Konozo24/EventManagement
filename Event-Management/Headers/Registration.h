#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>
#include <vector>
using namespace std;

struct Registration {
    string registrationID;   // e.g. "R1", "R2"
    string eventID;          // link to Event (e.g. "E1")
    string eventName;
	string eventDate;   
    string userName;         // who registered
    int ticketsBought;       // number of tickets bought
	double registrationCost; // total cost
    


    Registration();
    Registration(const string& regId, const string& eventId, const string& eventName, const string& eventDate, const string& userName, int tickets, double registrationCost);
};

extern vector<Registration> registrations;

// File handling
void loadRegistrationFromFile();
void saveRegistrationToFile();

// Display
void displayRegistration();

string generateRegistrationID();



#endif
