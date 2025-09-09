#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <vector>
using namespace std;


struct Admin {
    string username;
    string password;
};


struct AdminSession {
    bool loggedIn = false;
    string currentAdmin = "";
};


void adminRegister();
bool adminLogin(AdminSession& session);
void adminLogout(AdminSession& session);
bool requireAdminLogin(AdminSession& session);

#endif
