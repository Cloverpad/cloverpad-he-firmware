#include <cstdint>

#include <unity.h>

#include <utils.h>

void setUp(void)
{
    // N/A
}

void tearDown(void)
{
    // N/A
}

void test_clamp(void)
{
    TEST_ASSERT_EQUAL(5, clamp(5, 1, 10));
    TEST_ASSERT_EQUAL(7, clamp(5, 7, 10));
    TEST_ASSERT_EQUAL(3, clamp(5, 1, 3));
}

void test_is_between(void)
{
    TEST_ASSERT_TRUE(is_between(1, 1, 10));
    TEST_ASSERT_TRUE(is_between(5, 1, 10));
    TEST_ASSERT_TRUE(is_between(10, 1, 10));

    TEST_ASSERT_FALSE(is_between(0, 1, 10));
    TEST_ASSERT_FALSE(is_between(11, 1, 10));
}

void test_lerp_adc(void)
{
    // Ideal Range:         [1000, 3000]
    // Calibrated Range:    [900, 2800]
    // Raw Value:           1500
    //
    // Calibrated Value:
    // x = 1000 + (3000 - 1000) * (1500 - 900) / (2800 - 900)
    //   = 1000 + 2000 * 6/19
    //   ~= 1631.579
    uint16_t result = lerp_adc(1000, 3000 - 1000, 900, 2800, 1500);
    TEST_ASSERT_EQUAL(1631, result);

    // Ideal Range:         [1000, 3000]
    // Calibrated Range:    [900, 2800]
    // Raw Value:           800 (should clamp to 900)
    //
    // Calibrated Value:
    // x = 1000 + (3000 - 1000) * (900 - 900) / (2800 - 900)
    //   = 1000 + 0 / 1900
    //   = 1000
    result = lerp_adc(1000, 3000 - 1000, 900, 2800, 800);
    TEST_ASSERT_EQUAL(1000, result);

    // Ideal Range:         [1000, 3000]
    // Calibrated Range:    [900, 2800]
    // Raw Value:           3100 (should clamp to 2800)
    //
    // Calibrated Value:
    // x = 1000 + (3000 - 1000) * (2800 - 900) / (2800 - 900)
    //   = 1000 + 1900 / 1900
    //   = 3000
    result = lerp_adc(1000, 3000 - 1000, 900, 2800, 3100);
    TEST_ASSERT_EQUAL(3000, result);
}

void process()
{
    UNITY_BEGIN();
    RUN_TEST(test_clamp);
    RUN_TEST(test_is_between);
    RUN_TEST(test_lerp_adc);
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
