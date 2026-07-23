#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

// A pure data model representing a single booking
struct Reservation {
    int id;
    std::string name;
    std::string email;
    std::string phone;
    std::string date;
    std::string roomType;
    double price;
};

#endif // RESERVATION_H