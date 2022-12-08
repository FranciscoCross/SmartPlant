/*
Param.h: se configuran los parámetros del dispositivo
*/
#ifndef param_hpp
#define param_hpp

//Tiempo en ms para esperar antes de cambiar el estado del LED
#define DELAY 200

//GPIOs
#define LED_ONBOARD 2
#define LED_1 2
#define LED_2 4
#define LED_3 15

//AnalogReads
#define ANALOG_1 34
#define ANALOG_2 35

//Topicos MQTT
#define TOPIC_LUZ "esp32/nivelLuz"
#define TOPIC_HUME_SUELO "esp32/humedadSuelo"
#define TOPIC_HUME_AIRE "esp32/humidity"
#define TOPIC_TEMPERATURA "esp32/temperature"

//PWM
#define PWM_LED 16
#define PWM_FREQ 5000
#define PWM_LED_CHANNEL 0
#define PWM_RESOLUTION 8

//DHT Sensor de Humedad y Temperatura
#define DHTPIN 22
#define DHTTYPE DHT11

#endif

//testing workflow 2