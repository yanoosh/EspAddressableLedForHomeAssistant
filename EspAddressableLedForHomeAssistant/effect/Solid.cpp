#ifndef _SOLID_CPP
#define _SOLID_CPP

#include "../rgbw.h"
#include "EffectProcessor.cpp"

class Solid : public EffectProcessor {
 public:
  Solid(Adafruit_NeoPixel *strip, RGBW color) : EffectProcessor(strip) {
    this->color = strip->Color(color.red, color.green, color.blue);
  }
  const char *getName() {
    return "solid";
  }
  void loop() {
    this->strip->fill(this->color);
    this->strip->show();
  }
  bool isFinished() {
    return true;
  }

 private:
  uint32_t color;
};

#endif