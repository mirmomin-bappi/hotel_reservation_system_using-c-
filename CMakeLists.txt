cmake_minimum_required(VERSION 3.16)
project(HotelReservation)

set(CMAKE_CXX_STANDARD 17)

add_library(reservationlib ReservationManager.cpp)

add_executable(hotel_system main.cpp)
target_link_libraries(hotel_system PRIVATE reservationlib)

add_executable(hotel_system_cli main.cpp)
target_link_libraries(hotel_system_cli PRIVATE reservationlib)

find_package(Qt6 COMPONENTS Widgets QUIET)
if (Qt6_FOUND)
	add_executable(hotel_gui gui.cpp)
	target_link_libraries(hotel_gui PRIVATE reservationlib Qt6::Widgets)
else()
	message(STATUS "Qt6 Widgets not found; GUI target will not be built")
endif()
