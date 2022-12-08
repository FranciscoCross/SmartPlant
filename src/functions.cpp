#include "functions.hpp"
#include "param.hpp"
#include <AsyncElegantOTA.h>

AsyncWebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

//Se toman las credenciales de las variables de entorno. Ver platformio.ini, sección build_flags
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* mqtt_user = MQTT_USER;
const char* mqtt_pass = MQTT_PASS;

//Dirección IP del BROKER MQTT
const char *mqtt_server = "terrariapancho.ddns.net";

long lastMsg = 0;
int value = 0;
float nivelLuz = 0;
float humedad = 0;
float humedadSuelo = 0;
float temperature = 0;
int tiempoMuestras = 1;
int pesoMuestras = 1;

uint8_t tempArray[20] = {0};
uint8_t N_fil = 5;
uint8_t current_temp = 0; // Temperatura actual
uint8_t prom = 0;         // Promedio
uint8_t humeArray[20] = {0};
uint8_t current_hume = 0; // Humedad actual
uint8_t promhume = 0;     // Promedio

void start_ota_webserver(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
      Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a: ");
  Serial.println(ssid);
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "Bienvenido a ESP32 over-the-air (OTA). Para actualizar el firmware de su ESP32 agregue /update en la direccion del navegador.");
  });
  //Inicia ElegantOTA
  AsyncElegantOTA.begin(&server);    
  server.begin();
  Serial.println("HTTP server listo");
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
    changeState(messageTemp, LED_1);
  }
  //------------------Segundo Output topic esp32/output2------------------
  if (String(topic) == "esp32/output2")
  {
    changeState(messageTemp, LED_2);
  }
  //------------------Tercer Output topic esp32/output3------------------
  if (String(topic) == "esp32/output3")
  {
    changeState(messageTemp, LED_3);
  }
  //------------------Cuarto Output topic esp32/output4------------------
  if (String(topic) == "esp32/output4")
  {
    // Serial.print("Cambio de salida PWM");
    // Serial.println("messageTemp");
    ledcWrite(PWM_LED_CHANNEL, messageTemp.toInt());
  }
  // AGREGAR MAS TOPICOS PARA PODER TENER MAS GPIO O CONFIG
  if (String(topic) == "esp32/output5")
  {
    // Serial.print("Cambio de NUmero del filtro");
    // Serial.println("messageTemp");
    int aux = messageTemp.toInt();
    if (0 < aux && aux < 20)
    {
      N_fil = aux;
    }
    // Serial.println(N_fil);
  }
  // AGREGAR MAS TOPICOS PARA PODER TENER MAS GPIO O CONFIG
  if (String(topic) == "esp32/output6")
  {
    // Serial.print("Cambio de tiempo muestra");
    // Serial.println("messageTemp");
    int aux = messageTemp.toInt();
    if (0 < aux && aux < 120)
    {
      long now = millis();
      lastMsg = now;
      tiempoMuestras = aux;
    }
  }
  // AGREGAR MAS TOPICOS PARA PODER TENER MAS GPIO O CONFIG
  if (String(topic) == "esp32/output7")
  {
    // Serial.print("Cambio de tiempo muestra");
    // Serial.println("messageTemp");
    int aux = messageTemp.toInt();
    if (0 < aux && aux < 60)
    {
      long now = millis();
      lastMsg = now;
      pesoMuestras = aux;
    }
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
    Serial.print("Intentando conexion MQTT... ");
    if (client.connect("ESP32Client-fede", mqtt_user, mqtt_pass))
    {
      Serial.println("Conectado");
      client.subscribe("esp32/output1");
      client.subscribe("esp32/output2");
      client.subscribe("esp32/output3");
      client.subscribe("esp32/output4");
      client.subscribe("esp32/output5");
      client.subscribe("esp32/output6");
      client.subscribe("esp32/output7");
    }
    else
    {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos...");
      delay(5000);
    }
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
void mandarDatos(const int Read, uint8_t *datoArray, uint8_t N_fil, const char *topic, int min, int max)
{
  float dato;
  char datoString[8];
  for (size_t i = 0; i < N_fil; i++)
  {
    delay(100);
    dato = 100 - map(analogRead(Read), min, max, 0, 100);
    pushData(datoArray, dato, N_fil);
  }
  prom = prome(datoArray, N_fil);
  dtostrf(prom, 1, 2, datoString);
  client.publish(topic, datoString);
}