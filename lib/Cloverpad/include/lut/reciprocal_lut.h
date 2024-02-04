#pragma once

#include <array>
#include <cstdint>

#define RECIPROCAL_ADC_START 745    // Quiescent ADC
#define RECIPROCAL_ADC_END 3846     // Max DRV5056 ADC Value

#define RECIPROCAL_COEFF_A_DRV5056A3 2.68121498e+03
#define RECIPROCAL_COEFF_B_DRV5056A3 5.14597664e+02
#define RECIPROCAL_COEFF_C_DRV5056A3 1.20041036e+00

#define RECIPROCAL_COEFF_A_DRV5056A4 1.34060764e+03
#define RECIPROCAL_COEFF_B_DRV5056A4 6.29571542e+02
#define RECIPROCAL_COEFF_C_DRV5056A4 1.20041021e+00

// Use DRV5056A3 coefficients if specified
#ifdef DRV5056A3
#pragma message "Using DRV5056A3 reciprocal coefficients"
#define RECIPROCAL_COEFF_A RECIPROCAL_COEFF_A_DRV5056A3
#define RECIPROCAL_COEFF_B RECIPROCAL_COEFF_B_DRV5056A3
#define RECIPROCAL_COEFF_C RECIPROCAL_COEFF_C_DRV5056A3
#endif

// Use DRV5056A4 coefficients if specified or coefficients are incomplete
#if defined(DRV5056A4) || (!defined(RECIPROCAL_COEFF_A) || !defined(RECIPROCAL_COEFF_B) || !defined(RECIPROCAL_COEFF_C))
#pragma message "Using DRV5056A4 reciprocal coefficients"
#define RECIPROCAL_COEFF_A RECIPROCAL_COEFF_A_DRV5056A4
#define RECIPROCAL_COEFF_B RECIPROCAL_COEFF_B_DRV5056A4
#define RECIPROCAL_COEFF_C RECIPROCAL_COEFF_C_DRV5056A4
#endif

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
