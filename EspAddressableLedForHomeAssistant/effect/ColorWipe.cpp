#ifndef _COLOR_WIPE_RAINBOW_CPP
#define _COLOR_WIPE_RAINBOW_CPP

#include "EffectProcessor.cpp"
#include "Common.cpp"

class ColorWipe : public EffectProcessor {
 public:
  ColorWipe(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("color wipe");
    this->drawColor();
    this->length = this->strip->numPixels();
  }

  void loop() override {
    while (this->sleep > 0) {
      this->sleep--;
      return;
    }

    this->strip->setPixelColor(this->step, this->color.raw);
    this->strip->show();

    this->step++;
    if (this->step > this->length) {
      this->step = 0;
      this->drawColor();
      this->sleep = 0;
    }
  }

 private:
  uint16_t length;
  uint8_t sleep = 0;

  void drawColor() {
    this->color.raw = Common::colorWheel(random(255));
  }
};

#endif