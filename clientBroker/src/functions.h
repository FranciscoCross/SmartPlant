/*
Functions.h: se configuran las funciones y dependencias
*/
#ifndef functions_h
#define functions_h

#include <Arduino.h>
#include <stdlib.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <DHT.h>

void start_ota_webserver(void);
void callback(char *topic, byte *message, unsigned int length);
void reconnect();
void changeState(String messageTemp, int pin);
double prome(uint8_t temp[], uint8_t N_filter);
void pushData(uint8_t *tempArray, uint8_t newTemp, uint8_t N_fil);
void mandarDatos(const int Read, uint8_t *datoArray, uint8_t N_fil, const char *topic, int min, int max);

#endif

