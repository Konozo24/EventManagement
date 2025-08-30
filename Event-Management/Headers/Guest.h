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
    bool checkedIn;
    string checkInTime;

    Guest();
    Guest(const string& id, const string& guestName, const string& event);

    void checkIn();
};

// Declare global guest list
extern vector<Guest> guests;

// Guest utilities
void loadGuestsFromFile();
void saveGuestsToFile();
void displayRegisteredGuests();
string validateGuestIDInput();
string generateGuestID();

// Extra utilities (for Payment & Feedback)
bool verifyGuest(const string& guestID, const string& name);

#endif
