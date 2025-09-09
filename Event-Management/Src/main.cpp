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
#include "Login.h"
#include "History.h"

using namespace std;

void printMainMenu() {
    cout << "\n" << string(60, '=') << endl;
    cout << "           PRODUCT LAUNCH EVENT MANAGEMENT SYSTEM " << endl;
    cout << string(60, '=') << endl;

    cout << "\n---------------General Options---------------\n";
    cout << "  1. Register for Events / Ticketing\n";
    cout << "  2. View Receipts\n";
    cout << "  3. Submit Feedback & Review\n";
    cout << "  4. Marketing (User)\n";
    cout << "  5. View Event History Participate\n";
    cout << "  6. View Admin Options (Login required)\n";
    cout << "\n---------------Exit---------------\n";
    cout << "  0. Exit Program\n";

    cout << string(60, '-') << endl;
    cout << "Enter your choice: ";
}

void printAdminMenu() {
    cout << "\n--- Admin Options ---\n";
    cout << "  1. Event Booking\n";
    cout << "  2. Event Monitoring\n";
    cout << "  3. View Available Venues\n";
    cout << "  4. View Receipts (Admin)\n";
    cout << "  5. View Feedbacks (Admin)\n";
    cout << "  6. Event Reporting\n";
    cout << "  7. Marketing (Admin)\n";
	cout << "  8. View Event History\n";
    cout << "  9. Admin Logout\n";
	cout << "  0. Return back to main menu\n";

    cout << string(60, '-') << endl;
    cout << "Enter your choice: ";
}


int main() {
	VenueManager venueManager;
    MarketingManager marketManager;
    Report report;
    venueManager.loadVenuesFromFile();

    string input;
    int choice = -1;
    do {

        printMainMenu();
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
            tickets();
            break;

        case 2:
            viewReceipts();
            break;

        case 3:
            submitFeedback();
            break;

        case 4:
            marketManager.marketingUser();
            break;

        case 5: {
            string guestID;
            cout << "Enter your Guest ID or 0 to cancel: ";
            getline(cin, guestID);
            if (guestID == "0") {
                clearScreen();
                break;  // go back to menu
            }
            viewUserHistory(guestID);
            break;
        }

        case 6: {
            if (!requireAdminLogin()) {
                adminLogin();
                if (!isAdminLoggedIn) {
                    clearScreen();
                    break; // back to main if login fails/cancelled
                }
            }

            int adminChoice = -1;
            do {
                printAdminMenu();
                getline(cin, input);

                bool isNumber = !input.empty() && all_of(input.begin(), input.end(), ::isdigit);
                if (!isNumber) {
                    cout << "Invalid input! Please enter a number only.\n";
                    cout << "\nPress Enter to continue...";
                    cin.get();
                    clearScreen();
                    continue;
                }

                adminChoice = stoi(input);
                clearScreen();

                switch (adminChoice) {
                case 1:
                    bookEvent();
                    break;
                case 2:
                    monitorEvent();
                    break;
                case 3: {
                    venueManager.loadVenuesFromFile();
                    venueManager.displayAvailableVenues();
					cout << "Press Enter to continue...";
					cin.get();
					clearScreen();
                } break;
                case 4:
                    viewReceipts();
                    break;
                case 5:
                    viewFeedback();
                    break;
                case 6: 
                    report.displayReportMenu();
                    break;
                case 7: 
                    marketManager.marketingAdmin();
                    break;
				case 8:
					adminHistoryMenu();
					break;
                case 9:
                    adminLogout();
                    adminChoice = 0;
                    break;
                case 0:
                    cout << "Returning to Main Menu...\n";
                    break;
                default:
                    cout << "Invalid choice! Try again.\n";
                    break;
                }
            } while (adminChoice != 0);
            break;

        }

        case 0:
            cout << "\nThank you for using the system!" << endl;
            break;

        default:
            cout << "Invalid choice! Please select a valid menu option." << endl;
            break;
        }


    } while (choice != 0);
    return 0;
}
