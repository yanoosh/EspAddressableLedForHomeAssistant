#ifndef _CLEAR_CPP
#define _CLEAR_CPP

#include "EffectProcessor.cpp"

class Clear : public EffectProcessor {
 public:
  Clear(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("clear");
  }

  void loop() override {
    this->strip->clear();
    this->strip->show();
  }
};

#endif