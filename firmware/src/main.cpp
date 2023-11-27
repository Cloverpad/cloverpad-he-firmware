#include <Arduino.h>
#include <Keyboard.h>

#include <main.h>

/// @brief Setup code for Core 0.
void setup()
{
    Keyboard.begin();
    Serial.begin();

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
    serial_handler.handle_next_command();
}

/// @brief Main loop for Core 1. Used to handle reading hall effect inputs and sending HID commands.
void loop1()
{
}