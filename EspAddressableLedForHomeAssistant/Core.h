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

 private:
  Adafruit_NeoPixel *strip;
  boolean turnOn;
  RGBW sourceColor = {255, 255, 255, 0};
  RGBW filteredColor;
  uint8_t brightness = 255;
  uint8_t maxBrightness = 255;
  const char *effectName;
  EffectProcessor *effectProcessor;
  uint8_t speed;
};

#endif