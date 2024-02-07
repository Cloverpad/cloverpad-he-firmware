#include <Arduino.h>

#include <constants.h>
#include <he_key_input.h>
#include <input/cloverpad_keyboard.h>
#include <input/input_handler.h>
#include <lerp.h>

void InputHandler::reset()
{
    // Release any keys that are currently pressed
    CloverpadKeyboard.releaseAll();
    CloverpadKeyboard.sendReport();

    // Reset all key states
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
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

void InputHandler::handle_next(HEKeyConfiguration he_key_configs[HE_KEY_COUNT])
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
        this->handle_normal_input(he_key_configs);
        break;
    }
}

void InputHandler::handle_normal_input(HEKeyConfiguration he_key_configs[HE_KEY_COUNT])
{
    // Read the current ADC values for each key
    for (std::size_t i = 0; i < HE_KEY_COUNT; i++)
    {
        uint16_t adc_value = analogRead(HE_KEY_PIN(i));
        this->he_key_states[i].average_reading.push(adc_value);
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

        // Calibrate the ADC value
        uint16_t calibrated_adc_value = lerp_adc(
            (uint16_t)RECIPROCAL_ADC_TOP,
            (uint16_t)RECIPROCAL_ADC_RANGE,
            config->calibration_adc_top,
            config->calibration_adc_bot,
            state->average_reading.current_average());

        // Determine the distance from the top of the switch
        // Clamp this to 0.0mm if it's too far away
        double current_dist = this->dist_lut.get_distance(calibrated_adc_value);
        double dist_from_top = max(AIR_GAP_TOP_MM - current_dist, 0.0);

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
    // TODO: Implement value capturing for manual calibration
}

void InputHandler::apply_manual_calibration(HEKeyConfiguration he_key_configs[HE_KEY_COUNT])
{
    // TODO: Implement applying of manual calibration
}
