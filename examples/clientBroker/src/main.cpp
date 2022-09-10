#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>

//--------Funciones-------
void setup_wifi();
void callback(char *topic, byte *message, unsigned int length);
void reconnect();
void changeState(String messageTemp, int pin);
double prome(uint8_t temp[], uint8_t N_filter);
void pushData(uint8_t *tempArray, uint8_t newTemp, uint8_t N_fil);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

uint8_t tempArray[20] = {0};
uint8_t N_fil = 10;
uint8_t current_temp = 0; // Temperatura actual s
uint8_t prom = 0;         // Promedio

float nivelLuz = 0;
float humedad = 0;
float temperature = 0;
const int ledPin1 = 2;
const int ledPin2 = 4;
const int ledPin3 = 15;
//-------------PWM-----------------
const int ledPin4 = 16;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
//---------------------------------

//------------DHT11----------------
#define DHTPIN 22
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//*********SSID and PASS***********
const char *ssid = "Fibertel WiFi833 2.4GHz";
const char *password = "01416015234";

//------------IP BROKER MQTT----------
const char *mqtt_server = "192.168.0.240";

const int analogRead1 = 34;

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

void setup_wifi()
{
  delay(10);
  Serial.printf("\nContectando a %c\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Mensaje Recibido en topic: ");
  Serial.print(topic);
  Serial.print(", Mensaje: ");
  String messageTemp;
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  //------------------Primer Output topic esp32/output1------------------
  if (String(topic) == "esp32/output1")
  {
    changeState(messageTemp, ledPin1);
  }
  //------------------Segundo Output topic esp32/output2------------------
  if (String(topic) == "esp32/output2")
  {
    changeState(messageTemp, ledPin2);
  }
  //------------------Tercer Output topic esp32/output3------------------
  if (String(topic) == "esp32/output3")
  {
    changeState(messageTemp, ledPin3);
  }
  //------------------Cuarto Output topic esp32/output4------------------
  if (String(topic) == "esp32/output4")
  {
    Serial.print("Cambio de salida PWM");
    Serial.println("messageTemp");
    ledcWrite(ledChannel, messageTemp.toInt());
  }
  // AGREGAR MAS TOPICOS PARA PODER TENER MAS GPIO O CONFIG
  if (String(topic) == "esp32/output5")
  {
    Serial.print("Cambio de NUmero del filtro");
    Serial.println("messageTemp");
    N_fil = messageTemp.toInt();
  }
}

void changeState(String messageTemp, int pin)
{
  Serial.print("Cambio de salida: ");
  if (messageTemp == "on")
  {
    Serial.println("on");
    digitalWrite(pin, HIGH);
  }
  else if (messageTemp == "off")
  {
    Serial.println("off");
    digitalWrite(pin, LOW);
  }
}

void reconnect()
{
  // Bucle hasta que se reconecte
  while (!client.connected())
  {
    Serial.print("Intentando conexion MQTT...");
    if (client.connect("ESP32Client", "mqtt", "fdlc11"))
    {
      Serial.println("Conectado");
      client.subscribe("esp32/output1");
      client.subscribe("esp32/output2");
      client.subscribe("esp32/output3");
      client.subscribe("esp32/output4");
      client.subscribe("esp32/output5");
    }
    else
    {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println("Intentando de nuevo en 5s");
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) // 1000ms de muestreo
  {
    lastMsg = now;
    //------Senial Analogica para enviar al broquer
    nivelLuz = 100 - analogRead(analogRead1) * 100 / 4095;
    pushData(tempArray, nivelLuz, N_fil);
    prom = prome(tempArray, N_fil);
    //  Convertir el valor a char array
    // Serial.printf("Luz Ambiente: %2.f%\n", prom);
    char luzString[8];
    dtostrf(prom, 1, 2, luzString);
    // Serial.print("Temperatura2: ");
    // Serial.println(luzString);
    client.publish("esp32/nivelLuz", luzString); // esp32/nivelLuz

    //------------Senial 2 que queremos enviar al broker-------------
    //---------------------------------------------------------------
    humedad = dht.readHumidity();
    ;

    // Convertir el valor a char array
    char humString[8];
    dtostrf(humedad, 1, 2, humString);
    Serial.printf("Luz Ambiente: %2.f%\n", humedad);
    client.publish("esp32/humidity", humString); // esp32/humidity
    //------------Senial 2 que queremos enviar al broker-------------
    //---------------------------------------------------------------
    temperature = dht.readTemperature();
    ;

    // Convertir el valor a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    // Serial.print("Humedad: ");
    // Serial.println(humString);
    client.publish("esp32/temperature", tempString); // esp32/temperature
  }
}

double prome(uint8_t temp[], uint8_t N_filter)
{
  uint16_t acum = 0;

  for (uint8_t i = 0; i < N_filter; i++)
  {
    acum += temp[i];
  }
  return acum / N_filter;
}

/**
 * @brief Genero un corrimiento de valores en el arreglo y luego almaceno el nuevo valor
 *
 * @param tempArray
 * @param newTemp
 * @param N_fil
 */
void pushData(uint8_t *tempArray, uint8_t newTemp, uint8_t N_fil)
{
  // Corro el valor anterior en el arreglo per copio todo uno a la derecha, luego pongo el valor recibido en este en CERO como en mas actual
  for (int i = (N_fil - 1); i > 0; i--)
  {
    tempArray[i] = tempArray[i - 1];
  }
  tempArray[0] = newTemp;
}