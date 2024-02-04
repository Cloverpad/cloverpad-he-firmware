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

void test_reciprocal_lut(void)
{
    /*
        a = 300
        b = 50
        c = 5

        x       a / (x - b) + c
        100     11
        101     10.8823529411765
        102     10.7692307692308
        103     10.6603773584906
        104     10.5555555555556
        105     10.4545454545455
    */
    auto lut = ReciprocalDistanceLUT<100, 105>(300.0, 50.0, 5.0);
    TEST_ASSERT_EQUAL_DOUBLE(11, lut.get_distance(100));
    TEST_ASSERT_EQUAL_DOUBLE(10.8823529411765, lut.get_distance(101));
    TEST_ASSERT_EQUAL_DOUBLE(10.7692307692308, lut.get_distance(102));
    TEST_ASSERT_EQUAL_DOUBLE(10.6603773584906, lut.get_distance(103));
    TEST_ASSERT_EQUAL_DOUBLE(10.5555555555556, lut.get_distance(104));
    TEST_ASSERT_EQUAL_DOUBLE(10.4545454545455, lut.get_distance(105));
}

void process()
{
    UNITY_BEGIN();
    RUN_TEST(test_reciprocal_lut);
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
