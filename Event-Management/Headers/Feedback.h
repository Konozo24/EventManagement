#pragma once
#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <string>
#include <vector>
using namespace std;

struct Feedback {
    string guestID;
    string name;
    string eventName;
    int rating;
    string comment;
};

// Feedback functions
void submitFeedback();
void viewFeedback();

#endif
