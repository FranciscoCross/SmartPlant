#include "param.hpp"
#include "functions.hpp"

DHT dht(DHTPIN, DHTTYPE);
//-----------------Arduino-Setup-y-Loop-------------------------//
void setup(void)
{
  //resetWifiConfig();
  pinMode(LED_ONBOARD, OUTPUT);
  pinMode(LED_1, OUTPUT);
  Serial.begin(115200);

  wifiConfig();
  initConfig();
  dht.begin();
}


 void loop()
 {
   // Bucle para repetir el programa antes de comprobar actualizaciones de firmware
   for (int i = 0; i < N_LOOPS_CHECK_FIRMWARE_UPDATE; i++)
   {
     if (!pubSubClient.connected())
     {
       Serial.println("Cliente PubSub desconectado, intentando reconexión...");
       reconnect();
     }
     pubSubClient.loop();

     long now = millis();
     if (now - lastMsg > DELAY * tiempoMuestras * pesoMuestras) // Bucle de tiempo
     {
       lastMsg = now;
       char humString[8];
       char tempString[8];
       mandarDatos(ANALOG_1, tempArray, N_fil, TOPIC_LUZ, 0, 4095);
       mandarDatos(ANALOG_2, humeArray, N_fil, TOPIC_HUME_SUELO, 2370, 4095); // 880, 1540); //2370, 4095);

       humedad = dht.readHumidity();
       dtostrf(humedad, 1, 2, humString);
       pubSubClient.publish(TOPIC_HUME_AIRE, humString);
       temperature = dht.readTemperature();
       dtostrf(temperature, 1, 2, tempString);
       pubSubClient.publish(TOPIC_TEMPERATURA, tempString);
       char versionStr[8]; // Arreglo de caracteres para almacenar la versión como string
       snprintf(versionStr, sizeof(versionStr), "%.2f", FIRMWARE_VERSION); // Convertir el valor definido a string con dos decimales
       pubSubClient.publish(TOPIC_VERSION, versionStr); // Publicar la versión como string
       //Alternar el estado del LED para indicar una vuelta del loop
       digitalWrite(LED_ONBOARD, !digitalRead(LED_ONBOARD)); 
     }
   }
   // Comprobar actualizaciones de firmware
   checkFirmwareUpdate();
 }
