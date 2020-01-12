#ifndef _RAINBOW_CYCLE_CPP
#define _RAINBOW_CYCLE_CPP

#include <Adafruit_NeoPixel.h>
#include "EffectProcessor.cpp"

class RainbowCycle : public EffectProcessor {
 public:
  RainbowCycle(Adafruit_NeoPixel *strip) : EffectProcessor(strip){
    // todo led number from settings.
    this->ledCount = 50;
  }

  const char *getName() override {
    return this->name;
  }

  bool isFinished() override {
    return false;
  }

  void loop() override {
    byte c[3];
    unsigned int i;
    if (this->position > 256 * 2) {
      this->position = 0;
    }

    for (i = 0; i <= this->ledCount; i++) {
      this->wheel(((i * 256 / this->ledCount) + this->position) & 255, c);
      this->strip->setPixelColor(i, *c, *(c + 1), *(c + 2));
    }
    this->strip->show();

    this->position++;
  }

 private:
  const char *name = "rainbow cycle";
  unsigned int ledCount;
  unsigned int position = 0;

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