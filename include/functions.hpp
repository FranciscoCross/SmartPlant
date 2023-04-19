/*
Functions.h: se configuran las funciones y dependencias
*/
#ifndef functions_hpp
#define functions_hpp

#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>
#include <WiFi.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>

#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Update.h>
#include <ESP32httpUpdate.h>
#include <WiFiManager.h>
#include <FS.h>
#include <SPIFFS.h>

extern DHT dht;
//extern AsyncWebServer server;
extern WiFiClientSecure wifiSecureClient;
extern PubSubClient pubSubClient;
extern WiFiManager wm;

extern const char *mqtt_server;

extern long lastMsg;
extern char msg[50];
extern int value;

extern float nivelLuz;
extern float humedad;
extern float humedadSuelo;
extern float temperature;
extern int tiempoMuestras;
extern int pesoMuestras;

extern uint8_t tempArray[20];
extern uint8_t N_fil;
extern uint8_t current_temp; // Temperatura actual s
extern uint8_t prom;         // Promedio
extern uint8_t humeArray[20];
extern uint8_t current_hume; // Temperatura actual s
extern uint8_t promhume;     // Promedio

extern bool shouldSaveConfig;

void wifiConfig(void);
//void start_ota_webserver(void);
void callback(char *topic, byte *message, unsigned int length);
void reconnect();
void changeState(String messageTemp, int pin);
double prome(uint8_t temp[], uint8_t N_filter);
void pushData(uint8_t *tempArray, uint8_t newTemp, uint8_t N_fil);
void mandarDatos(const int Read, uint8_t *datoArray, uint8_t N_fil, const char *topic, int min, int max);
void checkFirmwareUpdate(void);
void saveConfigFile(void);
bool loadConfigFile(void);
void saveConfigCallback(void);
void configModeCallback(WiFiManager *myWiFiManager);
void resetWifiConfig(void);
#endif