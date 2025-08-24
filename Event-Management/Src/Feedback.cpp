#include "Feedback.h"
#include "Constants.h"
#include "Guest.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
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
    if (!verifyGuest(stoi(guestID), name)) {
        cout << "Guest ID and Name do not match our records.\n";
        return;
    }

    cout << "Enter Event Name: ";
    getline(cin, eventName);
    cout << "Enter Rating (1-5): ";
    cin >> rating;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Comment: ";
    getline(cin, comment);

    int count = getNextFeedbackNumber();
    ofstream out(FEEDBACK_FILE, ios::app);
    out << "Feedback #" << count << ":\n"
        << "Guest ID   : " << guestID << "\n"
        << "Name       : " << name << "\n"
        << "Event Name : " << eventName << "\n"
        << "Rating     : " << rating << "/5\n"
        << "Comment    : " << comment << "\n"
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
