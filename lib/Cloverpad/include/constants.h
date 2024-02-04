#pragma once

/// @brief The current firmware version as an unsigned 32-bit integer, in YYYYMMDD format.
#define FIRMWARE_VERSION 20231126

/// @brief The configuration version that is supported by this firmware version.
#define CONFIGURATION_VERSION 1

/// @brief The number of hall effect analog keys on this keypad.
#define HE_KEY_COUNT 3

/// @brief Macro for getting the analog pin for a particular key.
/// @note On the Cloverpad HE rev. 1, the keys are wired in reverse, i.e. K0 = A2, K1 = A1, K2 = A0.
#define HE_KEY_PIN(index) A2 - index

/// @brief The ADC value at 3.3V for the DRV5056 when there is no magnetic field.
#define MIN_ADC 745

/// @brief The maximum possible ADC value at 3.3V for the DRV5056.
#define MAX_ADC 3846

#ifdef DRV5056A3

// Use DRV5056A3 values if specified
#pragma message "Using DRV5056A3 reciprocal coefficients"

#define RECIPROCAL_COEFF_A 2.68121498e+03
#define RECIPROCAL_COEFF_B 5.14597664e+02
#define RECIPROCAL_COEFF_C 1.20041036e+00

// DRV5056A3 + KS-20 will saturate around 2.0mm
// To make calculations easier, move the bottom position a bit higher
// Doing this here will make calculations easier later, e.g. deadzones
#define AIR_GAP_TOP_MM 5.9  // Air gap when switch is not pressed
#define AIR_GAP_BOT_MM 2.05 // Air gap when switch is pressed

#define RECIPROCAL_ADC_TOP 1085 // ADC value at 5.9mm air gap
#define RECIPROCAL_ADC_BOT 3670 // ADC value at 2.05mm air gap
#define RECIPROCAL_ADC_RANGE (RECIPROCAL_ADC_TOP - RECIPROCAL_ADC_BOT)

#else

// Otherwise, use DRV5056A4 values
#pragma message "Using DRV5056A4 reciprocal coefficients"

#define RECIPROCAL_COEFF_A 1.34060764e+03
#define RECIPROCAL_COEFF_B 6.29571542e+02
#define RECIPROCAL_COEFF_C 1.20041021e+00

#define AIR_GAP_TOP_MM 5.9 // Air gap when switch is not pressed
#define AIR_GAP_BOT_MM 1.9 // Air gap when switch is pressed

#define RECIPROCAL_ADC_TOP 914  // ADC value at 5.9mm air gap
#define RECIPROCAL_ADC_BOT 2546 // ADC value at 1.9mm air gap
#define RECIPROCAL_ADC_RANGE (RECIPROCAL_ADC_TOP - RECIPROCAL_ADC_BOT)

#endif
