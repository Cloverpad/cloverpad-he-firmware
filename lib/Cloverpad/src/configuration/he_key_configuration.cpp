#include <configuration/he_key_configuration.h>
#include <utils.h>

bool verify_he_key_configuration(HEKeyConfiguration &configuration)
{
    // Verify the following details:
    // - Actuation point in range [0.1, 4.0]
    // - Up sensitivity in range [0.1, 2.0]
    // - Down sensitivity in range [0.1, 2.0]
    // - Upper deadzone in range [0.1, 4.0]
    // - Lower deadzone in range [0.1, 4.0]
    // - Upper deadzone <= Lower deadzone
    return is_between(configuration.actuation_point_mm, 0.1, 4.0) &&
           is_between(configuration.up_sensitivity_mm, 0.1, 2.0) &&
           is_between(configuration.down_sensitivity_mm, 0.1, 2.0) &&
           is_between(configuration.upper_deadzone_mm, 0.1, 4.0) &&
           is_between(configuration.lower_deadzone_mm, 0.1, 4.0) &&
           configuration.upper_deadzone_mm <= configuration.lower_deadzone_mm;
}
