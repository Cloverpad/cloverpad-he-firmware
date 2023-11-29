#include <configuration.h>

HEKeyConfiguration get_default_he_key_config(size_t index)
{
    // Clamp the index (just in case)
    if (index > HE_KEY_COUNT)
    {
        index = HE_KEY_COUNT - 1;
    }

    HEKeyConfiguration configuration = {.keycode = get_default_keycode(index)};
    return configuration;
}

KeypadConfiguration get_default_keypad_config()
{
    KeypadConfiguration configuration = {};
    for (size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        configuration.he_keys[i] = get_default_he_key_config(i);
    }
}
