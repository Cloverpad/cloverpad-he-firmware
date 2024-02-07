#pragma once

#include <functional>

#include <configuration/he_key_configuration.h>
#include <state.h>

/// @brief Determines if the specified auto-calibration values are usable.
/// @param max_adc_value The current maximum ADC value for auto calibration
/// @param min_adc_value The current minimum ADC value for auto calibration
/// @param dist_from_sensor Function that maps ADC value to distance from sensor
/// @return True if these ADC values are usable, i.e. max > min and distance between them is greater than `MIN_AUTO_CALIBRATION_DIST_MM`
bool auto_calibration_values_usable(
    uint16_t min_adc_value,
    uint16_t max_adc_value,
    std::function<double(uint16_t)> dist_from_sensor);

/// @brief Updates the key state assuming that rapid trigger is used.
/// @param config The configuration for this key
/// @param state The current state of this key
/// @param dist_from_top The current distance from the top position
void update_key_state_rt(HEKeyConfiguration &config, HEKeyState &state, double dist_from_top);

/// @brief Updates the key state assuming that a fixed actuation point is used.
/// @param config The configuration for this key
/// @param state The current state of this key
/// @param dist_from_top The current distance from the top position
void update_key_state_fixed(HEKeyConfiguration &config, HEKeyState &state, double dist_from_top);
