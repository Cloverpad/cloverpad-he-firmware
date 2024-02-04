#pragma once

#include <cstdint>

#include <filters/moving_average.h>

/// @brief The highest possible position for a hall effect key in millimetres.
#define HIGHEST_POSITION_MM 0.0

/// @brief The lowest possible position for a hall effect key in millimetres.
#define LOWEST_POSITION_MM 4.0

/// @brief The number of samples in the moving average for each key.
#define MOVING_AVERAGE_SAMPLES 16

/// @brief Holds the current state of a hall effect key.
struct HEKeyState
{
    /// @brief Whether this hall effect key is currently pressed.
    bool pressed = false;

    /// @brief The moving average for this hall effect key's ADC readings.
    MovingAverage average_reading = MovingAverage(MOVING_AVERAGE_SAMPLES);

    /// @brief The last calculated position for this key, which is used for determining the next state of the key.
    double last_position_mm = HIGHEST_POSITION_MM;

    /// @brief The current highest position for this key. Only applicable when rapid trigger is enabled and the key is currently released.
    /// @note If `last_position` is not within the upper deadzone and `highest_position_mm` - `last_position_mm` > `sensitivity_down_mm`,
    ///       the key will transition from released to pressed.
    double highest_position_mm = LOWEST_POSITION_MM;

    /// @brief The current lowest position for this key. Only applicable when rapid trigger is enabled and the key is currently pressed.
    /// @note If `last_position` is not within the lower deadzone and `last_position_mm` - `lowest_position_mm` > `sensitivity_up_mm`,
    ///       the key will transition from pressed to released.
    double lowest_position_mm = HIGHEST_POSITION_MM;
};
