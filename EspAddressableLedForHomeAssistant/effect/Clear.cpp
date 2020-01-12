#ifndef _CLEAR_CPP
#define _CLEAR_CPP

#include "EffectProcessor.cpp"

class Clear : public EffectProcessor {
 public:
  Clear(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {}
  const char *getName() override {
    return "clear";
  }
  void loop() override {
    this->strip->clear();
    this->strip->show();
  }
  bool isFinished() override {
    return true;
  }
};

#endif