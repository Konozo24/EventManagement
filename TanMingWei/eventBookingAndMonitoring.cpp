#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// Files names
const string VENUES_FILE = "venues.txt";
const string EVENTS_FILE = "events.txt";
const string REGISTRATION_FILE = "registration.txt";
const string CHECKIN_LOG_FILE = "checkin_log.txt";


void bookEvent();                    // Complete booking system
void monitorEvent();                // Guest check-in system
void displayAvailableVenues();     // Show venue list
bool isValidDateFormat(const string& date);  // Date validation
void loadVenuesFromFile();          // Load venue data
void saveVenuesToFile();            // Save venue data
void clearScreen();                 // Clear console screen

// Venue structure for event booking
struct Venue {
    int venueID;
    string name;
    int capacity;
    string location;
    double cost;
    bool isBooked;
    int usageCount;

    // Default constructor
    Venue() : venueID(0), name(""), capacity(0), location(""), cost(0.0), isBooked(false), usageCount(0) {}

    // Parameterized constructor
    Venue(int id, const string& venueName, int cap, const string& loc, double venueCost)
        : venueID(id), name(venueName), capacity(cap), location(loc), cost(venueCost), isBooked(false), usageCount(0) {
    }
};

struct Guest {
    int guestID;
    string name;
    string eventName;
    bool checkedIn;
    string checkInTime;


    // Default constructor
    Guest() : guestID(0), name(""), eventName(""), checkedIn(false), checkInTime("") {}

    // Parameterized constructor
    Guest(int id, const string& guestName,   const string& event)
        : guestID(id), name(guestName), eventName(event), checkedIn(false), checkInTime("") {}

    // Method to check in guest (your implementation)
    void checkIn() {
        checkedIn = true;
        time_t now = time(0);
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &now);
        checkInTime = string(timeStr);

        if (!checkInTime.empty() && checkInTime.back() == '\n') {
            checkInTime.pop_back();
        }
    }
};
   
//Global Vectors (shared with teammate)
vector<Venue> venues;
vector<Guest> guests;

// Initialize default venues if file doesn't exist
void initializeDefaultVenues() {
    venues.clear();

    venues.push_back(Venue(1, "Skyline Grand Ballroom", 400, "Downtown City Center", 18000.0));
    venues.push_back(Venue(2, "Innovation Hub Conference Hall", 250, "Tech Business Park", 12000.0));
    venues.push_back(Venue(3, "Garden Pavilion", 300, "Botanical Gardens", 14000.0));
    venues.push_back(Venue(4, "Executive Boardroom", 50, "Corporate Tower", 6000.0));
    venues.push_back(Venue(5, "University Auditorium", 600, "City University Campus", 20000.0));
    venues.push_back(Venue(6, "Rooftop Terrace", 150, "Luxury Hotel Downtown", 10000.0));
    venues.push_back(Venue(7, "Tech Expo Hall", 800, "Convention Center", 25000.0));

    saveVenuesToFile();
}

// Load venues from file
void loadVenuesFromFile() {
    ifstream file(VENUES_FILE);
    if (!file.is_open()) {
        cout << "Creating new venues file with default venues..." << endl;
        initializeDefaultVenues();
        return;
    }

    venues.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string item;
        vector<string> tokens;

        while (getline(ss, item, '|')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 6) {
            Venue venue;
            venue.venueID = stoi(tokens[0]);
            venue.name = tokens[1];
            venue.capacity = stoi(tokens[2]);
            venue.location = tokens[3];
            venue.cost = stod(tokens[4]);
            venue.isBooked = (tokens[5] == "1");
            venue.usageCount = (tokens.size() > 6) ? stoi(tokens[6]) : 0;
            venues.push_back(venue);
        }
    }
    file.close();
}

// Save venues to file
void saveVenuesToFile() {
    ofstream file(VENUES_FILE);
    if (!file.is_open()) {
        cout << "Error: Cannot save venues to file!" << endl;
        return;
    }

    for (const auto& venue : venues) {
        file << venue.venueID << "|"
            << venue.name << "|"
            << venue.capacity << "|"
            << venue.location << "|"
            << venue.cost << "|"
            << (venue.isBooked ? 1 : 0) << "|"
            << venue.usageCount << endl;
    }
    file.close();
}

// Display available venues (formatted table)
void displayAvailableVenues() {
    cout << "\n" << string(80, '=') << endl;
    cout << "                      AVAILABLE VENUES" << endl;
    cout << string(80, '=') << endl;
    cout << left << setw(5) << "ID"
        << setw(40) << "Venue Name"
        << setw(10) << "Capacity"
        << setw(30) << "Location"
        << setw(15) << "Cost (RM)"
        << setw(10) << "Status" << endl;
    cout << string(80, '-') << endl;

    bool hasAvailable = false;
    for (const auto& venue : venues) {
        if (!venue.isBooked) {
            cout << left << setw(5) << venue.venueID
                << setw(40) << venue.name
                << setw(10) << venue.capacity
                << setw(30) << venue.location
                << "RM" << setw(10) << fixed << setprecision(2) << venue.cost
                << setw(10) << "Available" << endl;
            hasAvailable = true;
        }
    }

    if (!hasAvailable) {
        cout << "No venues are currently available for booking." << endl;
    }
    cout << string(80, '=') << endl;
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

// Validate venue selection - Your input validation
int validateVenueSelection() {
    int venueID;
    bool validInput = false;

    do {
        cout << "Enter Venue ID to book (0 to cancel): ";
        if (!(cin >> venueID)) {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (venueID == 0) return 0;

        bool found = false;
        for (const auto& venue : venues) {
            if (venue.venueID == venueID) {
                found = true;
                if (venue.isBooked) {
                    cout << "Sorry, this venue is already booked. Please select another venue." << endl;
                    break;
                }
                else {
                    validInput = true;
                    break;
                }
            }
        }

        if (!found) {
            cout << "Venue ID not found! Please enter a valid Venue ID." << endl;
        }

    } while (!validInput);

    return venueID;
}

// MAIN FUNCTION: Event Booking on Dates & Logistics
void bookEvent() {
    cout << "\n" << string(60, '=') << endl;
    cout << "               EVENT BOOKING SYSTEM" << endl;
    cout << string(60, '=') << endl;

    // Load venues from file
    loadVenuesFromFile();

    // Display available venues
    displayAvailableVenues();

    // Check if any venues are available
    bool hasAvailable = false;
    for (const auto& venue : venues) {
        if (!venue.isBooked) {
            hasAvailable = true;
            break;
        }
    }

    if (!hasAvailable) {
        cout << "No venues are currently available for booking." << endl;
        cout << "Press Enter to return to main menu...";
        cin.get();
        return;
    }

    // Get event details with validation
    string eventName, eventDate, vendorDetails;

    // Event name validation
    do {
        cout << "\nEnter Event Name (or 'cancel' to exit): ";
        getline(cin, eventName);

        if (eventName == "cancel") return;

        if (eventName.empty() || eventName.length() < 3) {
            cout << "Event name must be at least 3 characters long!" << endl;
            continue;
        }
        break;
    } while (true);

    // Event date validation
    do {
        cout << "Enter Event Date (DD-MM-YYYY): ";
        getline(cin, eventDate);

        if (!isValidDateFormat(eventDate)) {
            cout << "Invalid date format! Please use DD-MM-YYYY format." << endl;
            cout << "Example: 15-08-2025" << endl;
            continue;
        }
        break;
    } while (true);

    // Venue selection with validation
    int selectedVenueID = validateVenueSelection();
    if (selectedVenueID == 0) {
        cout << "Booking cancelled." << endl;
        return;
    }

    // Get vendor details
    cout << "\nEnter Vendor Details (press Enter for none): ";
    getline(cin, vendorDetails);
    if (vendorDetails.empty()) {
        vendorDetails = "No vendors assigned";
    }

    // Update venue booking status
    for (auto& venue : venues) {
        if (venue.venueID == selectedVenueID) {
            venue.isBooked = true;
            venue.usageCount++;
            break;
        }
    }

    // Save booking to events file
    ofstream bookingFile(EVENTS_FILE, ios::app);
    if (bookingFile.is_open()) {
        bookingFile << eventName << "|" << eventDate << "|"
            << selectedVenueID << "|" << vendorDetails << endl;
        bookingFile.close();
    }

    // Save updated venues
    saveVenuesToFile();

    // Display booking confirmation
    cout << "\n" << string(60, '=') << endl;
    cout << "              BOOKING CONFIRMATION" << endl;
    cout << string(60, '=') << endl;
    cout << "Event Name: " << eventName << endl;
    cout << "Event Date: " << eventDate << endl;
    cout << "Venue ID: " << selectedVenueID << endl;
    cout << "Vendor Details: " << vendorDetails << endl;
    cout << "Status: Successfully Booked!" << endl;
    cout << string(60, '=') << endl;

    cout << "\nPress Enter to continue...";
    cin.get();
    clearScreen();
}


// Module 2: Event Monitoring

// Load guests from registration file
void loadGuestsFromFile() {
    ifstream file(REGISTRATION_FILE);
    if (!file.is_open()) {
        return; // File doesn't exist yet
    }

    guests.clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string item;
        vector<string> tokens;

        while (getline(ss, item, '|')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 3) {
            Guest guest;
            guest.guestID = stoi(tokens[0]);
            guest.name = tokens[1];
            guest.eventName = tokens[2];
            guest.checkedIn = (tokens.size() > 3) ? (tokens[3] == "1") : false;
            guest.checkInTime = (tokens.size() > 4) ? tokens[4] : "";
            guests.push_back(guest);
        }
    }
    file.close();
}

// Display registered guests in formatted table
void displayRegisteredGuests() {
    cout << "\n" << string(80, '=') << endl;
    cout << "                    REGISTERED GUESTS" << endl;
    cout << string(80, '=') << endl;

    if (guests.empty()) {
        cout << "No guests are currently registered for any events." << endl;
        cout << string(80, '=') << endl;
        return;
    }

    cout << left << setw(10) << "Guest ID"
        << setw(20) << "Name"
        << setw(25) << "Event"
        << setw(12) << "Status"
        << setw(20) << "Check-in Time" << endl;
    cout << string(80, '-') << endl;

    for (const auto& guest : guests) {
        cout << left << setw(10) << guest.guestID
            << setw(20) << guest.name
            << setw(25) << guest.eventName
            << setw(12) << (guest.checkedIn ? "Checked In" : "Not Checked")
            << setw(20) << guest.checkInTime << endl;
    }
    cout << string(80, '=') << endl;
}

// Validate guest ID input 
int validateGuestIDInput() {
    int guestID;
    bool validInput = false;

    do {
        cout << "Enter Guest ID to check in (0 to return to menu): ";
        if (!(cin >> guestID)) {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (guestID == 0) return 0;

        if (guestID < 0) {
            cout << "Guest ID cannot be negative!" << endl;
            continue;
        }

        validInput = true;

    } while (!validInput);

    return guestID;
}

// Display check-in statistics
void displayCheckInStatistics() {
    cout << "\n" << string(60, '=') << endl;
    cout << "              CHECK-IN STATISTICS" << endl;
    cout << string(60, '=') << endl;

    int totalGuests = guests.size();
    int checkedInGuests = 0;

    for (const auto& guest : guests) {
        if (guest.checkedIn) {
            checkedInGuests++;
        }
    }

    cout << "Total Registered Guests: " << totalGuests << endl;
    cout << "Checked-in Guests: " << checkedInGuests << endl;
    cout << "Pending Check-ins: " << (totalGuests - checkedInGuests) << endl;

    if (totalGuests > 0) {
        double percentage = (static_cast<double>(checkedInGuests) / totalGuests) * 100;
        cout << "Check-in Rate: " << fixed << setprecision(1) << percentage << "%" << endl;
    }

    cout << string(60, '=') << endl;
}

// MAIN FUNCTION: Event Monitoring
void monitorEvent() {
    cout << "\n" << string(70, '=') << endl;
    cout << "                EVENT MONITORING SYSTEM" << endl;
    cout << string(70, '=') << endl;

    // Load latest guest data
    loadGuestsFromFile();

    // Display current guest status
    displayRegisteredGuests();

    if (guests.empty()) {
        cout << "\nPress Enter to return to main menu...";
        cin.get();
        clearScreen();
        return;
    }

    // Event monitoring menu loop
    int choice;
    do {
        cout << "\nEvent Monitoring Options:" << endl;
        cout << "1. Check in a guest" << endl;
        cout << "2. Refresh guest list" << endl;
        cout << "3. View check-in statistics" << endl;
        cout << "0. Return to main menu" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        switch (choice) {
        case 1: {
            int guestID = validateGuestIDInput();
            if (guestID == 0) break;

            bool found = false;
            for (auto& guest : guests) {
                if (guest.guestID == guestID) {
                    found = true;
                    if (guest.checkedIn) {
                        cout << "\nGuest " << guest.name << " (ID: " << guestID
                            << ") is already checked in at " << guest.checkInTime << endl;
                    }
                    else {
                        guest.checkIn();

                        // Log check-in activity
                        ofstream logFile(CHECKIN_LOG_FILE, ios::app);
                        if (logFile.is_open()) {
                            logFile << guestID << "|" << guest.name << "|"
                                << guest.eventName << "|" << guest.checkInTime << endl;
                            logFile.close();
                        }

                        cout << "\n" << string(50, '*') << endl;
                        cout << "CHECK-IN SUCCESSFUL!" << endl;
                        cout << "Guest: " << guest.name << endl;
                        cout << "ID: " << guestID << endl;
                        cout << "Event: " << guest.eventName << endl;
                        cout << "Time: " << guest.checkInTime << endl;
                        cout << string(50, '*') << endl;
                    }
                    break;
                }
            }

            if (!found) {
                cout << "\nGuest ID " << guestID << " not found in the registration list!" << endl;
                cout << "Please verify the ID and try again." << endl;
            }
            break;
        }
        case 2:
            loadGuestsFromFile();
            displayRegisteredGuests();
            break;
        case 3:
            displayCheckInStatistics();
            break;
        case 0:
            cout << "Returning to main menu..." << endl;
            clearScreen();
            break;
        default:
            cout << "Invalid choice! Please select 0-3." << endl;
        }

    } while (choice != 0);

    
}

// =============================================================================
// HELPER FUNCTIONS FOR TEAMMATES
// =============================================================================
   
// Clear Screen
void clearScreen() {
#ifdef _WIN32
    system("cls");   // Windows

#endif
}

// Check if venue is available (for other modules to use)
bool isVenueAvailable(int venueID) {
    loadVenuesFromFile();
    for (const auto& venue : venues) {
        if (venue.venueID == venueID && !venue.isBooked) {
            return true;
        }
    }
    return false;
}

// Get venue details (for other modules to use)
Venue getVenueDetails(int venueID) {
    loadVenuesFromFile();
    for (const auto& venue : venues) {
        if (venue.venueID == venueID) {
            return venue;
        }
    }
    return Venue(); // Return empty venue if not found
}

// Check in guest by ID (for other modules to use)
bool checkInGuestByID(int guestID) {
    loadGuestsFromFile();
    for (auto& guest : guests) {
        if (guest.guestID == guestID && !guest.checkedIn) {
            guest.checkIn();

            // Log check-in
            ofstream logFile(CHECKIN_LOG_FILE, ios::app);
            if (logFile.is_open()) {
                logFile << guestID << "|" << guest.name << "|"
                    << guest.eventName << "|" << guest.checkInTime << endl;
                logFile.close();
            }
            return true;
        }
    }
    return false;
}
// =============================================================================
// MAIN FUNCTION - Demo of my modules
// =============================================================================
int main() {
    cout << "=" << string(60, '=') << endl;
    cout << "    TAN MING WEI - EVENT MANAGEMENT MODULES" << endl;
    cout << "=" << string(60, '=') << endl;

    int choice;
    do {
        cout << "\n" << string(50, '=') << endl;
        cout << "         PRODUCT LAUNCH EVENT MANAGEMENT MENU" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Event Booking (Module 1)" << endl;
        cout << "2. Event Monitoring (Module 2)" << endl;
        cout << "3. View Available Venues" << endl;
        cout << "0. Exit" << endl;
        cout << string(50, '-') << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        clearScreen();
        switch (choice) {
        case 1:
            bookEvent();
            break;
        case 2:
            monitorEvent();
            break;
        case 3:
            loadVenuesFromFile();
            displayAvailableVenues();
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            break;
        case 0:
            cout << "\nThank you for using the system!" << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-3." << endl;
        }

    } while (choice != 0);

    return 0;
}
