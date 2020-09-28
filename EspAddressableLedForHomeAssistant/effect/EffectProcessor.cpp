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

  void setColor(Color color) {
    this->color = color;
  }

  uint16_t getStep() {
    return this->step;
  }

  virtual void loop();

 protected:
  Adafruit_NeoPixel *strip;
  const char *name;
  Color color = {};
  int16_t step = 0;
};

#endif