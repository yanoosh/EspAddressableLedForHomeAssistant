#ifndef _SOLID_CPP
#define _SOLID_CPP

#include "../rgbw.h"
#include "EffectProcessor.cpp"

class Solid : public EffectProcessor {
 public:
 // todo add solid with rainbow effect
  Solid(Adafruit_NeoPixel *strip, RGBW color) : EffectProcessor(strip) {
    this->setName("solid");
    this->color = strip->Color(color.red, color.green, color.blue, color.white);
  }

  void loop() override {
    this->strip->fill(this->color);
    this->strip->show();
  }

  bool isFinished() override {
    return true;
  }

 private:
  uint32_t color;
};

#endif