//#include "param.h"
//#include "functions.h"

#include <Arduino.h>
#include <stdlib.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#define LED 2
#define DELAY 200
#define HUMIDITY_SENSOR 34

//Se toman las credenciales WiFi de las variables de entorno
//Ver platformio.ini, sección build_flags
/*const */char* ssid = WIFI_SSID;
/*const */char* password = WIFI_PASS;

void start_ota_webserver(void);

int humidity = 0;
AsyncWebServer server(80);

void setup(void) {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
        Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Bienvenido a ESP32 over-the-air (OTA).");
    });

    AsyncElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");

    //start_ota_webserver();  
}

void loop(void) {
    digitalWrite(LED, !digitalRead(LED));
    /*humidity = analogRead(HUMIDITY_SENSOR);
    Serial.println("");
    Serial.print("Sensor de humedad: ");
    Serial.println(humidity);*/
    delay(DELAY);
}

void start_ota_webserver(void)
{
}