#ifndef _THEATER_CHASE_CPP
#define _THEATER_CHASE_CPP

#include "EffectProcessor.cpp"

class TheaterChase : public EffectProcessor {
 public:
  TheaterChase(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("theater chase");
  }

  void loop() override {
    uint16_t length = this->strip->numPixels();
    for (uint16_t i = 0; i < length; i++) {
      if ((i % 2) == this->odd) {
        this->strip->setPixelColor(i, this->color.raw);
      } else {
        this->strip->setPixelColor(i, 0);
      }
    }
    this->strip->show();
    this->odd = !this->odd;
  }

 private:
  bool odd = 1;
};

#endif