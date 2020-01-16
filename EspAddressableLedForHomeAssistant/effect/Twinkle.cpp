#ifndef _TWINKLE_CPP
#define _TWINKLE_CPP

#include "../rgbw.h"
#include "Common.cpp"
#include "EffectProcessor.cpp"

class Twinkle : public EffectProcessor {
 public:
  static const uint8_t COLOR_SINGLE = 0;
  static const uint8_t COLOR_RANDOM_STRIP = 1;
  static const uint8_t COLOR_RANDOM_DIOD = 2;

  Twinkle(
      Adafruit_NeoPixel *strip,
      RGBW color,
      RGBW backgroudColor,
      uint16_t affectedPixels,
      uint8_t colorPattern) : EffectProcessor(strip) {
    this->setName("twinkle");
    this->color = Adafruit_NeoPixel::Color(color.red, color.green, color.blue, color.white);
    this->colorPattern = colorPattern;
    this->backgroudColor = Adafruit_NeoPixel::Color(backgroudColor.red, backgroudColor.green, backgroudColor.blue, backgroudColor.white);
    this->affectedPixels = affectedPixels;
    this->length = this->strip->numPixels();
    this->diodSequence = new uint16_t[this->length];
    this->prepereDiodSequence();
  }

  ~Twinkle() {
    delete[] this->diodSequence;
  }

  void loop() override {
    if (this->step == 0) {
      this->strip->fill(this->backgroudColor);
      this->strip->show();
    }

    // todo 100% random, find random number then iterate looking for black
    this->strip->setPixelColor(*(this->diodSequence + this->diodPosition), color);
    this->strip->show();

    this->step++;
    if (this->step > affectedPixels) {
      this->step = 0;
      if (this->colorPattern == COLOR_RANDOM_STRIP) {
        this->color = Common::colorWheel(random(255));
      }
    }

    this->diodPosition++;
    if (this->diodPosition > this->length) {
      Common::shuffleArray(diodSequence, this->length);
      this->diodPosition = 0;
    }
      if (this->colorPattern == COLOR_RANDOM_DIOD) {
        this->color = Common::colorWheel(random(255));
      }
  }

  bool isFinished() override {
    return false;
  }

 private:
  uint32_t color;
  uint32_t backgroudColor;
  uint16_t affectedPixels;
  uint16_t step = 0;
  uint16_t length;
  uint16_t *diodSequence;
  uint16_t diodPosition = 0;
  uint8_t colorPattern = 0;

  void prepereDiodSequence() {
    uint16_t length = this->length;
    for (uint16_t i = 0; i < length; i++) {
      *(this->diodSequence + i) = i;
    }
    Common::shuffleArray(this->diodSequence, length);
  }
};

#endif