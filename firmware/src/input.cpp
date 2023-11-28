#include <input.h>

void InputHandler::handle_next(HEKeyConfiguration he_key_configs[HE_KEY_COUNT])
{
    for (size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        uint16_t adc_value = analogRead(HE_KEY_PIN(i));
        this->he_key_states[i].average_reading.push(adc_value);
    }

    // Only continuing processing if the moving averages have initialised
    // Assumes that all of them use the same number of samples.
    if (!this->he_key_states[0].average_reading.is_initialised())
    {
        return;
    }

    // TODO: Sensor calibration based on configuration
    // TODO: Update current state for each key
    // TODO: Sending HID commands to host
}
