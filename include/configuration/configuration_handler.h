#pragma once

#include <cstdint>

#include <configuration/he_key_configuration.h>
#include <configuration/keypad_configuration.h>

/// @brief The address in EEPROM where the keypad configuration is stored.
#define EEPROM_CONFIGURATION_ADDR 0

class ConfigurationHandler
{
public:
    /// @brief Whether the configuration has been modified since it was last saved to EEPROM.
    bool modified = false;

    /// @brief The keypad configuration that is currently active.
    KeypadConfiguration keypad_configuration = load_keypad_configuration();

    /// @brief Loads the keypad configuration from EEPROM.
    /// @return The keypad configuration currently stored in EEPROM.
    /// @note If the configuration version doesn't match the version supported by this firmware, the default configuration is returned instead.
    KeypadConfiguration load_keypad_configuration();

    /// @brief Saves the current keypad configuration to EEPROM (if modifications were made).
    /// @return True if the keypad configuration was saved, false if no modifications were made since the last load.
    bool save_keypad_configuration();

    /// @brief Gets the default keycode for a particular key.
    /// @param index The index of the key. This is clamped to the range [0, HE_KEY_COUNT].
    /// @return The default keycode for this key index.
    char get_default_keycode(std::size_t index);

    /// @brief Gets the default configuration for a hall effect key.
    /// @param index The index of the hall effect key.
    /// @return The default configuration for this hall effect key, with an assigned keycode (Z, X or C).
    HEKeyConfiguration get_default_he_key_config(std::size_t index);

    /// @brief Gets the default keypad configuration.
    /// @return The default keypad configuration, including all sub-configurations.
    KeypadConfiguration get_default_keypad_config();
};
