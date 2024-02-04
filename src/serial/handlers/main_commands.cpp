#include <Arduino.h>

#include <serial/handlers/main_commands.h>
#include <serial/mappers.h>

void handle_firmware_version(cloverpad_Response &response)
{
    cloverpad_FirmwareVersionResponse firmware_version_response = cloverpad_FirmwareVersionResponse_init_zero;
    firmware_version_response.version = FIRMWARE_VERSION;

    response.code = cloverpad_ResponseCode_SUCCESS;
    response.which_data = cloverpad_Response_firmware_version_tag;
    response.data.firmware_version = firmware_version_response;
}

void handle_get_configuration(cloverpad_Response &response, ConfigurationHandler &configuration_handler)
{
    response.code = cloverpad_ResponseCode_SUCCESS;
    response.which_data = cloverpad_Response_get_configuration_tag;
    response.data.get_configuration = map_full_configuration(
        configuration_handler.keypad_configuration,
        configuration_handler.modified);
}

void handle_save_configuration(cloverpad_Response &response, ConfigurationHandler &configuration_handler)
{
    configuration_handler.save_keypad_configuration();

    response.code = cloverpad_ResponseCode_SUCCESS;
    response.which_data = cloverpad_Response_save_configuration_tag;
    response.data.save_configuration = map_full_configuration(
        configuration_handler.keypad_configuration,
        configuration_handler.modified);
}

void handle_factory_reset(cloverpad_Response &response, ConfigurationHandler &configuration_handler)
{
    // Idle Core 1 while the configuration is being reset, to avoid misinputs
    rp2040.idleOtherCore();

    configuration_handler.keypad_configuration = configuration_handler.get_default_keypad_config();
    configuration_handler.modified = true;
    configuration_handler.save_keypad_configuration();

    rp2040.resumeOtherCore();

    response.code = cloverpad_ResponseCode_SUCCESS;
    response.which_data = cloverpad_Response_factory_reset_tag;
    response.data.factory_reset = map_full_configuration(
        configuration_handler.keypad_configuration,
        configuration_handler.modified);
}
