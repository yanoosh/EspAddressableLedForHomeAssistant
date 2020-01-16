#ifndef _THEATER_CHASE_CPP
#define _THEATER_CHASE_CPP

#include "../rgbw.h"
#include "EffectProcessor.cpp"

class TheaterChase : public EffectProcessor {
 public:
  TheaterChase(Adafruit_NeoPixel *strip, RGBW color) : EffectProcessor(strip) {
    this->setName("theater chase");
    this->color = Adafruit_NeoPixel::Color(color.red, color.green, color.blue, color.white);
  }

  void loop() override {
    uint16_t length = this->strip->numPixels();
    for (uint16_t i = 0; i < length; i++) {
      if ((i % 2) == this->odd) {
        this->strip->setPixelColor(i, this->color);
      } else {
        this->strip->setPixelColor(i, 0);
      }
    }
    this->strip->show();
    this->odd = !this->odd;
  }

  bool isFinished() override {
    return false;
  }

 private:
  uint32_t color;
  bool odd = 1;
};

#endif