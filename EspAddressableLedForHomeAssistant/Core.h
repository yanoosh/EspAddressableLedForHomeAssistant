#ifndef _CORE_H
#define _CORE_H

#include "Adafruit_NeoPixel.h"
#include "diode.h"
#include "effect/EffectProcessor.cpp"
#include "rgbw.h"

class Core {
 public:
  static const uint32_t RED = 0x00330000;
  static const uint32_t GREEN = 0x00003300;
  static const uint32_t BLUE = 0x00000033;
  static const uint32_t DARK_YELLOW = 0x00333300;

  void setup() {
    this->generateNameWhenEmpty();
    this->strip = new Adafruit_NeoPixel(this->length, this->stripPin, NEO_RGB + NEO_KHZ400);
    this->diode = new Diode(this->strip);
    this->strip->begin();
    this->strip->show();

    this->strip->fill(DARK_YELLOW, 0);
    this->strip->setPixelColor(this->strip->numPixels() - 1, GREEN);
    this->strip->show();
  }

  Adafruit_NeoPixel *getStrip() {
    return this->strip;
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

  void setDeviceName(char *deviceName) {
    this->deviceName = deviceName;
  }

  char *getDeviceName() {
    return this->deviceName;
  }

  void setTurnOn(bool turnOn) {
    this->turnOn = turnOn;
    this->syncLoopAndState();
  }

  bool isTurnOn() {
    return this->turnOn;
  }

  void setColor(RGBW color) {
    this->color = color;
    this->syncLoopAndState();
  }

  RGBW getColor() {
    return color;
  }

  void setBrightness(uint8_t brightness) {
    this->brightness = max((uint8_t)0, min(brightness, this->maxBrightness));
    this->strip->setBrightness(this->brightness);
    this->syncLoopAndState();
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

  void setEffect(EffectProcessor *effectProcessor) {
    this->effectProcessor = effectProcessor;
    this->syncLoopAndState();
  }

  const char *getEffectName() {
    if (this->effectProcessor != NULL) {
      return this->effectProcessor->getName();
    } else {
      return "";
    }
  }

  EffectProcessor *getEffectProcessor() {
    return this->effectProcessor;
  }

  void disableLoop() {
    this->loopEnabled = false;
  }

  bool isLoopEnabled() {
    return this->loopEnabled;
  }

 private:
  Adafruit_NeoPixel *strip;
  uint16_t length = 50;
  uint8_t stripPin;
  char *deviceName = "";
  bool turnOn;
  RGBW color = {255, 255, 255, 0};
  uint8_t brightness = 255;
  uint8_t maxBrightness = 255;
  const char *effectName;
  EffectProcessor *effectProcessor;
  uint8_t speed;
  bool loopEnabled = true;
  Diode *diode;

  void syncLoopAndState() {
    this->loopEnabled = this->turnOn;
  }

  void generateNameWhenEmpty() {
    if (strlen(this->deviceName) == 0) {
      this->deviceName = new char[12 + 1];
      sprintf(this->deviceName, "led-%d", ESP.getChipId());
    }
    _DP("device name: ")
    _DPLN(this->deviceName)
  }
};

#endif