#include "Login.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Utils.h"

using namespace std;

bool isAdminLoggedIn = false;
string currentAdmin = "";

struct Admin {
    string username;
    string password;
};

// Load admins from file
vector<Admin> loadAdmins(const string& filename) {
    vector<Admin> admins;
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open " << filename << endl;
        return admins;
    }

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

// Save new admin
void saveAdmin(const string& filename, const Admin& admin) {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Could not open " << filename << " for writing." << endl;
        return;
    }
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

bool adminLogin() {
    if (isAdminLoggedIn) {
        cout << "Already logged in as " << currentAdmin << ".\n";
        return true;
    }

    string input;
    cout << "===== Admin Access =====\n";
    cout << "1. Login as existing admin\n";
    cout << "2. Register new admin\n";
    cout << "0. Cancel\n";
    cout << "Enter your choice: ";
    getline(cin, input);

    // ✅ Must be exactly 1 character and a digit
    if (input.size() != 1 || !isdigit(input[0])) {
        cout << "\nInvalid choice! Please enter 0, 1, or 2.\n\n";
        return false;
    }

    int choice = input[0] - '0';  // safe conversion

    if (choice == 0) {
        clearScreen();
        return false;   // user cancelled → back to menu
    }

    if (choice == 2) {
        adminRegister();
        clearScreen();
        cout << "Registration successful! Please login with your new account.\n";
        return false;   // must login after register
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
            isAdminLoggedIn = true;
            currentAdmin = username;
            cout << "\nLogin successful. Welcome, " << username << "!\n";
            return true;
        }
    }

    cout << "\nInvalid username or password.\n";
    return false;
}



void adminLogout() {
    if (isAdminLoggedIn) {
        cout << "Admin " << currentAdmin << " logged out.\n";
        isAdminLoggedIn = false;
        currentAdmin = "";
    }
    else {
        cout << "No admin is currently logged in.\n";
    }
}

bool requireAdminLogin() {
    if (!isAdminLoggedIn) {
        cout << "You must be logged in as Admin to access this feature.\n";
        return adminLogin();
    }
    return true;
}
