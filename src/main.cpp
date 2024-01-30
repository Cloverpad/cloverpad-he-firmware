#include <Arduino.h>
#include <EEPROM.h>
#include <Keyboard.h>

#include <main.h>

/// @brief 1 ms (1000 Hz) Polling Rate
int usb_hid_poll_interval = 1;

/// @brief Setup code for Core 0.
void setup()
{
    // Initialise USB interfaces
    Keyboard.begin();
    Serial.begin();

    // Initialise EEPROM with 4kB memory
    EEPROM.begin(4096);

    // Setup ADC to read as 12-bit values (native for RP2040)
    analogReadResolution(12);

    // Signal to Core 1 that it's finished setting up
    rp2040.fifo.push(1);
}

/// @brief Setup code for Core 1.
void setup1()
{
    // Wait for Core 0 to finish setting up
    rp2040.fifo.pop();
}

/// @brief Main loop for Core 0. Used to handle serial communication and RGB effects.
void loop()
{
    serial_handler.handle_next_command(configuration_manager, input_handler);
}

/// @brief Main loop for Core 1. Used to handle reading hall effect inputs and sending HID commands.
void loop1()
{
    input_handler.handle_next(configuration_manager.keypad_configuration.he_keys);
}
