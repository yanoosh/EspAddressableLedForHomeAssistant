#ifndef _STROBE_CPP
#define _STROBE_CPP

#include "EffectProcessor.cpp"

class Strobe : public EffectProcessor {
 public:
  Strobe(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("strobe");
  }

  void loop() override {
    if (this->step == 1) {
      this->strip->fill(this->color.raw);
    } else {
      this->strip->clear();
    }
    this->strip->show();
    this->step = !this->step;
  }
};

#endif