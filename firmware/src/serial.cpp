#include <Arduino.h>

#include <pb_decode.h>
#include <pb_encode.h>

#include <commands.pb.h>
#include <constants.h>
#include <serial.h>

void SerialHandler::handle_next_command()
{
    int available_bytes = Serial.available();
    if (available_bytes <= 0)
    {
        return;
    }

    // Attempt to read and parse the command that was just passed in
    int bytes_read = Serial.readBytes(this->command_buffer, available_bytes);
    models_commands_Command command = models_commands_Command_init_zero;
    pb_istream_t istream = pb_istream_from_buffer(this->command_buffer, bytes_read);
    bool status = pb_decode(&istream, models_commands_Command_fields, &command);

    if (!status)
    {
        // TODO: Send error response
        return;
    }

    // Determine the response based on what's been passed in
    models_commands_Response response = models_commands_Response_init_zero;

    switch (command.which_type)
    {
    case models_commands_Command_echo_tag:
    {
        models_commands_EchoResponse echo_response = models_commands_EchoResponse_init_zero;
        strcpy(echo_response.value, command.type.echo.value);

        response.code = models_commands_ResponseCode_SUCCESS;
        response.which_data = models_commands_Response_echo_tag;
        response.data.echo = echo_response;
        break;
    }

    case models_commands_Command_firmware_version_tag:
    {
        models_commands_FirmwareVersionResponse firmware_version_response = models_commands_FirmwareVersionResponse_init_zero;
        firmware_version_response.version = FIRMWARE_VERSION;

        response.code = models_commands_ResponseCode_SUCCESS;
        response.which_data = models_commands_Response_firmware_version_tag;
        response.data.firmware_version = firmware_version_response;
        break;
    }

    default:
    {
        response.code = models_commands_ResponseCode_UNSUPPORTED_COMMAND;
        break;
    }
    }

    // Send the response to the host
    // First, write how many bytes are present - this is an unsigned 32-bit integer in little endian
    // Then, write the actual data bytes from the encoded response
    pb_ostream_t ostream = pb_ostream_from_buffer(this->command_buffer, sizeof(this->command_buffer));
    pb_encode(&ostream, models_commands_Response_fields, &response);

    Serial.write((uint8_t)(ostream.bytes_written & 0xFF));
    Serial.write((uint8_t)(ostream.bytes_written >> 8));
    Serial.write((uint8_t)(ostream.bytes_written >> 16));
    Serial.write((uint8_t)(ostream.bytes_written >> 24));

    Serial.write(this->command_buffer, ostream.bytes_written);
}
