#include <lut/reciprocal_lut.h>
#include <unity.h>

void setUp(void)
{
    // N/A
}

void tearDown(void)
{
    // N/A
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
    RUN_TEST(test_generate_reciprocal_lut_simple);
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
