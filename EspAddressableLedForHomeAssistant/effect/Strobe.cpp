#ifndef _STROBE_CPP
#define _STROBE_CPP

#include "EffectProcessor.cpp"
#include "../rgbw.h"

class Strobe : public EffectProcessor {
 public:
  Strobe(Adafruit_NeoPixel *strip, RGBW color) : EffectProcessor(strip) {
    this->color = Adafruit_NeoPixel::Color(color.red, color.green, color.blue, color.white);
  }

  const char *getName() override {
    return "strobe";
  }

  void loop() override {
    if (this->light == 1) {
      this->strip->fill(this->color);
    } else {
      this->strip->clear();
    }
    this->strip->show();
    this->light = !this->light;
  }

  bool isFinished() override {
    return false;
  }

  private:
    uint32_t color;
    bool light = true;
};

#endif