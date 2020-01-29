# 1 "/tmp/tmpgnghn8pp"
#include <Arduino.h>
# 1 "/media/MySpace/home/yanoosh/Workspace/EspAddressableLedForHomeAssistant/EspAddressableLedForHomeAssistant/EspAddressableLedForHomeAssistant.ino"
# 16 "/media/MySpace/home/yanoosh/Workspace/EspAddressableLedForHomeAssistant/EspAddressableLedForHomeAssistant/EspAddressableLedForHomeAssistant.ino"
#ifdef _DEBUG
#define _DP(message) Serial.print(message);
#define _DPLN(message) Serial.println(message);
#else
#define _DP(message) 
#define _DPLN(message) 
#endif

#define VERSION F("v3.4 - LedController - https://github.com/DotNetDann - http://dotnetdan.info")

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include "core/Core.hpp"



#if MQTT_MAX_PACKET_SIZE < 512
#error "MQTT_MAX_PACKET_SIZE is too small in libraries/PubSubClient/src/PubSubClient.h at const MQTT_MAX_PACKET_SIZE, increase it from 128 to 512"
#endif


const int JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(20);

const char* birthMessage = "online";
const char* lwtMessage = "offline";





Core* core;
const char* on_cmd = "ON";
const char* off_cmd = "OFF";
bool newStateOn = true;
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
void setup();
void loop();
#line 72 "/media/MySpace/home/yanoosh/Workspace/EspAddressableLedForHomeAssistant/EspAddressableLedForHomeAssistant/EspAddressableLedForHomeAssistant.ino"
void setup() {
#ifdef _DEBUG
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  delay(2000);
#endif
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(DATA_PIN_RELAY, OUTPUT);
  digitalWrite(DATA_PIN_RELAY, LOW);

  core = new Core();
  authSetup(core);
  core->setup();

  wifiSetup();
  mqttSetup();
  webSetup();
  otaSetup();

  _DPLN("Ready")

  digitalWrite(LED_BUILTIN, HIGH);
  setupEffects();
  updateEffectByName("solid");
}


void setOff() {
  core->getStrip()->clear();
  core->getStrip()->show();
  core->setTurnOn(false);

  if (!digitalRead(DATA_PIN_RELAY)) {
    delay(200);
    digitalWrite(DATA_PIN_RELAY, HIGH);
    _DPLN("LED: OFF")
  }



}

void setOn() {
  if (digitalRead(DATA_PIN_RELAY)) {
    digitalWrite(DATA_PIN_RELAY, LOW);
    delay(500);
    _DPLN("LED: ON")
  }

  core->setTurnOn(true);
}

void loop() {
  uint32_t now = millis();
  static uint32_t lastServices = 0;
  static uint32_t lastStrip = 0;


  if (core->isLoopEnabled() && now - lastStrip > core->getTransitionInterval()) {
    lastStrip = now;
    if (core->getEffectProcessor() != NULL) {
      core->getEffectProcessor()->loop();
      if (core->getEffectProcessor()->isFinished()) {
        core->disableLoop();
      }
    }
  }

  if (now - lastServices > 113) {
    lastServices = now;
    wifiLoop();
    mqttLoop(now);
    otaLoop();
    webLoop();
  }

  delay(50);
}