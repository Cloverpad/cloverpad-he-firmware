#pragma once

#include <configuration.h>
#include <input.h>
#include <serial.h>

/// @brief The top-level configuration currently in use.
KeypadConfiguration configuration = {};

/// @brief The serial handler used by the main loop.
SerialHandler serial_handler = SerialHandler();

/// @brief The input handler used by the main loop.
InputHandler input_handler = InputHandler();
