#ifndef _LIGHTNING_CPP
#define _LIGHTNING_CPP

#include "EffectProcessor.cpp"

class Lightning : public EffectProcessor {
 public:
  Lightning(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("lightning");
    this->ledCount = this->strip->numPixels();
    this->strikes = random(1, 4);
    this->strip->clear();
  }

  void loop() override {
    while (this->step > 0) {
      this->step--;
      return;
    }

    while (this->strikes > 0) {
      uint16_t start = random(ledCount);
      uint16_t effectLength = random(ledCount - start);
      this->strip->fill(this->color.raw, start, effectLength);
      this->strip->show();
      this->strip->fill(0, start, effectLength);
      this->strikes--;
      return;
    }

    this->strip->show();
    this->step = random(20, 100);
    this->strikes = random(1, 4);
  }

 private:
  uint16_t ledCount = 0;
  uint8_t strikes = 0;
  bool explode = true;
};

#endif