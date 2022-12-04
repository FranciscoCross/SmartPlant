#include <unity.h>
#include <Arduino.h>
#include "param.h"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_valid_blink_frequency(void)
{
  //Test if DELAY value (blink frequency) is between 50 and 5000 ms
  TEST_ASSERT_GREATER_OR_EQUAL(50, DELAY);
  TEST_ASSERT_LESS_OR_EQUAL(5000, DELAY);
}

void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();
  RUN_TEST(test_valid_blink_frequency);
}

void loop()
{
  UNITY_END(); // stop unit testing
}
