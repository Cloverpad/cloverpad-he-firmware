#include <lut.h>
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

void test_generate_reciprocal_lut_simple(void)
{
    uint16_t adc_start = 70;
    uint16_t adc_step = 50;
    double a = 300.0;
    double b = 20.0;
    double c = 5.0;

    /*
        x       (x - b)     a / (x - b)     a / (x - b) + c
        70      50.0        6.0             11.0
        120     100.0       3.0             8.0
        170     150.0       2.0             7.0
        220     200.0       1.5             6.5
    */
    std::array<double, 4> lut = generate_reciprocal_lut<4>(adc_start, adc_step, a, b, c);
    TEST_ASSERT_EQUAL_DOUBLE(11.0, lut[0]);
    TEST_ASSERT_EQUAL_DOUBLE(8.0, lut[1]);
    TEST_ASSERT_EQUAL_DOUBLE(7.0, lut[2]);
    TEST_ASSERT_EQUAL_DOUBLE(6.5, lut[3]);
}

void process()
{
    UNITY_BEGIN();
    RUN_TEST(test_lerp_adc);
    RUN_TEST(test_lerp_simple);
    RUN_TEST(test_generate_reciprocal_lut_simple);
    UNITY_END();
}

#ifdef ARDUINO
#include <Arduino.h>

void setup()
{
    // NOTE: Wait for > 2 secs, mainly for boards that don't support software reset via Serial.DTR/RTS
    delay(2000);

    process();
}

void loop()
{
    digitalWrite(25, HIGH);
    delay(500);
    digitalWrite(25, LOW);
    delay(500);
}
#else
int main(int argc, char **argv)
{
    process();
    return 0;
}
#endif
