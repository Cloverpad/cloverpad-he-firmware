#pragma once

#include <configuration/he_key_configuration.h>
#include <constants.h>
#include <state.h>

/// @brief Class for handling inputs from hall effect keys and sending HID commands to the host.
class InputHandler
{
public:
    /// @brief Whether HID commands will be generated. This should be set to false when the configuration is being updated.
    bool enabled = true;

    /// @brief The current states for each hall effect key.
    HEKeyState he_key_states[HE_KEY_COUNT] = {};

    /// @brief Handles the next set of input readings and updates the key states.
    /// @param he_key_configs The current hall effect key configurations.
    void handle_next(HEKeyConfiguration he_key_configs[HE_KEY_COUNT]);

    /// @brief Resets the hall effect key states to their initial values.
    void reset_he_key_states();
};
