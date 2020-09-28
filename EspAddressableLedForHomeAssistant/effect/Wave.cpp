#ifndef _RUNNING_LIGHTS_CPP
#define _RUNNING_LIGHTS_CPP

#include "EffectProcessor.cpp"

class Wave : public EffectProcessor {
 public:
  Wave(Adafruit_NeoPixel *strip, float density, float smoothness) : EffectProcessor(strip) {
    this->setName("wave");
    this->density = density;
    this->smoothness = smoothness;
    this->length = this->strip->numPixels();
  }

  void loop() override {
    for(uint16_t i = 0; i < this->length; i++) {
      this->strip->setPixelColor(
        i,
        this->pixelColor(this->color.red, i),
        this->pixelColor(this->color.green, i),
        this->pixelColor(this->color.blue, i),
        this->pixelColor(this->color.white, i)
      );
    }
    this->strip->show();

    this->step += this->smoothness;
    if (this->step > this->length) {
      this->step = 0;
    }
  }

 private:
  float density;
  float smoothness;
  float step = 0;
  uint16_t length = 0;

  uint8_t pixelColor(uint8_t base, float position) {
    return (sin(((float)position / this->density) + (float)this->step) * 127 + 128) / 255 * base;
  }
};

#endif