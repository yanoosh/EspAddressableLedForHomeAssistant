#ifndef _COLOR_WIPE_RAINBOW_CPP
#define _COLOR_WIPE_RAINBOW_CPP

#include "EffectProcessor.cpp"
#include "Common.cpp"

class ColorWipeRainbow : public EffectProcessor {
 public:
  ColorWipeRainbow(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("color wipe");
    this->drawColor();
    this->length = this->strip->numPixels();
  }

  void loop() override {
    while (this->sleep > 0) {
      this->sleep--;
      return;
    }

    this->strip->setPixelColor(this->step, this->color);
    this->strip->show();

    this->step++;
    if (this->step > this->length) {
      this->step = 0;
      this->drawColor();
      this->sleep = 0;
    }
  }

  bool isFinished() override {
    return false;
  }

 private:
  uint32_t color;
  uint16_t length;
  uint16_t step = 0;
  uint8_t sleep = 0;

  void drawColor() {
    this->color = Common::colorWheel(random(255));
  }
};

#endif