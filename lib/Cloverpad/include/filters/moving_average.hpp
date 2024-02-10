#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

/// @brief Implements a moving average for ADC values.
/// @tparam N The number of samples in the moving average
template <std::size_t N>
class MovingAverage
{
private:
    /// @brief The current samples in this moving average.
    std::array<uint16_t, N> samples = {};

    /// @brief The next index to be replaced when a new sample is pushed.
    std::size_t next_index = 0;

    /// @brief Whether `N` samples have been pushed to this moving average.
    bool initialised = false;

    /// @brief The current sum of all samples in the buffer.
    uint32_t sum = 0;

    /// @brief The current average value.
    uint16_t avg = 0;

public:
    /// @brief Creates a new moving average with `N` samples.
    MovingAverage()
    {
        static_assert(N >= 2, "Moving average must contain at least 2 samples");
    }

    /// @brief Whether `N` samples have been pushed to this moving average.
    constexpr bool is_initialised()
    {
        return this->initialised;
    }

    /// @brief Returns the current average value.
    constexpr uint16_t current_average()
    {
        return this->avg;
    }

    /// @brief Pushes a new value into the moving average.
    /// @param value The new value to add to the moving average.
    constexpr void push(uint16_t value)
    {
        // Calculate the new sum of values
        this->sum = this->sum - this->samples[this->next_index] + value;

        // Calculate the new average value
        this->avg = this->sum / N;

        // Update the oldest index in the buffer
        this->samples[this->next_index] = value;
        this->next_index = (this->next_index + 1) % N;

        // Circular buffer is fully initialised if we've reached the beginning
        this->initialised |= this->next_index == 0;
    }
};
