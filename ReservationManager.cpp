
#include "ReservationManager.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

ReservationManager::ReservationManager()
	: nextBookingId(1) {
	loadFromFile();
}

void ReservationManager::saveToFile() const {
	std::ofstream ofs(RESERVATION_DATA_FILE, std::ios::trunc);
	if (!ofs) return;
	for (const auto& r : bookings) {
		ofs << r.id << '|' << escape(r.name) << '|' << escape(r.email) << '|' 
			<< escape(r.phone) << '|' << escape(r.date) << '|' << escape(r.roomType)
			<< '|' << r.price << '\n';
	}
}

void ReservationManager::loadFromFile() {
	std::ifstream ifs(RESERVATION_DATA_FILE);
	if (!ifs) return;
	bookings.clear();
	std::string line;
	int maxId = 0;
	while (std::getline(ifs, line)) {
		if (line.empty()) continue;
		std::istringstream iss(line);
		std::string token;
		Reservation r;
		// id
		if (!std::getline(iss, token, '|')) continue;
		r.id = std::stoi(token);
		maxId = std::max(maxId, r.id);
		// name
		if (!std::getline(iss, token, '|')) continue;
		r.name = unescape(token);
		// email
		if (!std::getline(iss, token, '|')) continue;
		r.email = unescape(token);
		// phone
		if (!std::getline(iss, token, '|')) continue;
		r.phone = unescape(token);
		// date
		if (!std::getline(iss, token, '|')) continue;
		r.date = unescape(token);
		// roomType
		if (!std::getline(iss, token, '|')) continue;
		r.roomType = unescape(token);
		// price
		if (!std::getline(iss, token)) continue;
		try { r.price = std::stod(token); } catch(...) { r.price = 0.0; }

		bookings.push_back(r);
	}
	nextBookingId = maxId + 1;
}

std::string ReservationManager::escape(const std::string& s) {
	std::string out;
	for (char c : s) {
		if (c == '|' || c == '\\' || c == '\n') {
			out.push_back('\\');
		}
		out.push_back(c);
	}
	return out;
}

std::string ReservationManager::unescape(const std::string& s) {
	std::string out;
	bool esc = false;
	for (char c : s) {
		if (esc) {
			out.push_back(c);
			esc = false;
		} else if (c == '\\') {
			esc = true;
		} else {
			out.push_back(c);
		}
	}
	return out;
}

int ReservationManager::addReservation(const std::string& name, const std::string& email,
									   const std::string& phone, const std::string& date,
									   int roomTypeChoice) {
	Reservation booking;
	booking.id = nextBookingId++;
	booking.name = name;
	booking.email = email;
	booking.phone = phone;
	booking.date = date;

	if (roomTypeChoice == 1) {
		booking.roomType = "Economy Class";
		booking.price = 80.0;
	} else {
		booking.roomType = "Business Class";
		booking.price = 180.0;
	}

	bookings.push_back(booking);
	saveToFile();
	return booking.id;
}

bool ReservationManager::removeReservation(int bookingId) {
	auto it = std::find_if(bookings.begin(), bookings.end(),
		[bookingId](const Reservation& booking) {
			return booking.id == bookingId;
		});

	if (it == bookings.end()) {
		return false;
	}

	bookings.erase(it);
	saveToFile();
	return true;
}

const std::vector<Reservation>& ReservationManager::getAllReservations() const {
	return bookings;
}

bool ReservationManager::isEmpty() const {
	return bookings.empty();
}
