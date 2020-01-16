#ifndef _COLOR_WIPE_RAINBOW_CPP
#define _COLOR_WIPE_RAINBOW_CPP

#include "EffectProcessor.cpp"

class ColorWipeRainbow : public EffectProcessor {
 public:
  ColorWipeRainbow(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->drawColor();
    this->length = this->strip->numPixels();
  }

  const char *getName() override {
    return "color wipe";
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
    this->color = this->wheel(random(255));
  }

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