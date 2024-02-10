#pragma once

#include <configuration_handler.hpp>
#include <input_handler.hpp>
#include <commands.pb.h>

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
