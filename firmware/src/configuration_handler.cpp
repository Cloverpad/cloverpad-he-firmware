#include <EEPROM.h>

#include <configuration_handler.h>

void ConfigurationHandler::load_keypad_configuration()
{
    KeypadConfiguration configuration = {};
    EEPROM.get(EEPROM_CONFIGURATION_ADDR, configuration);

    if (configuration.configuration_version != CONFIGURATION_VERSION)
    {
        this->keypad_configuration = get_default_keypad_config();
        save_keypad_configuration();
    }
}

bool ConfigurationHandler::save_keypad_configuration()
{
    if (!this->modified)
    {
        return false;
    }

    EEPROM.put(EEPROM_CONFIGURATION_ADDR, this->keypad_configuration);
    this->modified = false;

    return true;
}

char ConfigurationHandler::get_default_keycode(size_t index)
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

HEKeyConfiguration ConfigurationHandler::get_default_he_key_config(size_t index)
{
    // Clamp the index (just in case)
    if (index > HE_KEY_COUNT)
    {
        index = HE_KEY_COUNT - 1;
    }

    HEKeyConfiguration configuration = {.keycode = get_default_keycode(index)};
    return configuration;
}

KeypadConfiguration ConfigurationHandler::get_default_keypad_config()
{
    KeypadConfiguration configuration = {};
    for (size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        configuration.he_keys[i] = get_default_he_key_config(i);
    }

    return configuration;
}
