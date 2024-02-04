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

/// @brief Perform linear interpolation on the provided ADC values.
/// @tparam N The number of elements in the lookup table
/// @param lut The lookup table mapping ADC values to a predefined function
/// @param adc_start The starting ADC value
/// @param adc_step The difference between ADC values in the lookup table
/// @param adc_val The input ADC value
/// @return The mapped value after linear interpolation, clamped to the min/max of the lookup table
template <std::size_t N>
constexpr double lerp(std::array<double, N> lut, uint16_t adc_start, uint16_t adc_step, uint16_t adc_val)
{
    static_assert(N >= 2, "Lookup table must contain at least two elements");

    // If the specified value is less than the starting value, clamp to lowest lookup value
    if (adc_val < adc_start)
    {
        return lut[0];
    }

    // Determine the index of the entry that is less or equal to this ADC value
    std::size_t adc_index_lo = (adc_val - adc_start) / adc_step;

    // If the specified value is larger than the ending value, clamp to highest lookup value
    if (adc_index_lo >= N - 1)
    {
        return lut[N - 1];
    }

    // Otherwise, perform linear interpolation on the lower and upper values
    uint16_t adc_lo = adc_start + (adc_step * adc_index_lo);
    double adc_diff = (double)(adc_val - adc_lo);
    double interp_lo = lut[adc_index_lo];
    double interp_hi = lut[adc_index_lo + 1];

    return interp_lo + adc_diff * (interp_hi - interp_lo) / (double)adc_step;
};
