#include "param.hpp"
#include "functions.hpp"

DHT dht(DHTPIN, DHTTYPE);

//-----------------Arduino-Setup-y-Loop-------------------------//
void setup(void) {
  pinMode(LED_ONBOARD, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  Serial.begin(115200);

  start_ota_webserver();
  //wifiSecureClient.setCACert(SERVER_CERT_PEM);
  pubSubClient.setServer(mqtt_server, MQTT_PORT);
  pubSubClient.setCallback(callback);

  ledcSetup(PWM_LED_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWM_LED, PWM_LED_CHANNEL);
  dht.begin();
}

void loop()
{
  //Bucle para repetir el programa antes de comprobar actualizaciones de firmware
  for(int i=0; i<N_LOOPS_CHECK_FIRMWARE_UPDATE; i++)
  {
    if (!pubSubClient.connected())
    {
      Serial.println("Cliente PubSub desconectado, intentando reconexión...");
      reconnect();
    }
    pubSubClient.loop();

    long now = millis();
    if (now - lastMsg > DELAY * tiempoMuestras * pesoMuestras) //Bucle de tiempo
    {
      lastMsg = now;    
      char humString[8];
      char tempString[8];
      mandarDatos(ANALOG_1, tempArray, N_fil, TOPIC_LUZ, 0, 4095);
      mandarDatos(ANALOG_2, humeArray, N_fil, TOPIC_HUME_SUELO, 2370, 4095);//880, 1540); //2370, 4095);

      humedad = dht.readHumidity();
      dtostrf(humedad, 1, 2, humString);
      pubSubClient.publish(TOPIC_HUME_AIRE, humString); // esp32/humidity

      temperature = dht.readTemperature();
      dtostrf(temperature, 1, 2, tempString);
      pubSubClient.publish(TOPIC_TEMPERATURA, tempString); // esp32/temperature

      digitalWrite(LED_ONBOARD, !digitalRead(LED_ONBOARD));
    }
  }

  //Comprobar actualizaciones de firmware
  check_firmware_update();
}