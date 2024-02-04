#pragma once

#include <configuration/configuration_handler.h>
#include <input/cloverpad_keyboard.h>
#include <input/input_handler.h>
#include <serial/serial_handler.h>

/// @brief The current keyboard configuration to use.
CloverpadKeyboard_ CloverpadKeyboard;

/// @brief The configuration manager currently in use.
ConfigurationHandler configuration_manager = ConfigurationHandler();

/// @brief The serial handler used by the main loop.
SerialHandler serial_handler = SerialHandler();

/// @brief The input handler used by the main loop.
InputHandler input_handler = InputHandler();
