#include <Arduino.h>

#include <constants.h>
#include <input/cloverpad_keyboard.h>
#include <input/input_handler.h>
#include <lerp.h>

void InputHandler::handle_next(HEKeyConfiguration he_key_configs[HE_KEY_COUNT])
{
    // Only process ADC readings if input handler is enabled
    if (!this->enabled)
    {
        return;
    }

    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
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

    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        // Calibrate the ADC value
        uint16_t calibrated_adc_value = lerp_adc(
            (uint16_t)RECIPROCAL_ADC_TOP,
            (uint16_t)RECIPROCAL_ADC_RANGE,
            he_key_configs[i].calibration_adc_top,
            he_key_configs[i].calibration_adc_bot,
            this->he_key_states[i].average_reading.current_average());

        // Determine the distance from the top of the switch
        // Clamp this to 0.0mm if it's too far away
        double current_dist = this->dist_lut.get_distance(calibrated_adc_value);
        double dist_from_top = max(AIR_GAP_TOP_MM - current_dist, 0.0);

        this->he_key_states[i].last_position_mm = dist_from_top;

        // Update the state depending on the configuration:
        // - If key is disabled, ensure it is released
        // - If rapid trigger is enabled, update based on recent key positions
        // - If rapid trigger is disabled, update based on actuation point
        if (!he_key_configs[i].enabled)
        {
            CloverpadKeyboard.release(he_key_configs[i].keycode);
        }
        else if (he_key_configs[i].rapid_trigger)
        {
            // TODO: Handle state if rapid trigger is enabled
        }
        else
        {
            // Determine if this key is past the actuation point
            bool pressed = dist_from_top >= he_key_configs[i].actuation_point_mm;
            this->he_key_states[i].pressed = pressed;

            if (pressed)
            {
                CloverpadKeyboard.press(he_key_configs[i].keycode);
            }
            else
            {
                CloverpadKeyboard.release(he_key_configs[i].keycode);
            }
        }
    }

    // After all key states have been updated, send the combined report to the host
    CloverpadKeyboard.sendReport();
}

void InputHandler::reset_he_key_states()
{
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        this->he_key_states[i] = HEKeyState{};
    }
}
