#include <lerp.h>
#include <unity.h>

void setUp(void)
{
    // N/A
}

void tearDown(void)
{
    // N/A
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

void test_lerp_simple(void)
{
    // Simple example where we have a standard
    std::array<double, 3> lut = {10.0, 15.0, 20.0};
    uint16_t adc_start = 10;
    uint16_t adc_step = 10;

    // ADC value of 5 -> 10.0 mm (clamped to lowest value)
    double result = lerp(lut, adc_start, adc_step, 5);
    TEST_ASSERT_EQUAL_DOUBLE(10.0, result);

    // ADC value of 10 -> 10.0 mm
    result = lerp(lut, adc_start, adc_step, 10);
    TEST_ASSERT_EQUAL_DOUBLE(10.0, result);

    // ADC value of 15 -> 12.5 mm
    result = lerp(lut, adc_start, adc_step, 15);
    TEST_ASSERT_EQUAL_DOUBLE(12.5, result);

    // ADC value of 25 -> 17.5 mm
    result = lerp(lut, adc_start, adc_step, 25);
    TEST_ASSERT_EQUAL_DOUBLE(17.5, result);

    // ADC value of 30 -> 20.0 mm (clamped to largest value)
    result = lerp(lut, adc_start, adc_step, 30);
    TEST_ASSERT_EQUAL_DOUBLE(20.0, result);

    // ADC value of 35 -> 20.0 mm (clamped to largest value)
    result = lerp(lut, adc_start, adc_step, 35);
    TEST_ASSERT_EQUAL_DOUBLE(20.0, result);
}

void process()
{
    UNITY_BEGIN();
    RUN_TEST(test_lerp_adc);
    RUN_TEST(test_lerp_simple);
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
