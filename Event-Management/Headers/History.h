#ifndef HISTORY_H
#define HISTORY_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "Utils.h"
#include "Booking.h"   // for Registration struct + registrations vector
using namespace std;

// ===== Function Prototypes =====
void viewUserHistory(const string& guestID);  // existing guest-only function
void viewAllHistory();                        // new admin full history
void adminHistoryMenu();                      // admin menu

#endif
