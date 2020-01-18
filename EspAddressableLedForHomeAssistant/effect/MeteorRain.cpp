#ifndef _METEOR_RAIN_CPP
#define _METEOR_RAIN_CPP

#include "EffectProcessor.cpp"

class MeteorRain : public EffectProcessor {
 public:
  MeteorRain(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("meteor rain");
    this->meteorSize = strip->numPixels() / 5;
    this->meteorTrailDecay = strip->getBrightness() / 4;
    this->maxStep = 2 * strip->numPixels();
  }

  void loop() override {
    // fade brightness all LEDs one step
    uint8_t count = this->strip->numPixels(), i;

    for (i = 0; i < count; i++) {
      if ((!this->meteorRandomDecay) || (random(10) > 5)) {
        this->meteorRainFadeToBlack(i, this->meteorTrailDecay);
      }
    }

    // draw meteor
    for (i = 0; i < this->meteorSize; i++) {
      if ((this->step - i < count) && (this->step - i >= 0)) {
        this->strip->setPixelColor(this->step - i, this->color);
      }
    }

    this->strip->show();
    this->step++;
    if (this->step > this->maxStep) {
      this->step = 0;
    }
  }

  bool isFinished() override {
    return false;
  }

 private:
  uint32_t color;
  uint8_t meteorSize;
  uint8_t meteorTrailDecay;
  bool meteorRandomDecay = true;
  uint16_t step = 0;
  uint16_t maxStep;

  void meteorRainFadeToBlack(int ledNo, byte fadeValue) {
    uint32_t oldColor;
    uint8_t r, g, b;

    oldColor = this->strip->getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r = (r <= 10) ? 0 : (int)r - (r * fadeValue / 256);
    g = (g <= 10) ? 0 : (int)g - (g * fadeValue / 256);
    b = (b <= 10) ? 0 : (int)b - (b * fadeValue / 256);

    this->strip->setPixelColor(ledNo, r, g, b);
  }
};

#endif