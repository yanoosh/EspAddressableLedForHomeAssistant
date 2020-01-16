#ifndef _TWINKLE_CPP
#define _TWINKLE_CPP

#include "../rgbw.h"
#include "EffectProcessor.cpp"

class Twinkle : public EffectProcessor {
 public:
  Twinkle(Adafruit_NeoPixel *strip, RGBW color) : EffectProcessor(strip) {
    this->color = strip->Color(color.red, color.green, color.blue, color.white);
    this->affectedPixels = this->strip->numPixels() / 5;
  }
  const char *getName() override {
    return "twinkle";
  }
  void loop() override {
    if (this->step == 0) {
      this->strip->clear();
      this->strip->show();
    }

    // todo 100% random, find random number then iterate looking for black
    this->strip->setPixelColor(random(this->strip->numPixels()), color);
    this->strip->show();

    this->step++;
    if (this->step > affectedPixels) {
      this->step = 0;
    }
  }

  bool isFinished() override {
    return false;
  }

 private:
  uint32_t color;
  uint16_t affectedPixels;
  uint16_t step = 0;
};

#endif