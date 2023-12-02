#pragma once

#include <configuration/he_key_configuration.h>
#include <configuration/keypad_configuration.h>
#include <protocol-commands.pb.h>

/// @brief Maps the main configuration details into the Protobuf response format.
/// @param keypad_configuration The top-level keypad configuration to convert.
/// @return The mapped Protobuf response.
protocol_MainConfiguration map_main_configuration(KeypadConfiguration &keypad_configuration);

/// @brief Maps the hall effect key configuration details into the Protobuf response format.
/// @param he_key_configuration The hall effect key configuration to convert.
/// @return The mapped Protobuf response.
protocol_HEKeyConfiguration map_he_key_configuration(HEKeyConfiguration &he_key_configuration);

/// @brief Maps the full configuration details into the Protobuf response format.
/// @param keypad_configuration The top-level keypad configuration to convert.
/// @param modified Whether this keypad configuration was previously modified.
/// @return The mapped Protobuf response.
protocol_FullConfigurationResponse map_full_configuration(KeypadConfiguration &keypad_configuration, bool modified);
