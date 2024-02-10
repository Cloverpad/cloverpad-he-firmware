#include <serial/mappers.h>
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

    TEST_ASSERT_EQUAL(10, target.configuration_version);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(source.name, target.name, sizeof(source.name));
}

void process()
{
    UNITY_BEGIN();
    RUN_TEST(test_copy_main_configuration);
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
