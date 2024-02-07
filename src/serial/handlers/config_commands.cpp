#include <serial/handlers/config_commands.h>
#include <serial/mappers.h>

void handle_set_main_configuration(
    cloverpad_Response &response,
    ConfigurationHandler &configuration_handler,
    cloverpad_SetMainConfiguration &data)
{
    // Ensure that the configuration details were provided
    if (!data.has_configuration)
    {
        response.code = cloverpad_ResponseCode_INVALID_PARAMETERS;
        return;
    }

    // Map and copy the main configuration details over
    KeypadConfiguration new_config = map_main_configuration(data.configuration);
    copy_main_configuration(new_config, configuration_handler.keypad_configuration);
    configuration_handler.modified = true;

    // Prepare the response to send back to the host
    cloverpad_MainConfigurationResponse response_data = cloverpad_MainConfigurationResponse_init_zero;
    response_data.has_main_configuration = true;
    response_data.main_configuration = data.configuration;

    response.code = cloverpad_ResponseCode_SUCCESS;
    response.which_data = cloverpad_Response_set_main_configuration_tag;
    response.data.set_main_configuration = response_data;
}

void handle_revert_main_configuration(
    cloverpad_Response &response,
    ConfigurationHandler &configuration_handler)
{
    // Load the configuration from EEPROM and replace the main configuration details
    KeypadConfiguration eeprom_config = configuration_handler.load_keypad_configuration();
    copy_main_configuration(eeprom_config, configuration_handler.keypad_configuration);
    configuration_handler.modified = true;

    // Prepare the response to send back to the host
    cloverpad_MainConfigurationResponse response_data = cloverpad_MainConfigurationResponse_init_zero;
    response_data.has_main_configuration = true;
    response_data.main_configuration = map_main_configuration(eeprom_config);

    response.code = cloverpad_ResponseCode_SUCCESS;
    response.which_data = cloverpad_Response_revert_main_configuration_tag;
    response.data.set_main_configuration = response_data;
}

void handle_set_he_key_configuration(
    cloverpad_Response &response,
    ConfigurationHandler &configuration_handler,
    InputHandler &input_handler,
    cloverpad_SetHEKeyConfiguration &data)
{
    // Ensure that the configuration details were provided
    // If an index was specified, also ensure it is valid
    if (!data.has_configuration || (data.has_index && data.index >= HE_KEY_COUNT))
    {
        response.code = cloverpad_ResponseCode_INVALID_PARAMETERS;
        return;
    }

    // Map and verify the hall effect key configuration
    HEKeyConfiguration new_config = map_he_key_configuration(data.configuration);
    if (!verify_he_key_configuration(new_config))
    {
        response.code = cloverpad_ResponseCode_INVALID_PARAMETERS;
        return;
    }

    // Determine if we need to set this configuration for all keys or a specific key
    // Also pause input handler while these settings are being updated
    input_handler.change_mode(InputHandlerMode::Disabled);

    if (!data.has_index)
    {
        // Set for all keys
        for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
        {
            copy_he_key_configuration(new_config, configuration_handler.keypad_configuration.he_keys[i], false);
        }
    }
    else
    {
        // Set for the specified key
        copy_he_key_configuration(new_config, configuration_handler.keypad_configuration.he_keys[data.index], true);
    }

    // Reset key states and re-enable input handler
    input_handler.change_mode(InputHandlerMode::NormalInput);

    // Mark configuration as modified
    configuration_handler.modified = true;

    // Prepare the response to send back to the host
    cloverpad_SetHEKeyConfigurationResponse response_data = cloverpad_SetHEKeyConfigurationResponse_init_zero;
    response_data.has_index = data.has_index;
    response_data.index = data.index;
    response_data.has_configuration = true;
    response_data.configuration = data.configuration;

    response.code = cloverpad_ResponseCode_SUCCESS;
    response.which_data = cloverpad_Response_set_he_key_configuration_tag;
    response.data.set_he_key_configuration = response_data;
}

void handle_revert_he_key_configuration(
    cloverpad_Response &response,
    ConfigurationHandler &configuration_handler,
    InputHandler &input_handler)
{
    // Load the configuration from EEPROM and replace the hall effect key configurations
    // Also pause input handler while these settings are being updated
    KeypadConfiguration eeprom_config = configuration_handler.load_keypad_configuration();
    input_handler.change_mode(InputHandlerMode::Disabled);

    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        configuration_handler.keypad_configuration.he_keys[i] = eeprom_config.he_keys[i];
    }

    // Reset key states and re-enable input handler
    input_handler.change_mode(InputHandlerMode::NormalInput);

    // Mark configuration as modified
    configuration_handler.modified = true;

    // Prepare the response to send back to the host
    cloverpad_RevertHEKeyConfigurationResponse response_data = cloverpad_RevertHEKeyConfigurationResponse_init_zero;
    response_data.configurations_count = HE_KEY_COUNT;

    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        response_data.configurations[i] = map_he_key_configuration(eeprom_config.he_keys[i]);
    }

    response.code = cloverpad_ResponseCode_SUCCESS;
    response.which_data = cloverpad_Response_revert_he_key_configuration_tag;
    response.data.revert_he_key_configuration = response_data;
}
