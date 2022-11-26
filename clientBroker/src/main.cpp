#include "param.hpp"
#include "functions.hpp"

DHT dht(DHTPIN, DHTTYPE);

void setup(void) {
  pinMode(LED_ONBOARD, OUTPUT);
  Serial.begin(115200);

  start_ota_webserver();  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  ledcSetup(PWM_LED_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWM_LED, PWM_LED_CHANNEL);
  dht.begin();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > tiempoMuestras * DELAY * pesoMuestras) // 1000ms de muestreo
  {
    lastMsg = now;
    char humString[8];
    char tempString[8];
    mandarDatos(ANALOG_1, tempArray, N_fil, "esp32/nivelLuz", 0, 4095);
    mandarDatos(ANALOG_2, humeArray, N_fil, "esp32/humedadSuelo", 880, 1540); //2370, 4095);

    humedad = dht.readHumidity();
    dtostrf(humedad, 1, 2, humString);
    client.publish("esp32/humidity", humString); // esp32/humidity

    temperature = dht.readTemperature();
    dtostrf(temperature, 1, 2, tempString);
    client.publish("esp32/temperature", tempString); // esp32/temperature

    digitalWrite(LED_ONBOARD, !digitalRead(LED_ONBOARD));
    // Test fede
    //Serial.println("");
    //Serial.print("Sensor Humedad de Suelo: ");
    //Serial.println(analogRead(ANALOG_2));
  }
}

/*
void loop(void) {
    digitalWrite(LED, !digitalRead(LED));
    //humedad = analogRead(HUMIDITY_SENSOR);
    //Serial.println("");
    //Serial.print("Sensor de humedad: ");
    //Serial.println(humedad);
    delay(DELAY);
}
*/