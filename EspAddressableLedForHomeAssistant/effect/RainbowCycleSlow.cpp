#ifndef _RAINBOW_CYCLE_CPP
#define _RAINBOW_CYCLE_CPP

#include <Adafruit_NeoPixel.h>
#include "EffectProcessor.cpp"

class RainbowCycle : public EffectProcessor {
 public:
  RainbowCycle(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {}

  const char *getName() override {
    return "rainbow cycle";
  }

  bool isFinished() override {
    return false;
  }

  // todo made by mistake - check ranges
  void loop() override {
    uint8_t c[3];
    uint16_t i, count;
    count = this->strip->numPixels();

    for (i = 0; i < count; i++) {
      this->wheel(((i * 256 / count) + this->step) & 255, c);
      this->strip->setPixelColor(i, *c, *(c + 1), *(c + 2));
    }
    this->strip->show();

    this->step++;
    if (this->step > this->maxStep) {
      this->step = 0;
    }
  }

 private:
  uint16_t step = 0;
  const uint16_t maxStep = 256 * 2;

  void wheel(byte wheelPos, byte *c) {
    if (wheelPos < 85) {
      c[0] = wheelPos * 3;
      c[1] = 255 - wheelPos * 3;
      c[2] = 0;
    } else if (wheelPos < 170) {
      wheelPos -= 85;
      c[0] = 255 - wheelPos * 3;
      c[1] = 0;
      c[2] = wheelPos * 3;
    } else {
      wheelPos -= 170;
      c[0] = 0;
      c[1] = wheelPos * 3;
      c[2] = 255 - wheelPos * 3;
    }
  }
};

#endif