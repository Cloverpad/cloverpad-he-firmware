#pragma once

#include <stdint.h>

#include <moving_average.h>

/// @brief The highest possible position for a hall effect key in millimetres.
#define HIGHEST_POSITION_MM 0.0

/// @brief The lowest possible position for a hall effect key in millimetres.
#define LOWEST_POSITION_MM 4.0

/// @brief The number of samples in the moving average for each key.
#define MOVING_AVERAGE_SAMPLES 16

/// @brief Holds the configuration for a hall effect key.
struct HEKeyConfig
{
    /// @brief Whether this hall effect key is enabled (i.e. keypresses are sent to the host).
    bool enabled = false;

    /// @brief The ASCII keycode assigned to this hall effect key.
    char keycode = '\0';

    /// @brief Whether rapid trigger is enabled on this hall effect key.
    bool rapid_trigger = true;

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
    /// @note Must be in the range [0.0, 4.0] and less than `lower_deadzone_mm`.
    ///       The key is always considered as released when above this threshold.
    double upper_deadzone_mm = 0.4;

    /// @brief The lower deadzone to use when rapid trigger is enabled, in millimetres.
    /// @note Must be in the range [0.0, 4.0] and greater than `upper_deadzone_mm`.
    ///       The key is always considered as pressed when below this threshold.
    double lower_deadzone_mm = 3.6;
};

/// @brief Holds the current state of a hall effect key.
struct HEKeyState
{
    /// @brief Whether this hall effect key is currently pressed.
    bool pressed = false;

    /// @brief The moving average for this hall effect key's ADC readings.
    MovingAverage average_reading = MovingAverage(16);

    /// @brief The last calculated position for this key, which is used for determining the next state of the key.
    double last_position_mm = HIGHEST_POSITION_MM;

    /// @brief The current highest position for this key. Only applicable when rapid trigger is enabled and the key is currently released.
    /// @note If `last_position` is not within the upper deadzone and `highest_position_mm` - `last_position_mm` > `HEKeyConfig::sensitivity_down_mm`,
    ///       the key will transition from released to pressed.
    double highest_position_mm = LOWEST_POSITION_MM;

    /// @brief The current lowest position for this key. Only applicable when rapid trigger is enabled and the key is currently pressed.
    /// @note If `last_position` is not within the lower deadzone and `last_position_mm` - `lowest_position_mm` > `HEKeyConfig::sensitivity_up_mm`,
    ///       the key will transition from pressed to released.
    double lowest_position_mm = HIGHEST_POSITION_MM;
};
