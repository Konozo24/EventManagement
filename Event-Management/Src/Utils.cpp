#include "Utils.h"
#include <iostream>
using namespace std;


// cross-platform clear screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#endif
}

// Validate date format (DD-MM-YYYY) - Your input validation
bool isValidDateFormat(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return false;
    }

    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 2025 || year > 2030) return false;
   
    return true;
}

