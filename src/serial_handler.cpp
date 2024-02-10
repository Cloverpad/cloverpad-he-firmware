#include <Arduino.h>

#include <pb_decode.h>
#include <pb_encode.h>

#include <constants.hpp>
#include <commands.pb.h>
#include <command_handlers.hpp>
#include <serial_handler.hpp>

void SerialHandler::send_response(cloverpad_Response &response)
{
    // Send how many encoded bytes are present (as unsigned 32-bit integer)
    // Then send the actual data bytes
    pb_ostream_t ostream = pb_ostream_from_buffer(this->command_buffer, sizeof(this->command_buffer));
    pb_encode(&ostream, cloverpad_Response_fields, &response);

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
    std::size_t command_length = (std::size_t)(this->command_buffer[0]) |
                            (std::size_t)(this->command_buffer[1] << 8) |
                            (std::size_t)(this->command_buffer[2] << 16) |
                            (std::size_t)(this->command_buffer[3] << 24);

    // Then, read in the command bytes
    Serial.readBytes(this->command_buffer, command_length);
    cloverpad_Command command = cloverpad_Command_init_zero;
    pb_istream_t istream = pb_istream_from_buffer(this->command_buffer, command_length);
    bool status = pb_decode(&istream, cloverpad_Command_fields, &command);

    // Initialise output response
    cloverpad_Response response = cloverpad_Response_init_zero;

    if (!status)
    {
        // Something went wrong while decoding
        response.code = cloverpad_ResponseCode_DECODE_ERROR;
        send_response(response);
        return;
    }

    // Determine the response based on what's been passed in
    switch (command.which_type)
    {
    case cloverpad_Command_firmware_version_tag:
        handle_firmware_version(response);
        break;

    case cloverpad_Command_get_configuration_tag:
        handle_get_configuration(response, configuration_handler);
        break;

    case cloverpad_Command_save_configuration_tag:
        handle_save_configuration(response, configuration_handler);
        break;

    case cloverpad_Command_factory_reset_tag:
        handle_factory_reset(response, configuration_handler);
        break;

    case cloverpad_Command_set_main_configuration_tag:
        handle_set_main_configuration(response, configuration_handler, command.type.set_main_configuration);
        break;

    case cloverpad_Command_revert_main_configuration_tag:
        handle_revert_main_configuration(response, configuration_handler);
        break;

    case cloverpad_Command_set_he_key_configuration_tag:
        handle_set_he_key_configuration(response, configuration_handler, input_handler, command.type.set_he_key_configuration);
        break;

    case cloverpad_Command_revert_he_key_configuration_tag:
        handle_revert_he_key_configuration(response, configuration_handler, input_handler);
        break;

    default:
        response.code = cloverpad_ResponseCode_UNSUPPORTED_COMMAND;
        break;
    }

    send_response(response);
}
