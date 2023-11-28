#pragma once

#include <constants.h>
#include <keys_he.h>

/// @brief Top-level struct for this keypad's configuration.
/// @note This is stored in the virtual EEPROM.
struct KeypadConfiguration {
    /// @brief The configuration version.
    uint32_t configuration_version = CONFIGURATION_VERSION;

    /// @brief The configurations for each hall effect key on this keypad.
    HEKeyConfig he_keys[HE_KEY_COUNT];
};
