# Hotel reservation system

This repository contains a simple hotel reservation system with a menu-driven CLI and an optional Qt GUI.

Build the C++ CLI binary from the project root:
```bash
/usr/bin/clang++ -std=c++17 -stdlib=libc++ main.cpp ReservationManager.cpp -o hotel_system
```

Or use the local runner script, which starts the CLI when it is available and falls back to the GUI only if needed:
```bash
bash run.sh
```

To build the GUI on a machine with Qt 6 installed:
```bash
cmake -S . -B build
cmake --build build
./build/hotel_gui
```

CLI usage examples:
```bash
./hotel_system list
./hotel_system add "Alice" "alice@example.com" "123" "01/08/2026" 1
./hotel_system remove 3
```

