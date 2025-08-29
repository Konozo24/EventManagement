#include <iostream>
#include <vector>
#include "Venue.h"
#include "Guest.h"
#include "Booking.h"
#include "Monitoring.h"
#include "Payment.h"
#include "Feedback.h"
#include "Tickets.h"
#include "Registration.h"
#include "Report.h"
#include "Utils.h"
using namespace std;



int main() {
    cout << "=" << string(60, '=') << endl;
    cout << "EVENT MANAGEMENT MODULES" << endl;
    cout << "=" << string(60, '=') << endl;

    initializeDefaultRegistration(); 
    
    int choice;
    do {
        cout << "\n" << string(50, '=') << endl;
        cout << "         PRODUCT LAUNCH EVENT MANAGEMENT MENU" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Event Booking (Module 1)" << endl;
        cout << "2. Event Monitoring (Module 2)" << endl;
        cout << "3. View Available Venues" << endl;
        cout << "4. Payment & Checkout (Module 6)" << endl;
        cout << "5. Submit Feedback & Review (Module 7)" << endl;
        cout << "6. View Receipts (Admin)" << endl;
        cout << "7. View Feedbacks (Admin)" << endl;
        cout << "8. Register Events / Ticketing" << endl;
        cout << "9. Event Reporting (Admin)" << endl;
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
        case 4: {
            string guestID;
            cout << "Enter your Guest ID: ";
            getline(cin, guestID);
            processPayment(guestID);
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
        case 9:
            displayReportMenu();
            break;
        case 0:
            cout << "\nThank you for using the system!" << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-7." << endl;
        }

    } while (choice != 0);

    return 0;
}
