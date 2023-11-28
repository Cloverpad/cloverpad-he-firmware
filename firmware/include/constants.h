#pragma once

/// @brief The current firmware version as an unsigned 32-bit integer, in YYYYMMDD format.
#define FIRMWARE_VERSION 20231126

/// @brief The configuration version that is supported by this firmware version.
#define CONFIGURATION_VERSION 1

/// @brief The number of hall effect analog keys on this keypad.
#define HE_KEY_COUNT 3

/// @brief Macro for getting the analog pin for a particular key.
/// @note On the Harukapad rev. 1, the keys are wired in reverse, i.e. K0 = A2, K1 = A1, K2 = A0.
#define HE_KEY_PIN(index) A2 - index
