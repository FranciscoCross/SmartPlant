//Se incluye la libreria del framework de testing Unity
#include <unity.h>

//Se incluyen las mismas funciones usadas en el firmware oficial para un testeo fehaciente
#include "param.hpp"
#include "functions.hpp"
#include "functions.cpp"

void setUp(void)
{
  // Setear lo necesario antes de todos los tests
}

void tearDown(void)
{
  // Limpiar lo necesario despues de todos los tests
}

void test_valid_frequency(void)
{
  Serial.println("Test: Checkeando Frecuencia dentro del rango válido (Entre 50 y 5000 ms)");
  TEST_ASSERT_GREATER_OR_EQUAL(50, DELAY);
  TEST_ASSERT_LESS_OR_EQUAL(5000, DELAY);
}

void test_mqtt_connection(void)
{
  if (!pubSubClient.connected())
  {
    Serial.println("Test: Conexión al servidor MQTT");
    reconnect();
  }
  pubSubClient.loop(); 
  TEST_ASSERT_EQUAL(true, pubSubClient.connected());
}

void setup()
{
  // Esperar al menos 2 segundos para que se inicialicen bien los tests
  delay(2000);

  pinMode(LED_ONBOARD, OUTPUT);
  Serial.begin(115200);
  
  wifiConfig();
  pubSubClient.setServer(MQTT_SERV, MQTT_PORT);
  pubSubClient.setCallback(callback);

  Serial.println("Iniciando Tests...");
  UNITY_BEGIN();
  digitalWrite(LED_ONBOARD, HIGH);
  RUN_TEST(test_valid_frequency);
  RUN_TEST(test_mqtt_connection);
  Serial.println("Finalizando Tests...");
}

void loop()
{
  // Una vez que termina el setup() terminar Unit Testing
  UNITY_END(); 
}
