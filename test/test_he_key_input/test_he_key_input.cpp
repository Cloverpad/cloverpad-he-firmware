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
    TEST_FAIL();
}

void test_update_key_state_rt_lower_deadzone(void)
{
    TEST_FAIL();
}

void test_update_key_state_rt_pressed_below_lowest_point(void)
{
    TEST_FAIL();
}

void test_update_key_state_rt_pressed_within_up_sensitivity_range(void)
{
    TEST_FAIL();
}

void test_update_key_state_rt_pressed_outside_up_sensitivity_range(void)
{
    TEST_FAIL();
}

void test_update_key_state_rt_released_above_highest_point(void)
{
    TEST_FAIL();
}

void test_update_key_state_rt_released_within_down_sensitivity_range(void)
{
    TEST_FAIL();
}

void test_update_key_state_rt_released_outside_down_sensitivity_range(void)
{
    TEST_FAIL();
}

void test_update_key_state_fixed_above_actuation_point(void)
{
    TEST_FAIL();
}

void test_update_key_state_fixed_below_actuation_point(void)
{
    TEST_FAIL();
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
