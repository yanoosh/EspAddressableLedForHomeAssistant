#ifndef _CORE_H
#define _CORE_H

#include "Adafruit_NeoPixel.h"
#include "effect/EffectProcessor.cpp"
#include "rgbw.h"

class Core {
 public:
  Core(Adafruit_NeoPixel *strip) {
    this->strip = strip;
  }

  void setup() {
    this->strip->begin();
    this->strip->show();
  }
  
  void setTurnOn(bool turnOn) {
    this->turnOn = turnOn;
    this->syncLoopAndState();
  }

  bool isTurnOn() {
    return this->turnOn;
  }

  Adafruit_NeoPixel *getStrip() {
    return this->strip;
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

  void setEffect(const char *name, EffectProcessor *effectProcessor) {
    // todo remove name after refactor all effect to class
    this->effectName = name;
    this->effectProcessor = effectProcessor;
    this->syncLoopAndState();
  }

  const char *getEffectName() {
    return this->effectName;
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
  bool turnOn;
  RGBW color = {255, 255, 255, 0};
  uint8_t brightness = 255;
  uint8_t maxBrightness = 255;
  const char *effectName;
  EffectProcessor *effectProcessor;
  uint8_t speed;
  bool loopEnabled = true;

  void syncLoopAndState() {
    this->loopEnabled = this->turnOn;
  }
};

#endif