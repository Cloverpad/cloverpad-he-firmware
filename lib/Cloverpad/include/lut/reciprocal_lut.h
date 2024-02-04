#pragma once

#include <array>
#include <cstdint>

/// @brief Generates a lookup table using a reciprocal curve fit: `a / (x - b) + c`
/// @tparam N The number of ADC elements in this lookup table
/// @param adc_start The starting ADC value
/// @param adc_step The step in ADC value
/// @param a The value of coefficient `a` in the curve fit
/// @param b The value of coefficient `b` in the curve fit
/// @param c The value of coefficient `c` in the curve fit
/// @return The generated lookup table with N elements
template <std::size_t N>
constexpr std::array<double, N> generate_reciprocal_lut(uint16_t adc_start, uint16_t adc_step, double a, double b, double c)
{
    static_assert(N >= 2, "Lookup table must contain at least two elements");
    std::array<double, N> lut = {};

    for (std::size_t i = 0; i < N; i++)
    {
        double adc_val = (double)(adc_start + adc_step * i);
        lut[i] = (a / (adc_val - b)) + c;
    }

    return lut;
};
