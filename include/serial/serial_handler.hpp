#pragma once

#include <commands.pb.h>

#include <configuration/configuration_handler.hpp>
#include <input/input_handler.hpp>

/// @brief Class for handling commands sent over the serial interface.
class SerialHandler
{
private:
    /// @brief Buffer used for reading commands.
    uint8_t command_buffer[256] = {};

    void send_response(cloverpad_Response &response);

public:
    /// @brief Attempts to handle the next command (if one is available).
    /// @param configuration_manager The current configuration manager instance in use.
    /// @param input_handler The current input handler instance in use.
    void handle_next_command(ConfigurationHandler &configuration_manager, InputHandler &input_handler);
};
