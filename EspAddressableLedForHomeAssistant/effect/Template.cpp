#ifndef _TEMPLATE_CPP
#define _TEMPLATE_CPP

#include "EffectProcessor.cpp"

class Template : public EffectProcessor {
 public:
  Template(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {}

  void loop() override {}

  bool isFinished() override {
    return true;
  }
};

#endif