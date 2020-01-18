#ifndef _EFFECT_PROCESSOR_CPP
#define _EFFECT_PROCESSOR_CPP

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class EffectProcessor {
 public:
  static const uint8_t COLOR_SINGLE = 0;
  static const uint8_t COLOR_RANDOM_STRIP = 1;
  static const uint8_t COLOR_RANDOM_DIOD = 2;
  
  EffectProcessor(Adafruit_NeoPixel *strip) {
    this->strip = strip;
  }

  const void setName(const char *name) {
    this->name = name;
  }

  const char *getName() {
    return this->name;
  }

  virtual void loop();

  virtual bool isFinished();

 protected:
  Adafruit_NeoPixel *strip;
  const char *name;
};

#endif