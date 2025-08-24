#pragma once
#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <string>

// Feedback structure
struct Feedback {
    std::string guestID;
    std::string name;
    std::string eventName;
    int rating;
    std::string comment;
};

// Core functions
void submitFeedback();
void viewFeedback();

// Helpers
bool verifyGuest(const std::string& guestID, const std::string& name);
int getNextFeedbackNumber();

#endif
