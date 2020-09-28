#ifndef _RAINBOW_CYCLE_CPP
#define _RAINBOW_CYCLE_CPP

#include <Adafruit_NeoPixel.h>
#include "EffectProcessor.cpp"
#include "Common.cpp"

class RainbowCycle : public EffectProcessor {
 public:
  // todo add parametr to define how many color will be shown on one stripe 2 means two full colors on one strip 0.1 just part of rainbow on one strip
  RainbowCycle(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("rainbow cycle");
    this->ledCount = strip->numPixels();
  }

  void loop() override {
    unsigned int i;
    if (this->position > 256 * 2) {
      this->position = 0;
    }

    for (i = 0; i <= this->ledCount; i++) {
      this->strip->setPixelColor(
          i,
          Common::colorWheel(((i * 256 / this->ledCount) + this->position) & 255));
    }
    this->strip->show();

    this->position++;
  }

 private:
  unsigned int ledCount;
  unsigned int position = 0;
};

#endif