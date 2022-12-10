#include <unity.h>

#include "param.hpp"
#include "functions.hpp"
#include "functions.cpp"

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_valid_frequency(void)
{
  Serial.println("Test: Checkeando Frecuencia dentro del rango válido");
  //Test if DELAY value (blink frequency) is between 50 and 5000 ms
  TEST_ASSERT_GREATER_OR_EQUAL(50, DELAY);
  TEST_ASSERT_LESS_OR_EQUAL(5000, DELAY);
}

void test_mqtt_connection(void)
{
  if (!client.connected())
  {
    Serial.println("Test: Conexión al servidor MQTT");
    reconnect();
  }
  client.loop(); 
  TEST_ASSERT_EQUAL(true, client.connected());
}

void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  Serial.begin(115200);
  
  start_ota_webserver();
  client.setServer(mqtt_server, MQTT_PORT);
  client.setCallback(callback);

  Serial.println("Iniciando Tests...");
  UNITY_BEGIN();
  RUN_TEST(test_valid_frequency);
  RUN_TEST(test_mqtt_connection);
  Serial.println("Finalizando Tests...");
}

void loop()
{
  UNITY_END(); // stop unit testing
}
