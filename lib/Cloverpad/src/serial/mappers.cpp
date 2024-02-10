#include <algorithm>

#include <serial/mappers.hpp>

void copy_main_configuration(KeypadConfiguration &source, KeypadConfiguration &target)
{
    target.configuration_version = source.configuration_version;
    strncpy(target.name, source.name, sizeof(source.name));
}

void copy_he_key_configuration(HEKeyConfiguration &source, HEKeyConfiguration &target, bool include_keycode)
{
    target.enabled = source.enabled;

    if (include_keycode)
    {
        target.keycode = source.keycode;
    }

    target.rapid_trigger = source.rapid_trigger;
    target.actuation_point_mm = source.actuation_point_mm;
    target.up_sensitivity_mm = source.up_sensitivity_mm;
    target.down_sensitivity_mm = source.down_sensitivity_mm;
    target.upper_deadzone_mm = source.upper_deadzone_mm;
    target.lower_deadzone_mm = source.lower_deadzone_mm;
}

cloverpad_MainConfiguration map_main_configuration(KeypadConfiguration &keypad_configuration)
{
    cloverpad_MainConfiguration mapped = cloverpad_MainConfiguration_init_zero;
    mapped.configuration_version = keypad_configuration.configuration_version;
    strncpy(mapped.name, keypad_configuration.name, std::min(sizeof(mapped.name), sizeof(keypad_configuration.name)));

    return mapped;
}

KeypadConfiguration map_main_configuration(cloverpad_MainConfiguration &main_configuration)
{
    KeypadConfiguration mapped = {};
    mapped.configuration_version = CONFIGURATION_VERSION;
    strncpy(mapped.name, main_configuration.name, std::min(sizeof(mapped.name), sizeof(main_configuration.name)));

    return mapped;
}

cloverpad_HEKeyConfiguration map_he_key_configuration(HEKeyConfiguration &he_key_configuration)
{
    cloverpad_HEKeyConfiguration mapped = cloverpad_HEKeyConfiguration_init_zero;
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

HEKeyConfiguration map_he_key_configuration(cloverpad_HEKeyConfiguration &he_key_configuration)
{
    HEKeyConfiguration mapped = {};
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

cloverpad_FullConfigurationResponse map_full_configuration(KeypadConfiguration &keypad_configuration, bool modified)
{
    cloverpad_FullConfigurationResponse mapped = cloverpad_FullConfigurationResponse_init_zero;
    mapped.modified = modified;

    mapped.has_main_configuration = true;
    mapped.main_configuration = map_main_configuration(keypad_configuration);

    mapped.he_key_configurations_count = HE_KEY_COUNT;
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        mapped.he_key_configurations[i] = map_he_key_configuration(keypad_configuration.he_keys[i]);
    }

    return mapped;
}
