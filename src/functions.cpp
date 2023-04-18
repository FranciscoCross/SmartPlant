#include "functions.hpp"
#include "param.hpp"
// #include <AsyncElegantOTA.h>

// AsyncWebServer server(80);
WiFiClient wifi;
WiFiClientSecure wifiSecureClient;
PubSubClient pubSubClient(wifi);
WiFiManager wm;

// Se toman las credenciales de las variables de entorno. Ver platformio.ini, sección build_flags
/*
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;
const char *mqtt_user = MQTT_USER;
const char *mqtt_pass = MQTT_PASS;
*/

// Credenciales MQTT, se setean con WiFiManager en la funcion wifi_config()
char mqtt_user[MAX_CREDENTIALS_LEN];
char mqtt_pass[MAX_CREDENTIALS_LEN];

// Dirección IP del BROKER MQTT
const char *mqtt_server = MQTT_SERV;
const char *mqtt_client_id = ESP32_ID1;

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

bool shouldSaveConfig = false;

void wifiConfig(void)
{
  // Forzar configuracion de WiFiManager, normalmente false excepto para testing
  bool forceConfig = false;

  // Formatear SPIFFS para testing
  //SPIFFS.format();

  // Eliminar la configuracion guardada de WiFiManager
  //wm.resetSettings();

  // Debug info
  wm.setDebugOutput(true);
 
  bool spiffsSetup = loadConfigFile();

  // Si no se cargo la configuracion
  if (!spiffsSetup)
  {
    Serial.println(F("Forcing config mode as there is no saved config"));
    forceConfig = true;
  }

  // WiFi en modo Station
  WiFi.mode(WIFI_STA); 

  // Setear callback que indica que se deben guardar las configuraciones obtenidas por WifiManager
  wm.setSaveConfigCallback(saveConfigCallback);
 
  // Setear callback que se ejecuta cuando falla la conexion al WiFi con las credenciales guardadas, y se debe entrar en modo Access Point
  wm.setAPCallback(configModeCallback);

  // Se definen los parametros a configurar en WifiManager
  WiFiManagerParameter text_mqtt_user("text_mqtt_user", "MQTT User", "", MAX_CREDENTIALS_LEN);
  WiFiManagerParameter text_mqtt_pass("text_mqtt_pass", "MQTT Password", "", MAX_CREDENTIALS_LEN);
  wm.addParameter(&text_mqtt_user);
  wm.addParameter(&text_mqtt_pass);

  // Dependiendo si se fuerza la configuracion o se deja en autoconnect
  if (forceConfig)
  {
    if (!wm.startConfigPortal("PowerPotConfigAP", "password"))  //TODO: Cambiar password por env var
    {
      Serial.println("startConfigPortal() fallo y se llego al timeout, reiniciando ESP32...");
      delay(2000);
      //TODO: AGREGAR TIMEOUT , BORRAR CONFIGS Y RESETEAR
      ESP.restart();
      delay(5000);
    }
    else
    {
      Serial.println("WiFiManager startConfigPortal() exitoso (forceConfig = true)");
    }
  }
  else
  {
    if (!wm.autoConnect("PowerPotConfigAP", "password")) //TODO: Cambiar password por env var
    {
      Serial.println("autoConnect() fallo y se llego al timeout, reiniciando ESP32...");
      delay(2000);
      //TODO: AGREGAR TIMEOUT , BORRAR CONFIGS Y RESETEAR
      ESP.restart();
      delay(5000);
    }
    else
    {
      Serial.println("WiFiManager autoConnect() exitoso");
    }
  }

  // Las credenciales MQTT solo deben sobreescribirse si se configuraron, pero si se obtubieron de SPIFFS no deben sobreescribirse
  // Si no se cargo la configuracion significa que se configuro esta vez y debemos guardar
  if(!spiffsSetup)
  {
    strcpy(mqtt_user, text_mqtt_user.getValue());
    strcpy(mqtt_pass, text_mqtt_pass.getValue());
  }

  Serial.println("");
  Serial.print("Conectado a la red: ");
  Serial.println(WiFi.SSID());
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  //---- TODO: Quitar cuando funcione bien ----
  Serial.print("Usuario MQTT: ");
  Serial.println(mqtt_user);
  Serial.print("Contra MQTT: ");
  Serial.println(mqtt_pass);
  //---- TODO: Quitar cuando funcione bien ----

  //Guardar las configuraciones en archivo para cuando se reinicie
  if (shouldSaveConfig)
  {
    saveConfigFile();
  }
}

/*void start_ota_webserver(void)
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Bienvenido a ESP32 over-the-air (OTA). Para actualizar el firmware de su ESP32 agregue /update en la direccion del navegador."); });
  // Inicia ElegantOTA
  AsyncElegantOTA.begin(&server);
  server.begin();
  Serial.println("OTA Webserver server listo");
}*/

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
  while (!pubSubClient.connected())
  {
    Serial.print("Intentando conexion MQTT... ");
    if (pubSubClient.connect(mqtt_client_id, mqtt_user, mqtt_pass))
    {
      Serial.println("Conectado a MQTT");
      pubSubClient.subscribe("esp32/output1");
      pubSubClient.subscribe("esp32/output2");
      pubSubClient.subscribe("esp32/output3");
      pubSubClient.subscribe("esp32/output4");
      pubSubClient.subscribe("esp32/output5");
      pubSubClient.subscribe("esp32/output6");
      pubSubClient.subscribe("esp32/output7");
    }
    else
    {
      Serial.print("Fallo, rc=");
      Serial.print(pubSubClient.state());
      Serial.printf("// mqtt_user: %s // mqtt_pass: %s //", mqtt_user, mqtt_pass);
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
  pubSubClient.publish(topic, datoString);
}

// Descarga un archivo JSON con informacion sobre las actualizaciones, y de ser necesario se descarga la ultima version de firmware
void checkFirmwareUpdate(void)
{
  Serial.println("Buscando actualizaciones de Firmware...");

  HTTPClient http;
  http.begin(wifiSecureClient, UPDATE_JSON_URL);

  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK)
  {
    String payload = http.getString();
    http.end();

    // Parsear el JSON
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    if (error)
    {
      Serial.println("deserializeJson() error code ");
      Serial.println(error.f_str());
    }
    else
    {
      float new_version = doc["version"];
      if (new_version > FIRMWARE_VERSION)
      {
        Serial.printf("La version de firmware actual (%.2f) es anterior a la version disponible online (%.2f), actualizando...\n", FIRMWARE_VERSION, new_version);
        // const char* fileName = doc["file"];
        String file_url = String(doc["base_url"].as<String>()) + String(doc["version"].as<String>()) + String(doc["file_extension"].as<String>());
        Serial.printf("filename: ");
        Serial.println(file_url);
        if (file_url != NULL)
        {
          ESPhttpUpdate.rebootOnUpdate(false); // Reboot manual con ESP.restart()
          t_httpUpdate_return ret = ESPhttpUpdate.update(file_url);
          switch (ret)
          {
          case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;
          case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;
          case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK, restarting...");
            delay(2000);
            ESP.restart();
            break;
          }
        }
        else
        {
          Serial.printf("Error al leer el nombre de archivo del nuevo firmware, cancelando... (version actual: %.2f)", FIRMWARE_VERSION);
        }
      }
      else
      {
        Serial.printf("La version de firmware actual (%.2f) es mayor o igual a la version disponible online (%.2f), no se necesita actualizar...\n", FIRMWARE_VERSION, new_version);
      }
    }
  }
  else
  {
    Serial.printf("Error al descargar el archivo JSON, error: %d (version actual: %.2f)\n", httpResponseCode, FIRMWARE_VERSION);
  }
  http.end();
}

// Guarda la configuracion en formato JSON en el sistema de archivos SPIFFS
void saveConfigFile()
{
  Serial.println(F("Guardando configuracion..."));
  // Se crea el JSON
  StaticJsonDocument<512> json;

  // Se agregan las configuraciones que deben ser guardadas
  json["mqtt_user"] = mqtt_user;
  json["mqtt_pass"] = mqtt_pass;

  // Se abre el archivo de configuracion
  File configFile = SPIFFS.open(JSON_CONFIG_FILE, FILE_WRITE);
  if (!configFile)
  {
    Serial.println("Fallo al abrir el archivo de configuracion en SPIFFS");
  }

  // Serializar JSON para escribirlo en el archivo
  serializeJsonPretty(json, Serial);
  if (serializeJson(json, configFile) == 0)
  {
    Serial.println(F("Error al escribir en el archivo de configuracion en SPIFFS"));
  }
  // Se cierra el archivo
  configFile.close();
}

// Carga la configuracion del archivo JSON guardado en el sistema de archivos SPIFFS
bool loadConfigFile()
{
  Serial.println("Mounting File System...");

  // Dependiendo si es la primera vez que se usa SPIFFS
  if (SPIFFS.begin(false) || SPIFFS.begin(true))
  {
    Serial.println("Mounted file system");
    if (SPIFFS.exists(JSON_CONFIG_FILE))
    {
      // Si el archivo existe se leen sus configuraciones
      Serial.println("Leyendo archivo de configuraciones en SPIFFS...");
      File configFile = SPIFFS.open(JSON_CONFIG_FILE, FILE_READ);
      if (configFile)
      {
        Serial.println("Se abrio el archivo de configuraciones con exito");
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        // Se muestra en el Serial Monitor el JSON
        serializeJsonPretty(json, Serial); //TODO: Decidir si se debe ocultar esta info
        Serial.println("");

        if (!error)
        {
          Serial.println("Parsing JSON");
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_pass, json["mqtt_pass"]);
          return true;
        }
        else
        {
          Serial.println("Error al cargar el JSON");
        }
      }
    }
  }
  else
  {
    Serial.println("Error al montar el File System");
  }
  return false;
}

// Callback que indica que se deben guardar las configuraciones
void saveConfigCallback()
{
  Serial.println("Se guardaran las configuraciones en SPIFFS");
  shouldSaveConfig = true;
}
 
// Callback que se ejecuta cuando se entra en modo configuracion
void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entrando en Modo Configuracion de WifiManager");
 
  Serial.print("Config SSID: ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
 
  Serial.print("Config IP Address: ");
  Serial.println(WiFi.softAPIP());
}