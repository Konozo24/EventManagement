#include "Feedback.h"
#include "Constants.h"
#include "Guest.h"
#include "Utils.h"
#include "Registration.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
using namespace std;

// ===== Helper: Get next feedback number =====
static int getNextFeedbackNumber() {
    ifstream in(FEEDBACK_FILE);
    string line;
    int lastNum = 0;

    while (getline(in, line)) {
        if (line.find("Feedback #") != string::npos) {
            lastNum = stoi(line.substr(10)); // "Feedback #" is 10 chars
        }
    }
    return lastNum + 1;
}

// ===== Helper: get a validated integer input (single try) =====
bool getValidatedInt(const string& prompt, int min, int max, int& value) {
    string input;
    cout << prompt;
    getline(cin, input);

    // Check if all characters are digits
    if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
        value = stoi(input);
        if (value >= min && value <= max) {
            return true; // valid input
        }
    }
    return false; // invalid
}

// ===== Main: Submit feedback =====
void submitFeedback() {
    string guestID, name, eventName, comment;
    int rating;

    cout << "\n====== Feedback ======";
    cout << "\nEnter Guest ID: ";
    cin >> guestID;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, name);

    // Verify Guest ID + Name
    loadGuestsFromFile();
    if (!verifyGuest(guestID, name)) {
        cout << "Guest ID and Name do not match our records.\n";
        return;
    }

    // Load this guest’s registrations
    loadRegistrationFromFile();
    vector<Registration> guestRegs;
    for (const auto& reg : registrations) {
        if (reg.guestID == guestID) {
            guestRegs.push_back(reg);
        }
    }

    if (guestRegs.empty()) {
        cout << "No registrations found for this guest.\n";
        return;
    }

    // Show list of events attended by this guest
    cout << "\nSelect the event to give feedback for:\n";
    for (size_t i = 0; i < guestRegs.size(); i++) {
        cout << i + 1 << ". [" << guestRegs[i].registrationID << "] "
            << guestRegs[i].eventName << " (" << guestRegs[i].eventDate << ")\n";
    }

    // Ask for event choice
    int choice;
    if (!getValidatedInt("Enter choice (1-" + to_string(guestRegs.size()) + "): ", 1, guestRegs.size(), choice)) {
        cout << "Invalid choice.\n";
        return;
    }
    Registration selected = guestRegs[choice - 1];

    // Ask for rating
    if (!getValidatedInt("Enter Rating (1-5): ", 1, 5, rating)) {
        cout << "Invalid rating.\n";
        return;
    }

    cout << "Enter Comment: ";
    getline(cin, comment);

    int count = getNextFeedbackNumber();
    ofstream out(FEEDBACK_FILE, ios::app);
    out << "Feedback #" << count << ":\n"
        << "Guest ID     : " << guestID << "\n"
        << "Name         : " << name << "\n"
        << "Reg ID       : " << selected.registrationID << "\n"
        << "Event Name   : " << selected.eventName << "\n"
        << "Event Date   : " << selected.eventDate << "\n"
        << "Rating       : " << rating << "/5\n"
        << "Comment      : " << comment << "\n"
        << "----------------------------------\n\n";
    out.close();

    cout << "\nFeedback submitted successfully.\n";

    cin.get();     // Wait for Enter
    clearScreen();
}

// ===== Main: View all feedback =====
void viewFeedback() {
    ifstream in(FEEDBACK_FILE);
    if (!in) {
        cout << "\nNo feedback found.\n";
        return;
    }

    cout << "\n======= All Feedback Entries =======\n";
    string line;
    while (getline(in, line)) {
        cout << line << "\n";
    }
    in.close();

    cin.get();     // Wait for Enter
    clearScreen();
}
