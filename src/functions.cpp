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
const char *mqtt_server = MQTT_SERV;
const char *mqtt_client_id = ESP32_ID1;

//Constantes para actualizar si se lanza una nueva version de Firmware con autoupdate
const float FIRMWARE_VERSION = 0.1;
const char* UPDATE_JSON_URL = "https://power-pot.com/update.json";
const char* SERVER_CERT_PEM = "-----BEGIN CERTIFICATE-----\n" \
"MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n" \
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
"MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n" \
"MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
"Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n" \
"A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n" \
"27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n" \
"Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n" \
"TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n" \
"qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n" \
"szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n" \
"Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n" \
"MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n" \
"wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n" \
"aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n" \
"VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n" \
"AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n" \
"FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n" \
"C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n" \
"QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n" \
"h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n" \
"7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n" \
"ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n" \
"MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n" \
"Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n" \
"6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n" \
"0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n" \
"2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n" \
"bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
"-----END CERTIFICATE-----";

//static HttpsOTAStatus_t otastatus;

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
  Serial.println("Iniciando OTA Webserver...");
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
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_pass))
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

// downloads the JSON file with the latest firmware information
void check_firmware_update(void) {
  Serial.println("Looking for a new firmware...");
  
  HTTPClient http;
  http.begin(UPDATE_JSON_URL);
  
  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    String payload = http.getString();
    http.end();
    
    // parse the json file
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.println("downloaded file is not a valid json, aborting...");
    } else {
      float new_version = doc["version"];
      if (new_version > FIRMWARE_VERSION) {
        Serial.printf("current firmware version (%.1f) is lower than the available one (%.1f), upgrading...\n", FIRMWARE_VERSION, new_version);
        const char* fileName = doc["file"];
        if (fileName != NULL) {
          t_httpUpdate_return ret = ESPhttpUpdate.update(fileName);
          switch(ret) {
            case HTTP_UPDATE_FAILED:
              Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
              break;
            case HTTP_UPDATE_NO_UPDATES:
              Serial.println("HTTP_UPDATE_NO_UPDATES");
              break;
            case HTTP_UPDATE_OK:
              Serial.println("HTTP_UPDATE_OK, restarting");
              ESP.restart();
              break;
          }
        } else {
          Serial.println("unable to read the new file name, aborting...");
        }
      } else {
        Serial.printf("current firmware version (%.1f) is greater or equal to the available one (%.1f), nothing to do...\n", FIRMWARE_VERSION, new_version);
      }
    }
  } else {
    Serial.println("unable to download the json file, aborting...");
  }

  //Clean up
    http.end();
}