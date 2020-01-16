#ifndef _RAINBOW_CYCLE_CPP
#define _RAINBOW_CYCLE_CPP

#include <Adafruit_NeoPixel.h>
#include "EffectProcessor.cpp"

class RainbowCycle : public EffectProcessor {
 public:
  // todo add parametr to define how many color will be shown on one stripe 2 means two full colors on one strip 0.1 just part of rainbow on one strip
  RainbowCycle(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->ledCount = strip->numPixels();
  }

  const char *getName() override {
    return this->name;
  }

  bool isFinished() override {
    return false;
  }

  void loop() override {
    unsigned int i;
    if (this->position > 256 * 2) {
      this->position = 0;
    }

    for (i = 0; i <= this->ledCount; i++) {
      this->strip->setPixelColor(
          i,
          this->wheel(((i * 256 / this->ledCount) + this->position) & 255));
    }
    this->strip->show();

    this->position++;
  }

 private:
  const char *name = "rainbow cycle";
  unsigned int ledCount;
  unsigned int position = 0;

  uint32_t wheel(byte wheelPos) {
    if (wheelPos < 85) {
      return Adafruit_NeoPixel::Color(wheelPos * 3, 255 - wheelPos * 3, 0);
    } else if (wheelPos < 170) {
      wheelPos -= 85;
      return Adafruit_NeoPixel::Color(255 - wheelPos * 3, 0, wheelPos * 3);
    } else {
      wheelPos -= 170;
      return Adafruit_NeoPixel::Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
  }
};

#endif