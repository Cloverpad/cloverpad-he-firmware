#pragma once

/// @brief Class for handling commands sent over the serial interface.
class SerialHandler
{
private:
    /// @brief Buffer used for reading commands.
    uint8_t command_buffer[256] = {};

public:
    // TODO: Add other configurations

    /// @brief Attempts to handle the next command (if one is available).
    void handle_next_command();
};
