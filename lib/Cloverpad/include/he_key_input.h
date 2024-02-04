#pragma once

#include <configuration/he_key_configuration.h>
#include <state.h>

/// @brief Updates the key state assuming that rapid trigger is used.
/// @param config The configuration for this key
/// @param state The current state of this key
/// @param dist_from_top The current distance from the top position
void update_key_state_rt(HEKeyConfiguration &config, HEKeyState &state, double dist_from_top);

/// @brief Updates the key state assuming that a fixed actuation point is used.
/// @param config The configuration for this key
/// @param state The current state of this key
/// @param dist_from_top The current distance from the top position
void update_key_state_fixed(HEKeyConfiguration &config, HEKeyState &state, double dist_from_top);
