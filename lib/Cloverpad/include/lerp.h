#pragma once

#include <array>
#include <cstdint>

/// @brief Perform linear interpolation on the provided ADC values in software.
/// @param ideal_adc_low The start of the ideal ADC range
/// @param ideal_adc_range The size of the ideal ADC range
/// @param calibrated_adc_low The start of the calibrated ADC range
/// @param calibrated_adc_high The end of the calibrated ADC range
/// @param value The raw value being interpolated
/// @return The linearly interpolated value, mapped into the ideal ADC range
template <typename T>
constexpr T lerp_adc(T ideal_adc_low, T ideal_adc_range, T calibrated_adc_low, T calibrated_adc_high, T value)
{
    // Clamp the value to the calibrated range
    if (value < calibrated_adc_low)
    {
        value = calibrated_adc_low;
    }
    else if (value > calibrated_adc_high)
    {
        value = calibrated_adc_high;
    }

    // Interpolate the ADC value, mapping it to the ideal range based on the calibration
    return ideal_adc_low + ideal_adc_range * (value - calibrated_adc_low) / (calibrated_adc_high - calibrated_adc_low);
};
