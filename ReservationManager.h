#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include <vector>
#include "Reservation.h"

#include <string>

#define RESERVATION_DATA_FILE "reservations.txt"

class ReservationManager {
private:
    std::vector<Reservation> bookings;
    int nextBookingId;
    // Persistence helpers
    void saveToFile() const;
    void loadFromFile();
    static std::string escape(const std::string& s);
    static std::string unescape(const std::string& s);

public:
    // Constructor initializes the system
    ReservationManager();

    // Core business logic methods
    int addReservation(const std::string& name, const std::string& email, 
                       const std::string& phone, const std::string& date, 
                       int roomTypeChoice);

    // Remove a reservation by booking ID
    bool removeReservation(int bookingId);
                       
    // Returns a read-only reference to the list of bookings
    const std::vector<Reservation>& getAllReservations() const;
    
    // Check if the reservation list is empty
    bool isEmpty() const;
};

#endif // RESERVATION_MANAGER_H