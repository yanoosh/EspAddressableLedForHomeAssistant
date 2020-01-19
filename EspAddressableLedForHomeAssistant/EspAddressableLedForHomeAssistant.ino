/*
  To use this code you will need the following dependancies: 
  
  - Support for the ESP8266 boards. 
        - You can add it to the board manager by going to File -> Preference and pasting http://arduino.esp8266.com/stable/package_esp8266com_index.json into the Additional Board Managers URL field.
        - Next, download the ESP8266 dependancies by going to Tools -> Board -> Board Manager and searching for ESP8266 and installing it.
  
  - You will also need to download the follow libraries by going to Sketch -> Include Libraries -> Manage Libraries
      - Adafruit NeoPixel 
      - PubSubClient
      - ArduinoJSON
*/
// ------------------------------
// ---- all config in auth.h ----
// ------------------------------
#ifdef _DEBUG
#define _DP(message) Serial.print(message);
#define _DPLN(message) Serial.println(message);
#else
#define _DP(message)
#define _DPLN(message)
#endif

#define VERSION F("v3.4 - LedController - https://github.com/DotNetDann - http://dotnetdan.info")

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>  //Not beta version. Tested with v5.3.14
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include "Core.h"

// The maximum mqtt message size, including header, is 128 bytes by default.
// You must update your PubSubClient.h file manually.......
#if MQTT_MAX_PACKET_SIZE < 512  // If the max message size is too small, throw an error at compile time. See PubSubClient.cpp line 359
#error "MQTT_MAX_PACKET_SIZE is too small in libraries/PubSubClient/src/PubSubClient.h at const MQTT_MAX_PACKET_SIZE, increase it from 128 to 512"
#endif

/****************************************FOR JSON***************************************/
const int BUFFER_SIZE = JSON_OBJECT_SIZE(60);

char* birthMessage = "online";
const char* lwtMessage = "offline";

/*********************************** LED Defintions ********************************/
// Real values as requested from the MQTT server

/******************************** OTHER GLOBALS *******************************/
Core* core;
const char* on_cmd = "ON";
const char* off_cmd = "OFF";
bool newStateOn = true;
int transitionTime = 150;  // 1-150
RGBW BLACK = {0, 0, 0, 0};
RGBW WHITE = {255, 255, 255, 255};

void setOn();
void setOff();

#include "auth.h"
#include "wifi.h"

#include "effect.h"
#include "mqtt.h"
#include "ota.h"
#include "web.h"

/********************************** START SETUP*****************************************/
void setup() {
#ifdef _DEBUG
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  delay(2000); // wait for client console
#endif
  pinMode(LED_BUILTIN, OUTPUT);       // Initialize the LED_BUILTIN pin as an output (So it doesnt float as a LED is on this pin)
  digitalWrite(LED_BUILTIN, LOW);     // Turn the status LED on
  pinMode(DATA_PIN_RELAY, OUTPUT);    // Initialize the P-Channel MOSFET for the LED strip
  digitalWrite(DATA_PIN_RELAY, LOW);  // Turn the LED strip on
  //Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledCount, DATA_PIN_LEDS, NEO_GRBW + NEO_KHZ800);
  core = new Core();
  authSetup(core);
  core->setup();

  wifiSetup();
  mqttSetup();
  webSetup();
  otaSetup();

  _DPLN("Ready");
  // OK we are connected
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the status LED off
  updateEffectByName("solid");
}

/********************************** START LED POWER STATE *****************************************/
void setOff() {
  core->getStrip()->clear();
  core->getStrip()->show();
  core->setTurnOn(false);

  if (!digitalRead(DATA_PIN_RELAY)) {
    delay(200);                          // Wait for sequence to complete and stable
    digitalWrite(DATA_PIN_RELAY, HIGH);  // Do NOT write to strip while it has no power. (https://forums.adafruit.com/viewtopic.php?f=47&t=100265)
    _DPLN("LED: OFF");
  }

  // NOTE: Should really set the xxx pin to be an input to ensure that data is not sent and to stop potential current flow.
  //Writing to pin in INPUT/High-impedance mode enables/disables the internal pullup resistors. But the high impedance ensures that any current flow through the pin will be negligible.
}

void setOn() {
  if (digitalRead(DATA_PIN_RELAY)) {
    digitalWrite(DATA_PIN_RELAY, LOW);
    delay(500);  // Wait for Leds to init and capasitor to charge??
    _DPLN("LED: ON");
  }

  core->setTurnOn(true);
}

void loop() {
  uint32_t now = millis();
  static uint32_t lastServices = 0;
  static uint32_t lastStrip = 0;

  
  if (core->isLoopEnabled() && now - lastStrip > transitionTime) {  // Once we have completed the transition, No point to keep going though the process
    lastStrip = now;
    if (core->getEffectProcessor() != NULL) {
      core->getEffectProcessor()->loop();
      if (core->getEffectProcessor()->isFinished()) {
        core->disableLoop();
      }
    }
  }

  if (now - lastServices > 100) { 
    lastServices = now;
    wifiLoop();
    mqttLoop(now);
    otaLoop();
    webLoop();
  }

  delay(50);
}
