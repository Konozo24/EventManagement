#ifndef LOGIN_H
#define LOGIN_H

#include <string>
using namespace std;

extern bool isAdminLoggedIn;   // Global login status
extern string currentAdmin;    // Track who is logged in

bool adminLogin();             // Perform login
void adminLogout();            // Logout function
bool requireAdminLogin();      // Check before accessing admin features
void adminRegister();          // Register new admin

#endif
