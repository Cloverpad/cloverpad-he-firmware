#pragma once

#include <array>
#include <cstdint>

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
    size_t adc_index_lo = (adc_val - adc_start) / adc_step;

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
}

/// @brief Generates a lookup table using a reciprocal curve fit: `a / (x - b) + c`
/// @tparam N The number of ADC elements in this lookup table
/// @param adc_start The starting ADC value
/// @param adc_step The step in ADC value
/// @param a The value of coefficient `a` in the curve fit
/// @param b The value of coefficient `b` in the curve fit
/// @param c The value of coefficient `c` in the curve fit
/// @return The generated lookup table with N+1 elements
template <std::size_t N>
constexpr std::array<double, N> generate_reciprocal_lut(uint16_t adc_start, uint16_t adc_step, double a, double b, double c)
{
    static_assert(N >= 2, "Lookup table must contain at least two elements");
    std::array<double, N> lut = {};

    for (std::size_t i = 0; i <= N; i++)
    {
        double adc_val = (double)(adc_start + adc_step * i);
        lut[i] = (a / (adc_val - b)) + c;
    }

    return lut;
};
