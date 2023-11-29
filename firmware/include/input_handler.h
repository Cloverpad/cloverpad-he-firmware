#pragma once

#include <configuration.h>
#include <constants.h>
#include <state.h>

/// @brief Class for handling inputs from hall effect keys and sending HID commands to the host.
class InputHandler
{
public:
    /// @brief The current states for each hall effect key.
    HEKeyState he_key_states[HE_KEY_COUNT] = {};

    /// @brief Handles the next set of input readings and updates the key states.
    /// @param adc_input The current `ADCInput` instance in use. This should be configured to read from 3 pins.
    void handle_next(HEKeyConfiguration he_key_configs[HE_KEY_COUNT]);
};
