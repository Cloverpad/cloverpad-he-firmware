#pragma once

#include <configuration/he_key_configuration.h>
#include <configuration/keypad_configuration.h>
#include <protocol-commands.pb.h>

/// @brief Copies the main configuration details between structs.
/// @param source The source configuration.
/// @param target The target configuration.
void copy_main_configuration(KeypadConfiguration &source, KeypadConfiguration &target);

/// @brief Copies the hall effect key configuration details between structs.
/// @param source The source configuration.
/// @param target The target configuration.
/// @param include_keycode Whether to include the keycode when copying values.
void copy_he_key_configuration(HEKeyConfiguration &source, HEKeyConfiguration &target, bool include_keycode);

/// @brief Maps the main configuration details into the Protobuf format.
/// @param keypad_configuration The top-level keypad configuration to convert.
/// @return The mapped Protobuf configuration struct.
protocol_MainConfiguration map_main_configuration(KeypadConfiguration &keypad_configuration);

/// @brief Maps the main configuration details into the firmware format.
/// @param main_configuration The Protobuf configuration to convert.
/// @return The mapped firmware configuration struct (with only top-level details).
KeypadConfiguration map_main_configuration(protocol_MainConfiguration &main_configuration);

/// @brief Maps the hall effect key configuration details into the Protobuf format.
/// @param he_key_configuration The hall effect key configuration to convert.
/// @return The mapped Protobuf configuration struct.
protocol_HEKeyConfiguration map_he_key_configuration(HEKeyConfiguration &he_key_configuration);

/// @brief Maps the hall effect key configuration details into the firmware format.
/// @param he_key_configuration The hall effect key configuration to convert.
/// @return The mapped firmare configuration struct.
HEKeyConfiguration map_he_key_configuration(protocol_HEKeyConfiguration &he_key_configuration);

/// @brief Maps the full configuration details into the Protobuf format.
/// @param keypad_configuration The top-level keypad configuration to convert.
/// @param modified Whether this keypad configuration was previously modified.
/// @return The mapped Protobuf configuration struct.
protocol_FullConfigurationResponse map_full_configuration(KeypadConfiguration &keypad_configuration, bool modified);
