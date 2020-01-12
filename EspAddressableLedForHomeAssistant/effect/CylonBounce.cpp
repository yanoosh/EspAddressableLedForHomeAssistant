#ifndef _CYLON_BOUNCE_CPP
#define _CYLON_BOUNCE_CPP

#include "../rgbw.h"
#include "EffectProcessor.cpp"

class CylonBounce : public EffectProcessor {
 public:
  CylonBounce(Adafruit_NeoPixel *strip, RGBW color) : EffectProcessor(strip) {
    this->color = strip->Color(color.red, color.green, color.blue);
    this->colorPal = strip->Color(color.red / 10, color.green / 10, color.blue / 10);
    this->effectSize = strip->numPixels() / 5;
    this->maxStep = strip->numPixels() - this->effectSize - 2;
  }

  const char *getName() override {
    return "cylon bounce";
  }

  void loop() override {
    if (this->sleepStep > 0) {
      this->sleepStep--;
      return;
    }

    this->strip->clear();
    this->strip->setPixelColor(this->step, this->colorPal);
    this->strip->fill(this->color, this->step + 1, this->effectSize);
    this->strip->setPixelColor(this->step + this->effectSize + 1, this->colorPal);
    this->strip->show();

    this->step += this->direction;
    if (this->step > this->maxStep) {
      this->step = this->maxStep;
      this->direction = -1;
      this->sleepStep = 5;
    }
    if (this->step < 0) {
      this->step = 0;
      this->direction = 1;
      this->sleepStep = 5;
    }
  }

  bool isFinished() override {
    return false;
  }

 private:
  uint16_t color;
  uint16_t colorPal;
  uint16_t maxStep;
  uint8_t effectSize;
  uint16_t step = 0;
  uint8_t sleepStep = 0;
  uint8_t direction = 1;
};

#endif