#include <Arduino.h>

#include <constants.h>
#include <he_key_input.h>
#include <input/cloverpad_keyboard.h>
#include <input/input_handler.h>
#include <lerp.h>
#include <utils.h>

void InputHandler::reset()
{
    // Release any keys that are currently pressed
    CloverpadKeyboard.releaseAll();
    CloverpadKeyboard.sendReport();

    // Reset calibration min/max values and key states
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        this->calibration_min_values[i] = MAX_ADC;
        this->calibration_max_values[i] = MIN_ADC;
        this->he_key_states[i] = HEKeyState{};
    }
}

void InputHandler::change_mode(InputHandlerMode new_mode)
{
    // If the new mode is different, release all keys and reset key state
    if (new_mode == this->mode)
    {
        return;
    }

    this->mode = new_mode;
    this->reset();
}

void InputHandler::read_analog_values()
{
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        this->adc_buffer[i] = HE_KEY_PIN(i);
    }
}

void InputHandler::handle_next(HEKeyConfiguration he_key_configs[HE_KEY_COUNT], bool use_auto_calibration)
{
    switch (this->mode)
    {
    case InputHandlerMode::Disabled:
        return;
        break;

    case InputHandlerMode::ManualCalibration:
        this->handle_manual_calibration();
        break;

    case InputHandlerMode::NormalInput:
    default:
        this->handle_normal_input(he_key_configs, use_auto_calibration);
        break;
    }
}

void InputHandler::handle_normal_input(HEKeyConfiguration he_key_configs[HE_KEY_COUNT], bool use_auto_calibration)
{
    // Read the current ADC values for each key and update averages
    // Clamp the ADC value to avoid overflowing
    this->read_analog_values();

    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        this->he_key_states[i].average_reading.push(clamp(this->adc_buffer[i], MIN_ADC, MAX_ADC));
    }

    // Only continuing processing if the moving averages have initialised
    // Assumes that all of them use the same number of samples
    if (!this->he_key_states[0].average_reading.is_initialised())
    {
        return;
    }

    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        HEKeyConfiguration *config = &he_key_configs[i];
        HEKeyState *state = &this->he_key_states[i];

        // Update the ADC values for auto calibration
        uint16_t avg_adc_value = state->average_reading.current_average();
        this->calibration_max_values[i] = std::max(avg_adc_value, this->calibration_max_values[i]);
        this->calibration_min_values[i] = std::min(avg_adc_value, this->calibration_min_values[i]);

        // Calibrate the ADC value
        uint16_t calibrated_adc_value;

        if (use_auto_calibration)
        {
            // If auto calibration values are used, only use them if:
            // - They appear to be initialised (i.e. max > min)
            // - There is at least a 2.0mm gap between them
            uint16_t auto_calibration_max_value = this->calibration_max_values[i];
            uint16_t auto_calibration_min_value = this->calibration_min_values[i];

            if (auto_calibration_max_value <= auto_calibration_min_value)
            {
                continue;
            }

            // NOTE: This is distance from the sensor, i.e. lower ADC value = further away from sensor
            double dist_from_sensor_hi = this->dist_lut.get_distance(auto_calibration_min_value);
            double dist_from_sensor_lo = this->dist_lut.get_distance(auto_calibration_max_value);

            if (dist_from_sensor_hi - dist_from_sensor_lo <= 2.0)
            {
                continue;
            }

            // If these automatic calibration values are fine, use them to calibrate the main ADC reading
            calibrated_adc_value = lerp_adc(
                (uint16_t)RECIPROCAL_ADC_TOP,
                (uint16_t)RECIPROCAL_ADC_RANGE,
                auto_calibration_min_value,
                auto_calibration_max_value,
                avg_adc_value);
        }
        else
        {
            // Otherwise, use the manual calibration values
            calibrated_adc_value = lerp_adc(
                (uint16_t)RECIPROCAL_ADC_TOP,
                (uint16_t)RECIPROCAL_ADC_RANGE,
                config->calibration_adc_top,
                config->calibration_adc_bot,
                avg_adc_value);
        }

        // Determine the distance from the top of the switch
        // Clamp this to 0.0mm if it's too far away
        double dist_from_sensor = this->dist_lut.get_distance(calibrated_adc_value);
        double dist_from_top = max(AIR_GAP_TOP_MM - dist_from_sensor, 0.0);

        state->last_position_mm = dist_from_top;

        // Update the state depending on the configuration:
        // - If key is disabled, ensure it is released
        // - If rapid trigger is enabled, update based on recent key positions
        // - If rapid trigger is disabled, update based on actuation point
        if (!config->enabled)
        {
            CloverpadKeyboard.release(config->keycode);
        }
        else if (config->rapid_trigger)
        {
            update_key_state_rt(*config, *state, dist_from_top);
        }
        else
        {
            update_key_state_fixed(*config, *state, dist_from_top);
        }

        if (state->pressed)
        {
            CloverpadKeyboard.press(config->keycode);
        }
        else
        {
            CloverpadKeyboard.release(config->keycode);
        }
    }

    // After all key states have been updated, send the combined report to the host
    CloverpadKeyboard.sendReport();
}

void InputHandler::handle_manual_calibration()
{
    // Read the current ADC values for each key and update averages
    // Clamp the ADC value to avoid overflowing
    this->read_analog_values();

    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        this->he_key_states[i].average_reading.push(clamp(this->adc_buffer[i], MIN_ADC, MAX_ADC));
    }

    // Only continuing processing if the moving averages have initialised
    // Assumes that all of them use the same number of samples
    if (!this->he_key_states[0].average_reading.is_initialised())
    {
        return;
    }

    // Update the min/max ADC values
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        uint16_t avg_adc_value = this->he_key_states[i].average_reading.current_average();
        this->calibration_max_values[i] = std::max(this->calibration_max_values[i], avg_adc_value);
        this->calibration_min_values[i] = std::min(this->calibration_min_values[i], avg_adc_value);
    }
}

void InputHandler::apply_manual_calibration(HEKeyConfiguration he_key_configs[HE_KEY_COUNT])
{
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        // Minimum ADC value = calibration for top position (i.e. further from sensor)
        // Maximum ADC value = calibration for bottom position (i.e. closer to sensor)
        he_key_configs[i].calibration_adc_top = this->calibration_min_values[i];
        he_key_configs[i].calibration_adc_bot = this->calibration_max_values[i];
    }
}
