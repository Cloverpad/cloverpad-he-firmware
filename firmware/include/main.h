#pragma once

#include <configuration_manager.h>
#include <input_handler.h>
#include <serial_handler.h>

/// @brief The configuration manager currently in use.
ConfigurationManager configuration_manager = ConfigurationManager();

/// @brief The serial handler used by the main loop.
SerialHandler serial_handler = SerialHandler();

/// @brief The input handler used by the main loop.
InputHandler input_handler = InputHandler();
