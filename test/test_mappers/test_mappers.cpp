#include <serial/mappers.hpp>
#include <unity.h>

void setUp(void)
{
    // N/A
}

void tearDown(void)
{
    // N/A
}

void test_copy_main_configuration(void)
{
    KeypadConfiguration source = {.configuration_version = 10,
                                  .name = "Source Configuration"};

    KeypadConfiguration target = {.configuration_version = 5,
                                  .name = "Target Configuration"};

    copy_main_configuration(source, target);

    TEST_ASSERT_EQUAL(source.configuration_version, target.configuration_version);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(source.name, target.name, sizeof(source.name));
}

void test_copy_he_key_configuration_without_keycode(void)
{
    HEKeyConfiguration source = {
        .enabled = true,
        .keycode = 'a',
        .rapid_trigger = false,
        .actuation_point_mm = 1.0,
        .up_sensitivity_mm = 0.5,
        .down_sensitivity_mm = 0.3,
        .upper_deadzone_mm = 0.8,
        .lower_deadzone_mm = 0.9,
    };

    HEKeyConfiguration target = {
        .enabled = false,
        .keycode = 'b',
        .rapid_trigger = true,
        .actuation_point_mm = 1.2,
        .up_sensitivity_mm = 0.6,
        .down_sensitivity_mm = 0.4,
        .upper_deadzone_mm = 0.9,
        .lower_deadzone_mm = 1.0,
    };

    copy_he_key_configuration(source, target, false);

    TEST_ASSERT_EQUAL(source.enabled, target.enabled);
    TEST_ASSERT_EQUAL_CHAR('b', target.keycode);
    TEST_ASSERT_EQUAL(source.rapid_trigger, target.rapid_trigger);
    TEST_ASSERT_EQUAL_DOUBLE(source.actuation_point_mm, target.actuation_point_mm);
    TEST_ASSERT_EQUAL_DOUBLE(source.up_sensitivity_mm, target.up_sensitivity_mm);
    TEST_ASSERT_EQUAL_DOUBLE(source.down_sensitivity_mm, target.down_sensitivity_mm);
    TEST_ASSERT_EQUAL_DOUBLE(source.upper_deadzone_mm, target.upper_deadzone_mm);
    TEST_ASSERT_EQUAL_DOUBLE(source.lower_deadzone_mm, target.lower_deadzone_mm);
}

void test_copy_he_key_configuration_with_keycode(void)
{
    HEKeyConfiguration source = {
        .enabled = true,
        .keycode = 'a',
        .rapid_trigger = false,
        .actuation_point_mm = 1.0,
        .up_sensitivity_mm = 0.5,
        .down_sensitivity_mm = 0.3,
        .upper_deadzone_mm = 0.8,
        .lower_deadzone_mm = 0.9,
    };

    HEKeyConfiguration target = {
        .enabled = false,
        .keycode = 'b',
        .rapid_trigger = true,
        .actuation_point_mm = 1.2,
        .up_sensitivity_mm = 0.6,
        .down_sensitivity_mm = 0.4,
        .upper_deadzone_mm = 0.9,
        .lower_deadzone_mm = 1.0,
    };

    copy_he_key_configuration(source, target, true);

    TEST_ASSERT_EQUAL(source.enabled, target.enabled);
    TEST_ASSERT_EQUAL_CHAR(source.keycode, target.keycode);
    TEST_ASSERT_EQUAL(source.rapid_trigger, target.rapid_trigger);
    TEST_ASSERT_EQUAL_DOUBLE(source.actuation_point_mm, target.actuation_point_mm);
    TEST_ASSERT_EQUAL_DOUBLE(source.up_sensitivity_mm, target.up_sensitivity_mm);
    TEST_ASSERT_EQUAL_DOUBLE(source.down_sensitivity_mm, target.down_sensitivity_mm);
    TEST_ASSERT_EQUAL_DOUBLE(source.upper_deadzone_mm, target.upper_deadzone_mm);
    TEST_ASSERT_EQUAL_DOUBLE(source.lower_deadzone_mm, target.lower_deadzone_mm);
}

void test_map_main_configuration_to_firmware(void)
{
    cloverpad_MainConfiguration source = {.configuration_version = 0,
                                          .name = "Source Configuration"};

    KeypadConfiguration mapped = map_main_configuration(source);

    // NOTE: Protocol's configuration version can't override firmware's configuration version
    TEST_ASSERT_EQUAL(CONFIGURATION_VERSION, mapped.configuration_version);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(source.name, mapped.name, std::min(sizeof(source.name), sizeof(mapped.name)));
}

void test_map_main_configuration_to_protocol(void)
{
    KeypadConfiguration source = {.configuration_version = 10,
                                  .name = "Source Configuration"};

    cloverpad_MainConfiguration mapped = map_main_configuration(source);

    TEST_ASSERT_EQUAL(source.configuration_version, mapped.configuration_version);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(source.name, mapped.name, std::min(sizeof(source.name), sizeof(mapped.name)));
}

void process()
{
    UNITY_BEGIN();

    RUN_TEST(test_copy_main_configuration);
    RUN_TEST(test_copy_he_key_configuration_without_keycode);
    RUN_TEST(test_copy_he_key_configuration_with_keycode);

    RUN_TEST(test_map_main_configuration_to_firmware);
    RUN_TEST(test_map_main_configuration_to_protocol);

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
