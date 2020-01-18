#ifndef _CYLON_BOUNCE_CPP
#define _CYLON_BOUNCE_CPP

#include <Arduino.h>
#include "EffectProcessor.cpp"

class CylonBounce : public EffectProcessor {
 public:
  CylonBounce(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("cylon bounce");
    this->effectSize = strip->numPixels() / 5;
    this->maxStep = strip->numPixels() - this->effectSize;
    this->multiplier = PI / (this->effectSize + 1);
  }

  void loop() override {
    if (this->sleepStep > 0) {
      this->sleepStep--;
      return;
    }

    this->strip->clear();
    for (uint8_t i = 0; i < this->effectSize; i++) {
      float sinMultiplier = sin(this->multiplier * (float)(i + 1.0));
      this->strip->setPixelColor(
        this->step + i,
        this->color.r * sinMultiplier,
        this->color.g * sinMultiplier,
        this->color.b * sinMultiplier,
        this->color.w * sinMultiplier
      );
    }
    this->strip->show();

    this->step += this->direction;
    if (this->step > this->maxStep) {
      this->step = this->maxStep;
      this->direction = -1;
      this->sleepStep = 5;
    } else if (this->step < 0) {
      this->step = 0;
      this->direction = 1;
      this->sleepStep = 5;
    }
  }

  bool isFinished() override {
    return false;
  }

 private:
  uint8_t effectSize;
  uint8_t sleepStep = 0;
  int16_t step = 0;
  int16_t maxStep;
  int16_t direction = 1;
  float multiplier;
};

#endif