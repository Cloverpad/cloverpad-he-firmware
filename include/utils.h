#pragma once

/// @brief Returns whether a value is between two boundary values.
/// @tparam T The type of value being compared.
/// @param value The value to check.
/// @param start The starting boundary value.
/// @param end The ending boundary value.
/// @return Whether this value is between the two boundary values.
template<typename T>
constexpr inline bool is_between(T value, T start, T end)
{
    return start <= value && value <= end;
}
