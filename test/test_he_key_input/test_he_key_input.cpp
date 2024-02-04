#include <unity.h>

#include <he_key_input.h>

void setUp(void)
{
    // N/A
}

void tearDown(void)
{
    // N/A
}

void test_update_key_state_rt_upper_deadzone(void)
{
    // Setup a configuration and state where the new position is within the upper deadzone
    HEKeyConfiguration config = {.rapid_trigger = true,
                                 .upper_deadzone_mm = 0.5};

    HEKeyState state = {.highest_position_mm = 0.4};

    // Shift the key slightly above the old highest position
    update_key_state_rt(config, state, 0.3);

    // Last + highest positions should be updated accordingly
    TEST_ASSERT_EQUAL_DOUBLE(0.3, state.last_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(0.3, state.highest_position_mm);

    // Key should be released
    TEST_ASSERT_FALSE(state.pressed);
}

void test_update_key_state_rt_lower_deadzone(void)
{
    // Setup a configuration and state where the new position is within the lower deadzone
    HEKeyConfiguration config = {.rapid_trigger = true,
                                 .lower_deadzone_mm = 0.5};

    HEKeyState state = {.lowest_position_mm = AIR_GAP_RANGE - 0.4};

    // Shift the key slightly below the old loweset position
    update_key_state_rt(config, state, AIR_GAP_RANGE - 0.2);

    // Last + lowest positions should be updated accordingly
    TEST_ASSERT_EQUAL_DOUBLE(AIR_GAP_RANGE - 0.2, state.last_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(AIR_GAP_RANGE - 0.2, state.lowest_position_mm);

    // Key should be pressed
    TEST_ASSERT_TRUE(state.pressed);
}

void test_update_key_state_rt_pressed_below_lowest_point(void)
{
    // Setup a rapid trigger configuration and state where:
    // - Key is currently pressed
    // - Current position is outside of the upper/lower deadzone
    HEKeyConfiguration config = {.rapid_trigger = true,
                                 .up_sensitivity_mm = 0.3,
                                 .upper_deadzone_mm = 0.1,
                                 .lower_deadzone_mm = 0.1};

    HEKeyState state = {.pressed = true,
                        .lowest_position_mm = 2.0};

    // Shift the key below the current lowest position
    update_key_state_rt(config, state, 2.2);

    // Last + lowest positions should be updated accordingly
    TEST_ASSERT_EQUAL_DOUBLE(2.2, state.last_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(2.2, state.lowest_position_mm);

    // Key should be pressed
    TEST_ASSERT_TRUE(state.pressed);
}

void test_update_key_state_rt_pressed_within_up_sensitivity_range(void)
{
    // Setup a rapid trigger configuration and state where:
    // - Key is currently pressed
    // - Current position is outside of the upper/lower deadzone
    HEKeyConfiguration config = {.rapid_trigger = true,
                                 .up_sensitivity_mm = 0.3,
                                 .upper_deadzone_mm = 0.1,
                                 .lower_deadzone_mm = 0.1};

    HEKeyState state = {.pressed = true,
                        .lowest_position_mm = 2.0};

    // Shift the key a bit above the current lowest position, but stay within up sensitivity range
    update_key_state_rt(config, state, 1.9);

    // Only last position should be updated
    TEST_ASSERT_EQUAL_DOUBLE(1.9, state.last_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, state.lowest_position_mm);

    // Key should be pressed
    TEST_ASSERT_TRUE(state.pressed);
}

void test_update_key_state_rt_pressed_outside_up_sensitivity_range(void)
{
    // Setup a rapid trigger configuration and state where:
    // - Key is currently pressed
    // - Current position is outside of the upper/lower deadzone
    HEKeyConfiguration config = {.rapid_trigger = true,
                                 .up_sensitivity_mm = 0.3,
                                 .upper_deadzone_mm = 0.1,
                                 .lower_deadzone_mm = 0.1};

    HEKeyState state = {.pressed = true,
                        .lowest_position_mm = 2.0};

    // Shift the key above the current lowest position, outside the up sensitivity range
    update_key_state_rt(config, state, 1.6);

    // Last + highest positions should match
    // Lowest position should be reset
    TEST_ASSERT_EQUAL_DOUBLE(1.6, state.last_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(1.6, state.highest_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, state.lowest_position_mm);

    // Key should be released
    TEST_ASSERT_FALSE(state.pressed);
}

void test_update_key_state_rt_released_above_highest_point(void)
{
    // Setup a rapid trigger configuration and state where:
    // - Key is currently released
    // - Current position is outside of the upper/lower deadzone
    HEKeyConfiguration config = {.rapid_trigger = true,
                                 .down_sensitivity_mm = 0.3,
                                 .upper_deadzone_mm = 0.1,
                                 .lower_deadzone_mm = 0.1};

    HEKeyState state = {.pressed = false,
                        .highest_position_mm = 2.0};

    // Shift the key above the current highest position
    update_key_state_rt(config, state, 1.8);

    // Last + highest positions should be updated accordingly
    TEST_ASSERT_EQUAL_DOUBLE(1.8, state.last_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(1.8, state.highest_position_mm);

    // Key should be released
    TEST_ASSERT_FALSE(state.pressed);
}

void test_update_key_state_rt_released_within_down_sensitivity_range(void)
{
    // Setup a rapid trigger configuration and state where:
    // - Key is currently released
    // - Current position is outside of the upper/lower deadzone
    HEKeyConfiguration config = {.rapid_trigger = true,
                                 .down_sensitivity_mm = 0.3,
                                 .upper_deadzone_mm = 0.1,
                                 .lower_deadzone_mm = 0.1};

    HEKeyState state = {.pressed = false,
                        .highest_position_mm = 2.0};

    // Shift the key a bit below the current highest position, but stay within down sensitivity range
    update_key_state_rt(config, state, 2.1);

    // Only last position should be updated
    TEST_ASSERT_EQUAL_DOUBLE(2.1, state.last_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(2.0, state.highest_position_mm);

    // Key should be released
    TEST_ASSERT_FALSE(state.pressed);
}

void test_update_key_state_rt_released_outside_down_sensitivity_range(void)
{
    // Setup a rapid trigger configuration and state where:
    // - Key is currently released
    // - Current position is outside of the upper/lower deadzone
    HEKeyConfiguration config = {.rapid_trigger = true,
                                 .down_sensitivity_mm = 0.3,
                                 .upper_deadzone_mm = 0.1,
                                 .lower_deadzone_mm = 0.1};

    HEKeyState state = {.pressed = false,
                        .highest_position_mm = 2.0};

    // Shift the key below the current highest position, outside the down sensitivity range
    update_key_state_rt(config, state, 2.4);

    // Last + lowest positions should match
    // Highest position should be reset
    TEST_ASSERT_EQUAL_DOUBLE(2.4, state.last_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(AIR_GAP_RANGE, state.highest_position_mm);
    TEST_ASSERT_EQUAL_DOUBLE(2.4, state.lowest_position_mm);

    // Key should be pressed
    TEST_ASSERT_TRUE(state.pressed);
}

void test_update_key_state_fixed_above_actuation_point(void)
{
    // Setup a non-rapid trigger configuration and state where:
    // - Key is currently pressed
    HEKeyConfiguration config = {.rapid_trigger = false, .actuation_point_mm = 1.5};
    HEKeyState state = {.pressed = true};

    // Shift the key above the actuation point
    update_key_state_fixed(config, state, 1.2);

    // Last position should be updated accordingly
    TEST_ASSERT_EQUAL_DOUBLE(1.2, state.last_position_mm);

    // Key should be released
    TEST_ASSERT_FALSE(state.pressed);
}

void test_update_key_state_fixed_below_actuation_point(void)
{
    // Setup a non-rapid trigger configuration and state where:
    // - Key is currently released
    HEKeyConfiguration config = {.rapid_trigger = false, .actuation_point_mm = 1.5};
    HEKeyState state = {.pressed = false};

    // Shift the key below the actuation point
    update_key_state_fixed(config, state, 1.8);

    // Last position should be updated accordingly
    TEST_ASSERT_EQUAL_DOUBLE(1.8, state.last_position_mm);

    // Key should be pressed
    TEST_ASSERT_TRUE(state.pressed);
}

void process()
{
    UNITY_BEGIN();

    RUN_TEST(test_update_key_state_rt_upper_deadzone);
    RUN_TEST(test_update_key_state_rt_lower_deadzone);
    RUN_TEST(test_update_key_state_rt_pressed_below_lowest_point);
    RUN_TEST(test_update_key_state_rt_pressed_within_up_sensitivity_range);
    RUN_TEST(test_update_key_state_rt_pressed_outside_up_sensitivity_range);
    RUN_TEST(test_update_key_state_rt_released_above_highest_point);
    RUN_TEST(test_update_key_state_rt_released_within_down_sensitivity_range);
    RUN_TEST(test_update_key_state_rt_released_outside_down_sensitivity_range);

    RUN_TEST(test_update_key_state_fixed_above_actuation_point);
    RUN_TEST(test_update_key_state_fixed_below_actuation_point);

    UNITY_END();
}

#ifdef ARDUINO
#include <Arduino.h>

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Wait for serial connection to establish
    while (!Serial)
    {
        delay(10);
    }

    process();
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
#else
int main(void)
{
    process();
    return 0;
}
#endif
