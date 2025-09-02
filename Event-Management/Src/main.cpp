#include <iostream>
#include <vector>
#include <algorithm>
#include "Venue.h"
#include "Guest.h"
#include "Booking.h"
#include "Monitoring.h"
#include "Marketing.h"
#include "Payment.h"
#include "Feedback.h"
#include "Tickets.h"
#include "Registration.h"
#include "Report.h"
#include "Utils.h"

using namespace std;



int main() {
    loadVenuesFromFile();

    


    string input;
    int choice = -1;
    do {
        
        cout << "\n" << string(50, '=') << endl;
        cout << "         PRODUCT LAUNCH EVENT MANAGEMENT MENU" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Event Booking (Module 1)" << endl;
        cout << "2. Event Monitoring (Module 2)" << endl;
        cout << "3. View Available Venues" << endl;
        cout << "4. View Receipts Guest (Module 6)" << endl;
        cout << "5. Submit Feedback & Review (Module 7)" << endl;
        cout << "6. View Receipts (Admin)" << endl;
        cout << "7. View Feedbacks (Admin)" << endl;
        cout << "8. Register Events / Ticketing" << endl;
        cout << "9. Event Reporting (Admin)" << endl;
        cout << "10. Marketing (User)" << endl;
        cout << "11. Marketing (Admin)" << endl;
        cout << "12. View Guest Event History" << endl;
        cout << "0. Exit" << endl;
        cout << string(50, '-') << endl;
        cout << "Enter your choice: ";

        getline(cin, input);

        // Validate: must be digits only
        bool isNumber = !input.empty() && all_of(input.begin(), input.end(), ::isdigit);

        if (!isNumber) {
            cout << "Invalid input! Please enter a number only." << endl;
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            continue;
        }
        choice = stoi(input);
        clearScreen();

        switch (choice) {
        case 1:
            bookEvent();
            break;
        case 2:
            monitorEvent();
            clearScreen();
            break;
        case 3:
            loadVenuesFromFile();
            displayAvailableVenues();
            cout << "\nPress Enter to continue...";
            cin.get();
            clearScreen();
            break;
        case 4: {
            viewReceipts();
            break;
        }
        case 5:
            submitFeedback();
            break;
        case 6:
            viewReceipts();
            break;
        case 7:
            viewFeedback();
            break;
        case 8:
            tickets();
            break;
        case 9: {
            Report report;
            report.displayReportMenu();
            break;
        }
            
        case 10:
            marketingUser(); 
            break;
        case 11:
            marketingAdmin();
            break;
        case 12: {
            string guestID = validateGuestIDInput(); // makes sure it starts with 'G' and exists
            if (guestID != "0") { // not cancelled
                viewUserHistory(guestID);
            }
            break;
        }
        case 0:
            cout << "\nThank you for using the system!" << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-7." << endl;
        }

    } while (choice != 0);

    return 0;
}
