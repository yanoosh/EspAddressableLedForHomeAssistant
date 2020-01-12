#ifndef _EFFECT_PROCESSOR_CPP
#define _EFFECT_PROCESSOR_CPP

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

class EffectProcessor {
 public:
  EffectProcessor(Adafruit_NeoPixel *strip) {
    this->strip = strip;
  }
  virtual const char *getName();
  virtual void loop();
  virtual bool isFinished();

 protected:
  Adafruit_NeoPixel *strip;
};

#endif