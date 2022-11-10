#include "param.h"
#include "functions.h"

int humidity = 0;

void setup(void) {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);

    start_ota_webserver();  
}

void loop(void) {
    digitalWrite(LED, !digitalRead(LED));
    /*humidity = analogRead(HUMIDITY_SENSOR);
    Serial.println("");
    Serial.print("Sensor de humedad: ");
    Serial.println(humidity);*/
    delay(DELAY);
}

/*

//-------------SETUP--------------
void setup()
{
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin4, ledChannel);
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
  if (now - lastMsg > tiempoMuestras * 1000 * pesoMuestras) // 1000ms de muestreo
  {
    lastMsg = now;
    char humString[8];
    char tempString[8];
    mandarDatos(analogRead1, tempArray, N_fil, "esp32/nivelLuz", 0, 4095);
    mandarDatos(analogRead2, humeArray, N_fil, "esp32/humedadSuelo", 2370, 4095);

    humedad = dht.readHumidity();
    dtostrf(humedad, 1, 2, humString);
    client.publish("esp32/humidity", humString); // esp32/humidity

    temperature = dht.readTemperature();
    dtostrf(temperature, 1, 2, tempString);
    client.publish("esp32/temperature", tempString); // esp32/temperature
  }
}
*/

