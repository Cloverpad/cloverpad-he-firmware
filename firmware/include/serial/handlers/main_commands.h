#pragma once

#include <configuration/configuration_handler.h>
#include <protocol-commands.pb.h>

/// @brief Handles a `FirmwareVersion` command.
/// @param response The response object being sent back to the host.
void handle_firmware_version(protocol_Response &response);

/// @brief Handles a `GetConfiguration` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
void handle_get_configuration(protocol_Response &response, ConfigurationHandler &configuration_handler);

/// @brief Handles a `SaveConfiguration` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
void handle_save_configuration(protocol_Response &response, ConfigurationHandler &configuration_handler);

/// @brief Handles a `FactoryReset` command.
/// @param response The response object being sent back to the host.
/// @param configuration_handler The current `ConfigurationHandler` instance in use.
/// @note While this is running, all keypad inputs will be disabled.
void handle_factory_reset(protocol_Response &response, ConfigurationHandler &configuration_handler);
