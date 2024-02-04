#pragma once

#include <cstdint>

#include <constants.h>

/// @brief Holds the configuration for a hall effect key.
/// @note This is stored in the EEPROM via the `KeyConfiguration` struct.
struct HEKeyConfiguration
{
    /// @brief Whether this hall effect key is enabled (i.e. keypresses are sent to the host).
    bool enabled = true;

    /// @brief The ASCII keycode assigned to this hall effect key.
    char keycode = '\0';

    /// @brief Whether rapid trigger is enabled on this hall effect key.
    bool rapid_trigger = false;

    /// @brief The calibrated ADC value when the key is not pressed.
    uint16_t calibration_adc_top = RECIPROCAL_ADC_TOP;

    /// @brief The calibrated ADC value when the key is pressed.
    uint16_t calibration_adc_bot = RECIPROCAL_ADC_BOT;

    /// @brief The actuation point to use when rapid trigger is disabled, in millimetres.
    /// @note Must be in the range [0.1, 4.0].
    double actuation_point_mm = 1.2;

    /// @brief The upward sensitivity to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.1, 2.0].
    double up_sensitivity_mm = 0.3;

    /// @brief The downward sensitivity to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.1, 2.0].
    double down_sensitivity_mm = 0.3;

    /// @brief The upper deadzone to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.0, AIR_GAP_RANGE], and `lower_deadzone_mm + upper_deadzone_mm <= AIR_GAP_RANGE`.
    ///       The key is always considered as released when above this threshold.
    double upper_deadzone_mm = 0.4;

    /// @brief The lower deadzone to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.0, AIR_GAP_RANGE], and `lower_deadzone_mm + upper_deadzone_mm <= AIR_GAP_RANGE`.
    ///       The key is always considered as pressed when below this threshold.
    double lower_deadzone_mm = 0.4;
};

/// @brief Verifies if a hall effect key configuration is valid.
/// @param configuration The configuration to validate.
/// @return True if valid, false otherwise.
bool verify_he_key_configuration(HEKeyConfiguration &configuration);
