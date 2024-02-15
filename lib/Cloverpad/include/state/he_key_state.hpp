#pragma once

#include <constants.hpp>
#include <filters/moving_average.hpp>

#ifndef MOVING_AVERAGE_SAMPLES_POW
/// @brief The number of moving average samples per key, given as a power of 2 (i.e. a value of 4 means 2^4 = 16 samples).
#define MOVING_AVERAGE_SAMPLES_POW 4
#endif

/// @brief Holds the current state of a hall effect key.
struct HEKeyState
{
    /// @brief Whether this hall effect key is currently pressed.
    bool pressed = false;

    /// @brief The moving average for this hall effect key's ADC readings.
    MovingAverage<MOVING_AVERAGE_SAMPLES_POW> average_reading = MovingAverage<MOVING_AVERAGE_SAMPLES_POW>();

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
