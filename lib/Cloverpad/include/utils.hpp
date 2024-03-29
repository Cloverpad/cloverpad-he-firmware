#pragma once

/// @brief Clamps a value to the specified range.
/// @tparam T The type of value to clamp
/// @param value The value being clamped
/// @param min The minimum allowed value
/// @param max The maximum allowed value
/// @return The value clamped to the min/max values specified
template <typename T>
constexpr inline T clamp(const T &value, const T &min, const T &max)
{
    if (value < min)
    {
        return min;
    }

    if (value > max)
    {
        return max;
    }

    return value;
}

/// @brief Returns whether a value is between two boundary values.
/// @tparam T The type of value being compared.
/// @param value The value to check.
/// @param start The starting boundary value.
/// @param end The ending boundary value.
/// @return Whether this value is between the two boundary values.
template <typename T>
constexpr inline bool is_between(T value, T start, T end)
{
    return start <= value && value <= end;
}

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
    value = clamp(value, calibrated_adc_low, calibrated_adc_high);

    // Interpolate the ADC value, mapping it to the ideal range based on the calibration
    return ideal_adc_low + ideal_adc_range * (value - calibrated_adc_low) / (calibrated_adc_high - calibrated_adc_low);
};
