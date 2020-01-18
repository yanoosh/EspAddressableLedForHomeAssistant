#ifndef _FADE_CPP
#define _FADE_CPP

#include <Arduino.h>
#include "EffectProcessor.cpp"

class Fade : public EffectProcessor {
 public:
  // loop -1 ifinite
  // fade in: direction 1, loops 1;
  // fade out: direction -1, loops 1;
  // fade in->out->in->...: direction 1, loops -1;
  // fade in->in->in->...: direction 1, loops -1, cut = true;
  Fade(
      Adafruit_NeoPixel *strip,
      int8_t direction,
      int8_t loops,
      uint8_t smoothness,
      bool cut) : EffectProcessor(strip) {
    this->setName("fade in out");
    this->color = color;
    this->loops = loops;
    this->smoothness = max(smoothness, (uint8_t)1);
    this->cut = cut;
    this->direction = direction > 0 ? 1 : -1;
    if (this->direction > 0) {
      this->step = 0;
    } else {
      this->step = this->smoothness;
    }
  }

  void loop() override {
    if (this->loops != 0) {
      uint32_t color = Adafruit_NeoPixel::Color(
        this->color.r * this->step / this->smoothness, 
        this->color.g * this->step / this->smoothness, 
        this->color.b * this->step / this->smoothness, 
        this->color.w * this->step / this->smoothness
      );
      this->strip->fill(color);
      this->strip->show();
      this->step += this->direction;
      if (this->step > this->smoothness || this->step < 0) {
        if (loops > 0) {
          loops--;
        }
      }
      if (this->step > this->smoothness) {
        if (this->cut) {
          this->step = 0;
        } else {
          this->direction = -1;
          this->step = this->smoothness;
        }
      } else if (this->step < 0) {
        if (this->cut) {
          this->step = this->smoothness;
        } else {
          this->direction = 1;
          this->step = 0;
        }
      }
    }
  }

  bool isFinished() override {
    return this->loops == 0;
  }

 private:
  int8_t step;
  int8_t direction;
  int8_t loops;
  uint8_t smoothness;
  bool cut;
};

#endif