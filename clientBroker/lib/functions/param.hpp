/*
Param.h: se configuran los parámetros del dispositivo
*/
#ifndef param_hpp
#define param_hpp

//Tiempo en ms para esperar antes de cambiar el estado del LED
#define DELAY 200
//GPIO que se usa para el LED intermitente
#define LED_ONBOARD 2
#define LED_1 2
#define LED_2 4
#define LED_3 15

//AnalogReads
#define ANALOG_1 34
#define ANALOG_2 35

//PWM
#define PWM_LED 16
#define PWM_FREQ 5000
#define PWM_LED_CHANNEL 0
#define PWM_RESOLUTION 8

//GPIO que se usa para el sensor de humedad de suelo
#define HUMIDITY_SENSOR 34

//DHT Sensor de Humedad y Temperatura
#define DHTPIN 22
#define DHTTYPE DHT11

#endif