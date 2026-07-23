#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
BUILD_DIR="$ROOT_DIR/build"
CLI_OUTPUT="$BUILD_DIR/hotel_system"
CLI_ALT_OUTPUT="$BUILD_DIR/hotel_system_cli"
GUI_OUTPUT="$BUILD_DIR/hotel_gui"

mkdir -p "$BUILD_DIR"

if [ -x "$CLI_OUTPUT" ]; then
	exec "$CLI_OUTPUT"
fi

if [ -x "$CLI_ALT_OUTPUT" ]; then
	exec "$CLI_ALT_OUTPUT"
fi

if [ -x "$GUI_OUTPUT" ]; then
	exec "$GUI_OUTPUT"
fi

/usr/bin/clang++ -std=c++17 -stdlib=libc++ "$ROOT_DIR/main.cpp" "$ROOT_DIR/ReservationManager.cpp" -o "$CLI_OUTPUT"
exec "$CLI_OUTPUT"