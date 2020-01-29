#ifndef _TWINKLE_CPP
#define _TWINKLE_CPP

#include "Common.cpp"
#include "EffectProcessor.cpp"

class Twinkle : public EffectProcessor {
 public:
  Twinkle(
      Adafruit_NeoPixel *strip,
      uint16_t affectedPixels,
      uint8_t colorPattern) : EffectProcessor(strip) {
    this->setName("twinkle");
    this->affectedPixels = affectedPixels;
    this->colorPattern = colorPattern;
    this->length = this->strip->numPixels();
    this->diodSequence = new uint16_t[this->length];
    this->prepereDiodSequence();
  }

  ~Twinkle() {
    delete[] this->diodSequence;
  }

  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    this->backgroudColor.white = white;
    this->backgroudColor.red = red;
    this->backgroudColor.green = green;
    this->backgroudColor.blue = blue;
  }

  void loop() override {
    if (this->step == 0) {
      this->strip->fill(this->backgroudColor.raw);
      this->strip->show();
    }

    // todo 100% random, find random number then iterate looking for black
    this->strip->setPixelColor(*(this->diodSequence + this->diodPosition), this->color.raw);
    this->strip->show();

    this->step++;
    if (this->step > affectedPixels) {
      this->step = 0;
      if (this->colorPattern == COLOR_RANDOM_STRIP) {
        this->color.raw = Common::colorWheel(random(255));
      }
    }

    this->diodPosition++;
    if (this->diodPosition > this->length) {
      Common::shuffleArray(diodSequence, this->length);
      this->diodPosition = 0;
    }
    if (this->colorPattern == COLOR_RANDOM_DIOD) {
      this->color.raw = Common::colorWheel(random(255));
    }
  }

  bool isFinished() override {
    return false;
  }

 private:
  Color backgroudColor = {.raw = 0};
  uint16_t affectedPixels;
  uint8_t colorPattern = 0;
  uint16_t step = 0;
  uint16_t length;
  uint16_t *diodSequence;
  uint16_t diodPosition = 0;

  void prepereDiodSequence() {
    uint16_t length = this->length;
    for (uint16_t i = 0; i < length; i++) {
      *(this->diodSequence + i) = i;
    }
    Common::shuffleArray(this->diodSequence, length);
  }
};

#endif