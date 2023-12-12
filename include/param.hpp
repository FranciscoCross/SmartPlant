/*
param.hpp: se configuran los parámetros del dispositivo
*/
#ifndef param_hpp
#define param_hpp

// Version del Firmware
#define FIRMWARE_VERSION 0.61

// Actualizaciones de Firmware con autoupdate
#define UPDATE_JSON_URL_DEV "https://firmware-dev.power-pot.com/update/update.json"
#define UPDATE_JSON_URL_QA "https://firmware-qa.power-pot.com/update/update.json"
#define UPDATE_JSON_URL_PRD "https://firmware.power-pot.com/update/update.json"

#define AP_PASS "12344321"
#define MQTT_SERV "mqtt.power-pot.com"

// Cantidad de veces que se ejecuta el bucle principal para comprobar actualizaciones de firmware

#define N_LOOPS_CHECK_FIRMWARE_UPDATE 100


// Tiempo en ms para esperar antes de cambiar el estado del LED
#define DELAY 200

// GPIOs
#define LED_ONBOARD 2
#define LED_1 23


// AnalogReads
#define ANALOG_1 33
#define ANALOG_2 36

// Topicos MQTT-dev
#define TOPIC_LUZ_DEV "esp32-dev/nivelLuz"
#define TOPIC_HUME_SUELO_DEV "esp32-dev/humedadSuelo"
#define TOPIC_HUME_AIRE_DEV "esp32-dev/humidity"
#define TOPIC_TEMPERATURA_DEV "esp32-dev/temperature"
#define TOPIC_VERSION_DEV "esp32-dev/version"
#define TOPIC_TOGGLE_LED_DEV "esp32-dev/toggleLed"

// Topicos MQTT-dev
#define TOPIC_LUZ_QA "esp32-qa/nivelLuz"
#define TOPIC_HUME_SUELO_QA "esp32-qa/humedadSuelo"
#define TOPIC_HUME_AIRE_QA "esp32-qa/humidity"
#define TOPIC_TEMPERATURA_QA "esp32-qa/temperature"
#define TOPIC_VERSION_QA "esp32-qa/version"
#define TOPIC_TOGGLE_LED_QA "esp32-qa/toggleLed"

// Topicos MQTT-dev
#define TOPIC_LUZ_PRD "esp32/nivelLuz"
#define TOPIC_HUME_SUELO_PRD "esp32/humedadSuelo"
#define TOPIC_HUME_AIRE_PRD "esp32/humidity"
#define TOPIC_TEMPERATURA_PRD "esp32/temperature"
#define TOPIC_VERSION_PRD "esp32/version"
#define TOPIC_TOGGLE_LED_PRD "esp32/toggleLed"

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
#define CONFIG_TIMEOUT 600 //Tiempo en segundos esperando configuracion de WiFiManager
#define WIFI_DEBUG_MODE false
#define MAX_MQTT_ERRORS 30

#endif