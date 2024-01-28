#include <Arduino.h>

#include <pb_decode.h>
#include <pb_encode.h>

#include <constants.h>
#include <commands.pb.h>
#include <serial/handlers/config_commands.h>
#include <serial/handlers/main_commands.h>
#include <serial/serial_handler.h>

void SerialHandler::send_response(protocol_Response &response)
{
    // Send how many encoded bytes are present (as unsigned 32-bit integer)
    // Then send the actual data bytes
    pb_ostream_t ostream = pb_ostream_from_buffer(this->command_buffer, sizeof(this->command_buffer));
    pb_encode(&ostream, protocol_Response_fields, &response);

    Serial.write((uint8_t)(ostream.bytes_written & 0xFF));
    Serial.write((uint8_t)(ostream.bytes_written >> 8));
    Serial.write((uint8_t)(ostream.bytes_written >> 16));
    Serial.write((uint8_t)(ostream.bytes_written >> 24));

    Serial.write(this->command_buffer, ostream.bytes_written);
}

void SerialHandler::handle_next_command(ConfigurationHandler &configuration_handler, InputHandler &input_handler)
{
    int available_bytes = Serial.available();
    if (available_bytes <= 0)
    {
        return;
    }

    // Determine how many bytes are in the command
    Serial.readBytes(this->command_buffer, 4);
    size_t command_length = (size_t)(this->command_buffer[0]) |
                            (size_t)(this->command_buffer[1] << 8) |
                            (size_t)(this->command_buffer[2] << 16) |
                            (size_t)(this->command_buffer[3] << 24);

    // Then, read in the command bytes
    Serial.readBytes(this->command_buffer, command_length);
    protocol_Command command = protocol_Command_init_zero;
    pb_istream_t istream = pb_istream_from_buffer(this->command_buffer, command_length);
    bool status = pb_decode(&istream, protocol_Command_fields, &command);

    // Initialise output response
    protocol_Response response = protocol_Response_init_zero;

    if (!status)
    {
        // Something went wrong while decoding
        response.code = protocol_ResponseCode_DECODE_ERROR;
        send_response(response);
        return;
    }

    // Determine the response based on what's been passed in
    switch (command.which_type)
    {
    case protocol_Command_firmware_version_tag:
        handle_firmware_version(response);
        break;

    case protocol_Command_get_configuration_tag:
        handle_get_configuration(response, configuration_handler);
        break;

    case protocol_Command_save_configuration_tag:
        handle_save_configuration(response, configuration_handler);
        break;

    case protocol_Command_factory_reset_tag:
        handle_factory_reset(response, configuration_handler);
        break;

    case protocol_Command_set_main_configuration_tag:
        handle_set_main_configuration(response, configuration_handler, command.type.set_main_configuration);
        break;

    case protocol_Command_revert_main_configuration_tag:
        handle_revert_main_configuration(response, configuration_handler);
        break;

    case protocol_Command_set_he_key_configuration_tag:
        handle_set_he_key_configuration(response, configuration_handler, input_handler, command.type.set_he_key_configuration);
        break;

    case protocol_Command_revert_he_key_configuration_tag:
        handle_revert_he_key_configuration(response, configuration_handler, input_handler);
        break;

    default:
        response.code = protocol_ResponseCode_UNSUPPORTED_COMMAND;
        break;
    }

    send_response(response);
}
