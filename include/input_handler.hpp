#pragma once

#include <configuration/he_key_configuration.hpp>
#include <constants.hpp>
#include <lut/reciprocal_lut.hpp>
#include <state/he_key_state.hpp>

/// @brief Represents the different modes that the input handler can be in.
enum InputHandlerMode
{
    /// @brief Don't gather ADC readings or generate HID keyboard inputs
    Disabled,

    /// @brief Gather ADC readings and generate HID keyboard inputs
    NormalInput,

    /// @brief Gather ADC readings for manual calibration only
    ManualCalibration,
};

/// @brief Class for handling inputs from hall effect keys and sending HID commands to the host.
class InputHandler
{
private:
    InputHandlerMode mode = InputHandlerMode::NormalInput;

    uint16_t adc_buffer[HE_KEY_COUNT] = {};
    uint16_t calibration_min_values[HE_KEY_COUNT] = {};
    uint16_t calibration_max_values[HE_KEY_COUNT] = {};

    /// @brief Reads the current analog values and stores them in the private buffer.
    void read_analog_values();

    /// @brief Handles the next set of input readings, updates key states and sends HID commands.
    /// @param he_key_configs The current hall effect key configurations
    /// @param use_auto_calibration Whether to use the automatic calibration values when mapping ADC readings
    void handle_normal_input(HEKeyConfiguration he_key_configs[HE_KEY_COUNT], bool use_auto_calibration);

    /// @brief Handles the next set of input readings and updates the current top/bottom ADC calibration values.
    void handle_manual_calibration();

public:
    /// @brief The distance lookup table used by this input handler.
    ReciprocalDistanceLUT<MIN_ADC, MAX_ADC> dist_lut = ReciprocalDistanceLUT<MIN_ADC, MAX_ADC>(RECIPROCAL_COEFF_A, RECIPROCAL_COEFF_B, RECIPROCAL_COEFF_C);

    /// @brief The current states for each hall effect key.
    HEKeyState he_key_states[HE_KEY_COUNT] = {};

    /// @brief Resets the input handler's state to initial values.
    void reset();

    /// @brief Changes the current input handler mode.
    /// @param new_mode The new input handler mode to use
    /// @note Resets the input handler state if the new mode is different
    void change_mode(InputHandlerMode new_mode);

    /// @brief Handles the next operation for the current input handler mode
    /// @param he_key_configs The current hall effect key configurations
    /// @param use_auto_calibration Whether to use the automatic calibration values when mapping ADC readings
    void handle_next(HEKeyConfiguration he_key_configs[HE_KEY_COUNT], bool use_auto_calibration);

    /// @brief Applies the manual calibration values to the hall effect key configurations
    /// @param he_key_configs The current hall effect key configurations
    void apply_manual_calibration(HEKeyConfiguration he_key_configs[HE_KEY_COUNT]);
};
