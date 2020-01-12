/* Add your keys & rename this file to auth.h */

#ifndef _AUTH_DETAILS
#define _AUTH_DETAILS

#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"

#define MQTT_SERVER "mqtt server ip"
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_PORT 1883
char* MQTT_STATE_TOPIC_PREFIX = "led/";  // e.g. led/<deviceName> and led/<deviceName>/set
#define MQTT_AVAIL_TOPIC "/availability"

#define OTApassword "ota password"  //the password you will need to enter to upload remotely via the ArduinoIDE
#define OTAport 8266

#define DATA_PIN_LEDS 15   // D8 on ESP8266
#define DATA_PIN_RELAY 13  // D7 on ESP8266
// #define ENABLE_SUPPORT_WHITE_LED
// #define DEBUGE_MODE 1000 // Enable debug mode, value define interval to send status

/******************************** CONFIG SETUP *******************************/
int ledCount = 30;             // Default number of leds if no specific config has been set
char* deviceName = "led";
byte maxBrightness = 255;

void setup_config() {
  _DPLN()
  _DP("ESP8266 Chip id: ")
  _DPLN(ESP.getChipId())

  if (ESP.getChipId() == 5186042) {
    deviceName = "kitchen";
    ledCount = 128;       // 2.14m x 60 per meter
    maxBrightness = 155;  // 60% - Brightness Percentage (This is used to limit the current from the power supply) - 2.4A with 128 leds on RGB White (60ma)
  }

  _DP("Device Name: ");
  _DPLN(deviceName);
}

#endif
