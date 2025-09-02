#pragma once
#ifndef GUEST_H
#define GUEST_H

#include <string>
#include <vector>
using namespace std;

struct Guest {
    string guestID;
    string name;
    string eventName;
   

    Guest();
    Guest(const string& id, const string& guestName, const string& event);

 
};

// Declare global guest list
extern vector<Guest> guests;

// Guest utilities
void loadGuestsFromFile();
void saveGuestsToFile();

string validateGuestIDInput();
string generateGuestID();

// Extra utilities (for Payment & Feedback)
bool verifyGuest(const string& guestID, const string& name);
Guest* findGuestByID(const string& id);

#endif
