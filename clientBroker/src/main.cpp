#include "param.h"
#include "functions.h"

#include <Arduino.h>
#include <stdlib.h>

int humidity = 0;

void setup(void) {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);

    start_ota_webserver();  
}

void loop(void) {
    digitalWrite(LED, !digitalRead(LED));
    /*humidity = analogRead(HUMIDITY_SENSOR);
    Serial.println("");
    Serial.print("Sensor de humedad: ");
    Serial.println(humidity);*/
    delay(DELAY);
}

