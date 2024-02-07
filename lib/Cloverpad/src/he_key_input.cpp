#include <algorithm>

#include <constants.h>
#include <input/he_key_input.h>

bool auto_calibration_values_usable(uint16_t min_adc_value, uint16_t max_adc_value, std::function<double(uint16_t)> dist_from_sensor)
{
    // Check that:
    // - They appear to be initialised (max > min)
    // - Distance between them is >= minimum auto calibration distance
    return max_adc_value > min_adc_value &&
           dist_from_sensor(min_adc_value) - dist_from_sensor(max_adc_value) >= MIN_AUTO_CALIBRATION_DIST_MM;
}

void update_key_state_rt(HEKeyConfiguration &config, HEKeyState &state, double dist_from_top)
{
    // Update the current position
    state.last_position_mm = dist_from_top;

    if (dist_from_top <= config.upper_deadzone_mm)
    {
        // If key is in upper deadzone, release the key
        state.highest_position_mm = std::min(dist_from_top, state.highest_position_mm);
        state.lowest_position_mm = 0.0;
        state.pressed = false;
    }
    else if (dist_from_top >= AIR_GAP_RANGE - config.lower_deadzone_mm)
    {
        // If key is in lower deadzone, press the key
        state.highest_position_mm = AIR_GAP_RANGE;
        state.lowest_position_mm = std::max(dist_from_top, state.lowest_position_mm);
        state.pressed = true;
    }
    else if (state.pressed)
    {
        // If key is currently pressed and current position is more than 'up_sensitivity' above the lowest position, release it
        // Otherwise, just keep track of the lowest position
        if (dist_from_top <= (state.lowest_position_mm - config.up_sensitivity_mm))
        {
            state.highest_position_mm = dist_from_top;
            state.lowest_position_mm = 0.0;
            state.pressed = false;
        }
        else
        {
            state.lowest_position_mm = std::max(dist_from_top, state.lowest_position_mm);
        }
    }
    else
    {
        // If key is currently released and current position is more than 'down_sensitivity' below the highest position, press it
        // Otherwise, just keep track of the highest position
        if (dist_from_top >= (state.highest_position_mm + config.down_sensitivity_mm))
        {
            state.highest_position_mm = AIR_GAP_RANGE;
            state.lowest_position_mm = dist_from_top;
            state.pressed = true;
        }
        else
        {
            state.highest_position_mm = std::min(state.highest_position_mm, dist_from_top);
        }
    }
}

void update_key_state_fixed(HEKeyConfiguration &config, HEKeyState &state, double dist_from_top)
{
    // Just determine if this key is past the actuation point
    state.last_position_mm = dist_from_top;
    state.highest_position_mm = 0.0;
    state.lowest_position_mm = AIR_GAP_RANGE;
    state.pressed = dist_from_top >= config.actuation_point_mm;
}
