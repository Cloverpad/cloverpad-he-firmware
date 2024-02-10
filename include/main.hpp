#pragma once

#include <configuration_handler.hpp>
#include <input/cloverpad_keyboard.hpp>
#include <input/input_handler.hpp>
#include <serial/serial_handler.hpp>

/// @brief The current keyboard configuration to use.
CloverpadKeyboard_ CloverpadKeyboard;

/// @brief The configuration manager currently in use.
ConfigurationHandler configuration_manager = ConfigurationHandler();

/// @brief The serial handler used by the main loop.
SerialHandler serial_handler = SerialHandler();

/// @brief The input handler used by the main loop.
InputHandler input_handler = InputHandler();
