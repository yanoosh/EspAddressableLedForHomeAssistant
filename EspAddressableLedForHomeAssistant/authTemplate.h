/* Add your keys & rename this file to auth.h */
#include "core/Core.hpp"

#ifndef _AUTH_DETAILS
#define _AUTH_DETAILS

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define MQTT_SERVER ""
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_PORT 1883
const char* MQTT_STATE_TOPIC_PREFIX = "led/";  // e.g. led/<deviceName> and led/<deviceName>/set
#define MQTT_AVAIL_TOPIC "/availability"

#define OTApassword ""  //the password you will need to enter to upload remotely via the ArduinoIDE
#define OTAport 8266

#define DATA_PIN_RELAY 2  // D7 on ESP8266
//#define ENABLE_SUPPORT_WHITE_LED

void authSetup(Core *core) {
  core->setStripPin(13);
  core->mqtt->enableStatusExtended();
  core->mqtt->setStateInterval(60000 * 60);
  if (ESP.getChipId() == 123123) {
    core->setDeviceName("kitchen");
    core->setLength(128);
    core->setMaxBrightness(155);
  }
}

#endif
