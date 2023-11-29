#pragma once

#include <configuration.h>

/// @brief The address in EEPROM where the keypad configuration is stored.
#define EEPROM_CONFIGURATION_ADDR 0

class ConfigurationManager
{
public:
    /// @brief The keypad configuration that is currently active.
    KeypadConfiguration keypad_configuration = get_default_keypad_config();

    /// @brief Whether the keypad configuration was modified via serial commands and require saving to EEPROM.
    bool modified = false;

    /// @brief Loads the keypad configuration from EEPROM.
    /// @note If the configuration version doesn't match the version supported by this firmware, it will be replaced with the default configuration.
    void load_keypad_configuration();

    /// @brief Saves the current keypad configuration to EEPROM (if modifications were made).
    /// @return True if the keypad configuration was saved, false if no modifications were made since the last load.
    bool save_keypad_configuration();

    /// @brief Gets the default keycode for a particular key.
    /// @param index The index of the key. This is clamped to the range [0, HE_KEY_COUNT].
    /// @return The default keycode for this key index.
    char ConfigurationManager::get_default_keycode(size_t index);

    /// @brief Gets the default configuration for a hall effect key.
    /// @param index The index of the hall effect key.
    /// @return The default configuration for this hall effect key, with an assigned keycode (Z, X or C).
    HEKeyConfiguration get_default_he_key_config(size_t index);

    /// @brief Gets the default keypad configuration.
    /// @return The default keypad configuration, including all sub-configurations.
    KeypadConfiguration get_default_keypad_config();
};
