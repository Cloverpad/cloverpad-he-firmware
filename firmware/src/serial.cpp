#include <Arduino.h>

#include <pb_decode.h>
#include <pb_encode.h>

#include <protocol-commands.pb.h>
#include <constants.h>
#include <serial_handler.h>

void SerialHandler::send_response(protocol_Response *response)
{
    // Send how many encoded bytes are present (as unsigned 32-bit integer)
    // Then send the actual data bytes
    pb_ostream_t ostream = pb_ostream_from_buffer(this->command_buffer, sizeof(this->command_buffer));
    pb_encode(&ostream, protocol_Response_fields, response);

    Serial.write((uint8_t)(ostream.bytes_written & 0xFF));
    Serial.write((uint8_t)(ostream.bytes_written >> 8));
    Serial.write((uint8_t)(ostream.bytes_written >> 16));
    Serial.write((uint8_t)(ostream.bytes_written >> 24));

    Serial.write(this->command_buffer, ostream.bytes_written);
}

void SerialHandler::handle_next_command(ConfigurationManager &configuration_manager, InputHandler &input_handler)
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
        send_response(&response);
        return;
    }

    // Determine the response based on what's been passed in
    switch (command.which_type)
    {
    case protocol_Command_echo_tag:
    {
        protocol_EchoResponse echo_response = protocol_EchoResponse_init_zero;
        strcpy(echo_response.value, command.type.echo.value);

        response.code = protocol_ResponseCode_SUCCESS;
        response.which_data = protocol_Response_echo_tag;
        response.data.echo = echo_response;
        break;
    }

    case protocol_Command_firmware_version_tag:
    {
        protocol_FirmwareVersionResponse firmware_version_response = protocol_FirmwareVersionResponse_init_zero;
        firmware_version_response.version = FIRMWARE_VERSION;

        response.code = protocol_ResponseCode_SUCCESS;
        response.which_data = protocol_Response_firmware_version_tag;
        response.data.firmware_version = firmware_version_response;
        break;
    }

    default:
    {
        response.code = protocol_ResponseCode_UNSUPPORTED_COMMAND;
        break;
    }
    }

    send_response(&response);
}
