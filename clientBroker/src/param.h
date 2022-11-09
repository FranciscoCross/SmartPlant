#define LED 2
#define DELAY 200
#define HUMIDITY_SENSOR 34

//Se toman las credenciales WiFi de las variables de entorno
//Ver platformio.ini, sección build_flags
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;