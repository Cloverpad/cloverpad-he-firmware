#pragma once

#include <cstdint>

#include <filters/moving_average.h>

/// @brief The number of samples in the moving average for each key.
#ifndef MOVING_AVERAGE_SAMPLES
#define MOVING_AVERAGE_SAMPLES 16
#endif

/// @brief Holds the current state of a hall effect key.
struct HEKeyState
{
    /// @brief Whether this hall effect key is currently pressed.
    bool pressed = false;

    /// @brief The moving average for this hall effect key's ADC readings.
    MovingAverage<MOVING_AVERAGE_SAMPLES> average_reading = MovingAverage<MOVING_AVERAGE_SAMPLES>();

    /// @brief The last calculated position for this key, which is used for determining the next state of the key.
    double last_position_mm = 0.0;

    /// @brief The current highest position for this key. Only applicable when rapid trigger is enabled and the key is currently released.
    /// @note If `last_position` is not within the upper deadzone and `highest_position_mm` - `last_position_mm` > `sensitivity_down_mm`,
    ///       the key will transition from released to pressed.
    double highest_position_mm = AIR_GAP_RANGE;

    /// @brief The current lowest position for this key. Only applicable when rapid trigger is enabled and the key is currently pressed.
    /// @note If `last_position` is not within the lower deadzone and `last_position_mm` - `lowest_position_mm` > `sensitivity_up_mm`,
    ///       the key will transition from pressed to released.
    double lowest_position_mm = 0.0;
};
