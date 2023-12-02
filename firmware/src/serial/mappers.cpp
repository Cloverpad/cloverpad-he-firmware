#include <serial/mappers.h>

protocol_MainConfiguration map_main_configuration(KeypadConfiguration &keypad_configuration)
{
    protocol_MainConfiguration mapped = protocol_MainConfiguration_init_zero;
    mapped.configuration_version = keypad_configuration.configuration_version;
    strcpy(mapped.name, keypad_configuration.name);

    return mapped;
}

protocol_HEKeyConfiguration map_he_key_configuration(HEKeyConfiguration &he_key_configuration)
{
    protocol_HEKeyConfiguration mapped = protocol_HEKeyConfiguration_init_zero;
    mapped.enabled = he_key_configuration.enabled;
    mapped.keycode = he_key_configuration.keycode;
    mapped.rapid_trigger = he_key_configuration.rapid_trigger;
    mapped.actuation_point_mm = he_key_configuration.actuation_point_mm;
    mapped.up_sensitivity_mm = he_key_configuration.up_sensitivity_mm;
    mapped.down_sensitivity_mm = he_key_configuration.down_sensitivity_mm;
    mapped.upper_deadzone_mm = he_key_configuration.upper_deadzone_mm;
    mapped.lower_deadzone_mm = he_key_configuration.lower_deadzone_mm;

    return mapped;
}

protocol_FullConfigurationResponse map_full_configuration(KeypadConfiguration &keypad_configuration, bool modified)
{
    protocol_FullConfigurationResponse mapped = protocol_FullConfigurationResponse_init_zero;
    mapped.modified = modified;

    mapped.has_main_configuration = true;
    mapped.main_configuration = map_main_configuration(keypad_configuration);

    mapped.he_key_configurations_count = HE_KEY_COUNT;
    for (size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        mapped.he_key_configurations[i] = map_he_key_configuration(keypad_configuration.he_keys[i]);
    }

    return mapped;
}
