#include <EEPROM.h>

#include <configuration/configuration_handler.hpp>

KeypadConfiguration ConfigurationHandler::load_keypad_configuration()
{
    KeypadConfiguration configuration = {.configuration_version = 0};
    EEPROM.get(EEPROM_CONFIGURATION_ADDR, configuration);

    if (configuration.configuration_version != CONFIGURATION_VERSION)
    {
        configuration = get_default_keypad_config();
    }

    return configuration;
}

bool ConfigurationHandler::save_keypad_configuration()
{
    if (!this->modified)
    {
        return false;
    }

    EEPROM.put(EEPROM_CONFIGURATION_ADDR, this->keypad_configuration);
    // EEPROM.commit(); // For now, don't commit anything to EEPROM, to save flash writes
    this->modified = false;

    return true;
}

char ConfigurationHandler::get_default_keycode(std::size_t index)
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

HEKeyConfiguration ConfigurationHandler::get_default_he_key_config(std::size_t index)
{
    // Clamp the index (just in case)
    if (index >= HE_KEY_COUNT)
    {
        index = HE_KEY_COUNT - 1;
    }

    HEKeyConfiguration configuration = {.keycode = get_default_keycode(index)};
    return configuration;
}

KeypadConfiguration ConfigurationHandler::get_default_keypad_config()
{
    KeypadConfiguration configuration = {};
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        configuration.he_keys[i] = get_default_he_key_config(i);
    }

    return configuration;
}
