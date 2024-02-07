#pragma once

/// @brief Clamps a value to the specified range.
/// @tparam T The type of value to clamp
/// @param value The value being clamped
/// @param min The minimum allowed value
/// @param max The maximum allowed value
/// @return The value clamped to the min/max values specified
template <typename T>
constexpr inline T clamp(const T &value, const T &min, const T &max)
{
    if (value < min)
    {
        return min;
    }

    if (value > max)
    {
        return max;
    }

    return value;
}

/// @brief Returns whether a value is between two boundary values.
/// @tparam T The type of value being compared.
/// @param value The value to check.
/// @param start The starting boundary value.
/// @param end The ending boundary value.
/// @return Whether this value is between the two boundary values.
template <typename T>
constexpr inline bool is_between(T value, T start, T end)
{
    return start <= value && value <= end;
}
