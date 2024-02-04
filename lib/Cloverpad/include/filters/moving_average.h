#pragma once

#include <cstddef>
#include <cstdint>

/// @brief Implements a moving average for ADC values.
class MovingAverage
{
private:
    /// @brief The number of samples.
    uint16_t samples;

    /// @brief The data buffer containing each sample reading.
    uint16_t *buffer;

    /// @brief The index of the next data buffer entry to be replaced.
    std::size_t index = 0;

    /// @brief Whether every entry in this moving average has been set at least once.
    bool initialised = false;

    /// @brief The current sum of all samples in the buffer.
    uint32_t sum = 0;

    /// @brief The current average value.
    uint16_t avg = 0;

public:
    /// @brief Creates a new moving average.
    /// @param samples The number of samples in this moving average.
    MovingAverage(uint16_t samples);

    /// @brief Returns whether every entry in this moving average has been set at least once.
    bool is_initialised();

    /// @brief Returns the current average value.
    uint16_t current_average();

    /// @brief Pushes a new value into the moving average.
    /// @param value The new value to add to the moving average.
    /// @return The new average value.
    uint16_t push(uint16_t value);
};
