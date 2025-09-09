#include "Login.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

vector<Admin> loadAdmins(const string& filename) {
    vector<Admin> admins;
    ifstream file(filename);
    if (!file) return admins;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string username, password;
        if (getline(ss, username, ',') && getline(ss, password, ',')) {
            admins.push_back({ username, password });
        }
    }
    return admins;
}

void saveAdmin(const string& filename, const Admin& admin) {
    ofstream file(filename, ios::app);
    if (!file) return;
    file << admin.username << "," << admin.password << "\n";
}

void adminRegister() {
    string username, password;
    cout << "===== Register New Admin =====" << endl;
    cout << "Enter new username: ";
    getline(cin, username);
    cout << "Enter new password: ";
    getline(cin, password);

    vector<Admin> admins = loadAdmins("admins.txt");
    for (const auto& admin : admins) {
        if (admin.username == username) {
            cout << "Username already exists! Registration failed.\n";
            return;
        }
    }

    saveAdmin("admins.txt", { username, password });
    cout << "Registration successful! You can now log in.\n";
}

bool adminLogin(AdminSession& session) {
    if (session.loggedIn) {
        cout << "Already logged in as " << session.currentAdmin << ".\n";
        return true;
    }

    while (true) { 
        string input;
        cout << "===== Admin Access =====\n";
        cout << "1. Login as existing admin\n";
        cout << "2. Register new admin\n";
        cout << "0. Cancel\n";
        cout << "Enter your choice: ";
        getline(cin, input);

        // Validate input
        if (input.size() != 1 || !isdigit(input[0])) {
            cout << "\nInvalid choice! Please enter 0, 1, or 2.\n\n";
            continue;
        }

        int choice = input[0] - '0';

        if (choice == 0) {
            clearScreen();
            return false;   // back to menu
        }

        if (choice == 2) {
            adminRegister();
            clearScreen();
            cout << "Please login with your new account.\n";
            continue;  // back to login menu
        }

        // ===== Existing Admin Login =====
        string username, password;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        vector<Admin> admins = loadAdmins("admins.txt");

        for (const auto& admin : admins) {
            if (admin.username == username && admin.password == password) {
                session.loggedIn = true;
                session.currentAdmin = username;
                cout << "\nLogin successful. Welcome, " << username << "!\n";
                return true;
            }
        }

        cout << "\nInvalid username or password.\n\n";
    }
}

void adminLogout(AdminSession& session) {
    if (session.loggedIn) {
        cout << "Admin " << session.currentAdmin << " logged out.\n";
        session.loggedIn = false;
        session.currentAdmin.clear();
    }
    else {
        cout << "No admin is currently logged in.\n";
    }
}

bool requireAdminLogin(AdminSession& session) {
    if (!session.loggedIn) {
        cout << "You must be logged in as Admin to access this feature.\n";
        return adminLogin(session);
    }
    return true;
}
