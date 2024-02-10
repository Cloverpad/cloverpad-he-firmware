#pragma once

#include <cstdint>

#include <configuration/he_key_configuration.hpp>
#include <constants.hpp>

/// @brief Top-level struct for this keypad's configuration.
/// @note This is stored in the EEPROM.
struct KeypadConfiguration
{
    /// @brief The configuration version.
    uint32_t configuration_version = CONFIGURATION_VERSION;

    /// @brief The name of this keypad.
    char name[64] = "Unnamed Keypad";

    /// @brief Whether to use the automatic calibration values when mapping ADC readings
    bool use_auto_calibration = true;

    /// @brief The configurations for each hall effect key on this keypad.
    HEKeyConfiguration he_keys[HE_KEY_COUNT] = {};
};
