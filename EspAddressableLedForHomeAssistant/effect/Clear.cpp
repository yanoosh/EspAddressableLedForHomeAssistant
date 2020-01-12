#ifndef _CLEAR_CPP
#define _CLEAR_CPP

#include "EffectProcessor.cpp"

class Clear : public EffectProcessor {
 public:
  Clear(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {}
  const char *getName() {
    return "clear";
  }
  void loop() {
    this->strip->clear();
    this->strip->show();
  }
  bool isFinished() {
    return true;
  }
};

#endif