#ifndef _EFFECT_PROCESSOR_CPP
#define _EFFECT_PROCESSOR_CPP

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "../core/color.h"

class EffectProcessor {
 public:
  static const uint8_t COLOR_SINGLE = 0;
  static const uint8_t COLOR_RANDOM_STRIP = 1;
  static const uint8_t COLOR_RANDOM_DIOD = 2;
  static const uint8_t COLOR_RAINBOW_STRIP = 3;
  static const uint8_t COLOR_RAINBOW_DIOD = 4;

  EffectProcessor(Adafruit_NeoPixel *strip) {
    this->strip = strip;
  }

  void setName(const char *name) {
    this->name = name;
  }

  const char *getName() {
    return this->name;
  }

  void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    this->color.white = white;
    this->color.red = red;
    this->color.green = green;
    this->color.blue = blue;
  }

  void setColor(Color color) {
    this->color = color;
  }

  void setRawColor(uint32_t color) {
    this->color.raw = color;
  }

  virtual void loop();

  virtual bool isFinished();

 protected:
  Adafruit_NeoPixel *strip;
  const char *name;
  Color color = {};
};

#endif