#ifndef _CORE_CPP
#define _CORE_CPP

#include "../effect/EffectProcessor.cpp"
#include "Adafruit_NeoPixel.h"
#include "Diode.cpp"
#include "EEPROM.h"
#include "Effect.cpp"
#include "MqttProperties.cpp"
#include "color.h"

class Core {
 public:
  static const uint32_t RED = 0x00330000;
  static const uint32_t GREEN = 0x00003300;
  static const uint32_t BLUE = 0x00000033;
  static const uint32_t DARK_YELLOW = 0x00333300;
  MqttProperties *mqtt = new MqttProperties();

  void setup() {
    this->generateNameWhenEmpty();
    this->strip = new Adafruit_NeoPixel(this->length, this->stripPin, NEO_RGB + NEO_KHZ400);
    this->effect = new Effect(this->strip);
    this->diode = new Diode(this->strip);
    this->strip->begin();
    this->strip->show();

    this->strip->fill(DARK_YELLOW, 0);
    this->strip->setPixelColor(this->strip->numPixels() - 1, GREEN);
    this->strip->show();
    this->mqtt->generateTopics(this->deviceName);
    this->loadSettings();
    this->loopEnabled = this->turnOn;
  }

  Adafruit_NeoPixel *getStrip() {
    return this->strip;
  }

  Effect *getEffect() {
    return this->effect;
  }

  Diode *getDiode() {
    return this->diode;
  }

  void setLength(uint16_t length) {
    this->length = length;
  }

  uint16_t getLength() {
    return this->length;
  }

  void setStripPin(uint16_t stripPin) {
    this->stripPin = stripPin;
  }

  uint16_t getStripPin() {
    return this->stripPin;
  }

  void setDeviceName(const char *deviceName) {
    this->deviceName = deviceName;
  }

  const char *getDeviceName() {
    return this->deviceName;
  }

  void setTurnOn(bool turnOn) {
    this->turnOn = turnOn;
  }

  bool isTurnOn() {
    return this->turnOn;
  }

  void setBrightness(uint8_t brightness) {
    this->brightness = max((uint8_t)0, min(brightness, this->maxBrightness));
    this->strip->setBrightness(this->brightness);
  }

  uint8_t getBrightness() {
    return this->brightness;
  }

  void setMaxBrightness(uint8_t maxBrightness) {
    this->maxBrightness = maxBrightness;
    this->setBrightness(this->brightness);
  }

  uint8_t getMaxBrightness() {
    return this->brightness;
  }

  void setSpeed(byte speed) {
    this->speed = speed;
  }

  byte getSpeed() {
    return this->speed;
  }

  void disableLoop() {
    this->loopEnabled = false;
  }

  bool isLoopEnabled() {
    return this->loopEnabled;
  }

  void setTransitionInterval(uint8_t transitionInterval) {
    this->transitionInterval = transitionInterval;
  }

  uint8_t getTransitionInterval() {
    return this->transitionInterval;
  }

  void commit() {
    this->loopEnabled = this->turnOn;
    this->saveSettings();
  }

 private:
  Adafruit_NeoPixel *strip;
  Effect *effect;
  Diode *diode;
  uint16_t length = 50;
  uint8_t stripPin;
  const char *deviceName = "";
  bool turnOn = false;
  uint8_t brightness = 255;
  uint8_t maxBrightness = 255;
  EffectProcessor *effectProcessor;
  uint8_t speed;
  bool loopEnabled = false;
  uint8_t transitionInterval = 150;
  bool memorize = true;
  struct Memorized {
    boolean turnOn;
    Color color = {.raw = 0};
    uint8_t brightness;
    uint8_t effect;
    uint8_t speed;
  };

  void generateNameWhenEmpty() {
    if (strlen(this->deviceName) == 0) {
      this->deviceName = new char[12 + 1];
      sprintf((char *)this->deviceName, "led-%d", ESP.getChipId());
    }
  }
  void loadSettings() {
    if (this->memorize) {
      EEPROM.begin(sizeof(Memorized));
      Memorized memorized = {};
      EEPROM.get(0, memorized);
      setTurnOn(memorized.turnOn);
      setBrightness(memorized.brightness);
      getEffect()->setColor(memorized.color);
      getEffect()->setActiveById(memorized.effect);
      setSpeed(memorized.speed);
    }
  }

  void saveSettings() {
    if (this->memorize) {
      Memorized memorized = {};
      memorized.turnOn = isTurnOn();
      memorized.color = getEffect()->getColor();
      memorized.brightness = getBrightness();
      memorized.effect = getEffect()->getActiveId();
      memorized.speed = getSpeed();

      EEPROM.put(0, memorized);
      EEPROM.commit();
    }
  }
};
#endif