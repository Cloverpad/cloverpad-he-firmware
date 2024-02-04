#pragma once

#include <stdint.h>

#include <configuration/he_key_configuration.h>
#include <constants.h>

/// @brief Top-level struct for this keypad's configuration.
/// @note This is stored in the EEPROM.
struct KeypadConfiguration
{
    /// @brief The configuration version.
    uint32_t configuration_version = CONFIGURATION_VERSION;

    /// @brief The name of this keypad.
    char name[64] = "Unnamed Keypad";

    /// @brief The configurations for each hall effect key on this keypad.
    HEKeyConfiguration he_keys[HE_KEY_COUNT] = {};
};
