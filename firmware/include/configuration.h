#pragma once

#include <stddef.h>
#include <stdint.h>

#include <constants.h>

/// @brief Holds the configuration for a hall effect key.
struct HEKeyConfiguration
{
    /// @brief Whether this hall effect key is enabled (i.e. keypresses are sent to the host).
    bool enabled = true;

    /// @brief The ASCII keycode assigned to this hall effect key.
    char keycode = '\0';

    /// @brief Whether rapid trigger is enabled on this hall effect key.
    bool rapid_trigger = false;

    /// @brief The actuation point to use when rapid trigger is disabled, in millimetres.
    /// @note Must be in the range [0.1, 4.0].
    double actuation_point_mm = 1.2;

    /// @brief The upward sensitivity to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.1, 2.0].
    double up_sensitivity_mm = 0.3;

    /// @brief The downward sensitivity to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.1, 2.0].
    double down_sensitivity_mm = 0.3;

    /// @brief The upper deadzone to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.0, 4.0] and less than `lower_deadzone_mm`.
    ///       The key is always considered as released when above this threshold.
    double upper_deadzone_mm = 0.4;

    /// @brief The lower deadzone to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.0, 4.0] and greater than `upper_deadzone_mm`.
    ///       The key is always considered as pressed when below this threshold.
    double lower_deadzone_mm = 3.6;
};

/// @brief Top-level struct for this keypad's configuration.
/// @note This is stored in the virtual EEPROM.
struct KeypadConfiguration
{
    /// @brief The configuration version.
    uint32_t configuration_version = CONFIGURATION_VERSION;

    /// @brief The name of this keypad.
    char name[64] = "Unnamed Keypad";

    /// @brief The configurations for each hall effect key on this keypad.
    HEKeyConfiguration he_keys[HE_KEY_COUNT] = {};
};

char get_default_keycode(size_t index)
{
    switch (index)
    {
    case 0:
        return 'z';

    case 1:
        return 'x';

    case 2:
        return 'c';

    default:
        return '\0';
    }
}

/// @brief Gets the default configuration for a hall effect key.
/// @param index The index of the hall effect key.
/// @return The default configuration for this hall effect key, with an assigned keycode (Z, X or C).
HEKeyConfiguration get_default_he_key_config(size_t index);

/// @brief Gets the default keypad configuration.
/// @return The default keypad configuration, including all sub-configurations.
KeypadConfiguration get_default_keypad_config();
