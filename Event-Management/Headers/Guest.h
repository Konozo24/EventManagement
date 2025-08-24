#pragma once
#ifndef GUEST_H
#define GUEST_H

#include <string>
#include <vector>
using namespace std;

struct Guest {
    int guestID;
    string name;
    string eventName;
    bool checkedIn;
    string checkInTime;

    Guest();
    Guest(int id, const string& guestName, const string& event);

    void checkIn();
};

// Declare global guest list
extern vector<Guest> guests;

// Guest utilities
void loadGuestsFromFile();
void displayRegisteredGuests();
int validateGuestIDInput();

#endif
