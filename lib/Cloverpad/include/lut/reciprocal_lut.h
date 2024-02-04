#pragma once

#include <array>
#include <cstdint>

/// @brief Wrapper over a distance lookup table with ADC step 1 and a reciprocal curve fit.
/// @tparam Start The starting ADC value
/// @tparam End The ending ADC value
template <uint16_t Start, uint16_t End>
class ReciprocalDistanceLUT
{
private:
    std::array<double, End - Start + 1> lut;

public:
    /// @brief Initialises a new distance lookup table using a reciprocal curve fit: `a / (x - b) + c`
    /// @param a The value of coefficient `a` in the curve fit
    /// @param b The value of coefficient `b` in the curve fit
    /// @param c The value of coefficient `c` in the curve fit
    ReciprocalDistanceLUT(double a, double b, double c)
    {
        static_assert(Start < End, "Lookup table must contain at least 2 elements");

        // Initialize the LUT
        this->lut = {};

        uint16_t adc_val = Start;
        for (std::size_t i = 0; i < End - Start + 1; i++)
        {
            lut[i] = (a / (adc_val - b)) + c;
            adc_val++;
        }
    }

    double get_distance(uint16_t calibrated_adc_value)
    {
        // Clamp ADC value and retrieve from index
        // No additional interpolation is required as ADC step is 1
        if (calibrated_adc_value < Start)
        {
            calibrated_adc_value = Start;
        }
        else if (calibrated_adc_value > End)
        {
            calibrated_adc_value = End;
        }

        return this->lut[calibrated_adc_value - Start];
    }
};
