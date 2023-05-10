/*
param.hpp: se configuran los parámetros del dispositivo
*/
#ifndef param_hpp
#define param_hpp

// Version del Firmware
#define FIRMWARE_VERSION 0.53

// Actualizaciones de Firmware con autoupdate
#define UPDATE_JSON_URL "https://firmware.power-pot.com/update/update.json"

// Cantidad de veces que se ejecuta el bucle principal para comprobar actualizaciones de firmware
#define N_LOOPS_CHECK_FIRMWARE_UPDATE 120

// Tiempo en ms para esperar antes de cambiar el estado del LED
#define DELAY 200

// GPIOs
#define LED_ONBOARD 2
#define LED_1 2
#define LED_2 4
#define LED_3 15

// AnalogReads
#define ANALOG_1 34
#define ANALOG_2 35

// Topicos MQTT
#define TOPIC_LUZ "esp32/nivelLuz"
#define TOPIC_HUME_SUELO "esp32/humedadSuelo"
#define TOPIC_HUME_AIRE "esp32/humidity"
#define TOPIC_TEMPERATURA "esp32/temperature"

// Puerto del servidor MQTT
#define MQTT_PORT 1883

// PWM
#define PWM_LED 16
#define PWM_FREQ 5000
#define PWM_LED_CHANNEL 0
#define PWM_RESOLUTION 8

// DHT Sensor de Humedad y Temperatura
#define DHTPIN 22
#define DHTTYPE DHT11

// Guardar configuracion en memoria no volátil
#define JSON_CONFIG_FILE "/config.json"
#define MAX_CREDENTIALS_LEN 50
#define CONFIG_TIMEOUT 120 //Tiempo en segundos esperando configuracion de WiFiManager
#define WIFI_DEBUG_MODE false
#define MAX_MQTT_ERRORS 30

#endif