#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include "ReservationManager.h"

using namespace std;

// Helper function to handle booking inputs
void handleBooking(ReservationManager& manager) {
    string name, email, phone, date;
    int choice;

    cout << "\n==================================\n";
    cout << "        BOOK A NEW ROOM\n";
    cout << "==================================\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clean up input buffer
    
    cout << "Enter Guest Name: ";
    getline(cin, name);
    
    cout << "Enter Email Address: ";
    getline(cin, email);
    
    cout << "Enter Phone Number: ";
    getline(cin, phone);
    
    cout << "Enter Reservation Date (e.g., DD/MM/YYYY): ";
    getline(cin, date);
    
    cout << "\nSelect Room Type:\n";
    cout << "1. Economy Class ($80 per night)\n";
    cout << "2. Business Class ($180 per night)\n";
    cout << "Enter choice (1 or 2): ";
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid selection. Please choose 1 or 2: ";
    }
    
    // Offload the creation to the manager
    int assignedId = manager.addReservation(name, email, phone, date, choice);
    
    cout << "\n🎉 Room booked successfully! Your Booking ID is: " << assignedId << "\n";
}

// Helper function to display the bookings in a table
void displayBookings(const ReservationManager& manager) {
    cout << "\n=========================================================================================\n";
    cout << "                                  CURRENT RESERVATIONS\n";
    cout << "=========================================================================================\n";
    
    if (manager.isEmpty()) {
        cout << "No active reservations found. The hotel is currently empty!\n";
        cout << "=========================================================================================\n";
        return;
    }
    
    // Print Table Headers
    cout << left << setw(8)  << "ID" 
         << setw(20) << "Name" 
         << setw(25) << "Email" 
         << setw(15) << "Phone" 
         << setw(15) << "Date" 
         << setw(12) << "Type" 
         << setw(10) << "Price" << endl;
    cout << "-----------------------------------------------------------------------------------------\n";
    
    // Retrieve read-only reference from the manager
    const auto& list = manager.getAllReservations();
    for (const auto& booking : list) {
        cout << left << setw(8)  << booking.id 
             << setw(20) << booking.name 
             << setw(25) << booking.email 
             << setw(15) << booking.phone 
             << setw(15) << booking.date 
             << setw(12) << booking.roomType 
             << "$" << fixed << setprecision(2) << booking.price << endl;
    }
    cout << "=========================================================================================\n";
}

// Helper function to remove a booking by ID
void removeBooking(ReservationManager& manager) {
    int bookingId;

    cout << "\n==================================\n";
    cout << "        REMOVE RESERVATION\n";
    cout << "==================================\n";

    if (manager.isEmpty()) {
        cout << "No reservations available to remove.\n";
        return;
    }

    cout << "Enter Booking ID to remove: ";
    while (!(cin >> bookingId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a numeric Booking ID: ";
    }

    if (manager.removeReservation(bookingId)) {
        cout << "Reservation " << bookingId << " removed successfully.\n";
    } else {
        cout << "No reservation found with Booking ID " << bookingId << ".\n";
    }
}

static string jsonEscape(const string& s) {
    string out;
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            default: out.push_back(c);
        }
    }
    return out;
}

int main(int argc, char* argv[]) {
    ReservationManager hotelManager; // Create our manager object

    // Non-interactive CLI mode for GUI/server integration
    if (argc > 1) {
        string cmd = argv[1];
        if (cmd == "list") {
            const auto& list = hotelManager.getAllReservations();
            cout << "[";
            bool first = true;
            for (const auto& r : list) {
                if (!first) cout << ",";
                first = false;
                cout << "{\"id\":" << r.id
                     << ",\"name\":\"" << jsonEscape(r.name) << "\""
                     << ",\"email\":\"" << jsonEscape(r.email) << "\""
                     << ",\"phone\":\"" << jsonEscape(r.phone) << "\""
                     << ",\"date\":\"" << jsonEscape(r.date) << "\""
                     << ",\"roomType\":\"" << jsonEscape(r.roomType) << "\""
                     << ",\"price\":" << r.price << "}";
            }
            cout << "]\n";
            return 0;
        } else if (cmd == "add") {
            if (argc < 7) {
                cerr << "USAGE: add \"name\" \"email\" \"phone\" \"date\" roomTypeChoice\n";
                return 2;
            }
            string name = argv[2];
            string email = argv[3];
            string phone = argv[4];
            string date = argv[5];
            int roomTypeChoice = atoi(argv[6]);
            int id = hotelManager.addReservation(name, email, phone, date, roomTypeChoice);
            cout << id << "\n";
            return 0;
        } else if (cmd == "remove") {
            if (argc < 3) {
                cerr << "USAGE: remove id\n";
                return 2;
            }
            int id = atoi(argv[2]);
            bool ok = hotelManager.removeReservation(id);
            cout << (ok ? "OK" : "NOTFOUND") << "\n";
            return 0;
        } else {
            cerr << "Unknown command\n";
            return 2;
        }
    }

    int choice;
    
    do {
        cout << "\n==================================\n";
        cout << "     GRAND VISTA HOTEL PORTAL\n";
        cout << "==================================\n";
        cout << "1. Book a Room\n";
        cout << "2. View All Reservations\n";
        cout << "3. Remove Reservation\n";
        cout << "4. Exit System\n";
        cout << "----------------------------------\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        
        // Error handling for non-integer inputs
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 4.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                handleBooking(hotelManager);
                break;
            case 2:
                displayBookings(hotelManager);
                break;
            case 3:
                removeBooking(hotelManager);
                break;
            case 4:
                cout << "\nThank you for using the Grand Vista Portal. Have a great day!\n";
                break;
            default:
                cout << "\nInvalid choice! Please pick an option from the menu.\n";
        }
    } while (choice != 4);
    
    return 0;
}