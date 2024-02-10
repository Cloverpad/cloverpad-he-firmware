#pragma once

#include <configuration_handler.hpp>
#include <input_handler.hpp>
#include <commands.pb.h>

/// @brief Handles a `FirmwareVersion` command.
/// @param response The response object being sent back to the host.
void handle_firmware_version(cloverpad_Response &response);

/// @brief Handles a `GetConfiguration` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
void handle_get_configuration(cloverpad_Response &response, ConfigurationHandler &configuration_handler);

/// @brief Handles a `SaveConfiguration` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
void handle_save_configuration(cloverpad_Response &response, ConfigurationHandler &configuration_handler);

/// @brief Handles a `FactoryReset` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
/// @note While this is running, all keypad inputs will be disabled.
void handle_factory_reset(cloverpad_Response &response, ConfigurationHandler &configuration_handler);

/// @brief Handles a `SetMainConfiguration` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
/// @param data The command data.
void handle_set_main_configuration(
    cloverpad_Response &response,
    ConfigurationHandler &configuration_handler,
    cloverpad_SetMainConfiguration &data);

/// @brief Handles a `RevertMainConfiguration` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
void handle_revert_main_configuration(
    cloverpad_Response &response,
    ConfigurationHandler &configuration_handler);

/// @brief Handles a `SetHEKeyConfiguration` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
/// @param input_handler The current `InputHandler` instance in use.
/// @param data The command data.
/// @note While this is running, HID commands will not be generated as the key configuration is changing.
void handle_set_he_key_configuration(
    cloverpad_Response &response,
    ConfigurationHandler &configuration_handler,
    InputHandler &input_handler,
    cloverpad_SetHEKeyConfiguration &data);

/// @brief Handles a `RevertHEKeyConfiguration` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
/// @param input_handler The current `InputHandler` instance in use.
/// @note While this is running, HID commands will not be generated as the key configuration is changing.
void handle_revert_he_key_configuration(
    cloverpad_Response &response,
    ConfigurationHandler &configuration_handler,
    InputHandler &input_handler);
