#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
using namespace std;

struct Event {
    string eventID;          // unique ID
    string eventName;     // e.g., "iPhone 28 Launch"
    string eventDate;     // e.g., "15-08-2025"
    string venueID;          // which venue is used
    string vendorDetails; // who manages it
    double ticketPrice;   // ticket cost
    int ticketAmount;     // total tickets available

    Event() : eventID(""), eventName(""), eventDate(""), venueID(""),
        vendorDetails(""), ticketPrice(0.0), ticketAmount(0) {
    }
    Event(const string& evID, const string& name, const string& date, const string& venue,
        const string& vendor, double price, int amount)
        : eventID(evID), eventName(name), eventDate(date), venueID(venue),
        vendorDetails(vendor), ticketPrice(price), ticketAmount(amount) {
    }
};

// Global list of events
extern vector<Event> events;

// File handling
void loadEventsFromFile();
void saveEventsToFile();

// Display
void displayEvents();
void displayEventsForRegistration();

// Searching
Event* findEventByName(const string& name);

#endif
