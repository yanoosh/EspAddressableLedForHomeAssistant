#ifndef _COLOR_PROVIDER_CPP
#define _COLOR_PROVIDER_CPP

#include "../effect/Common.cpp"
#include "../effect/EffectProcessor.cpp"
// @todo 
// - check all effects (use step from parent class)
// - how config by home assitant ?
enum ColorMode { SOLID = 0,
                 RANDOM_LOOP = 1,
                 RANDOM = 2,
                 RAINBOW_LOOP = 3,
                 RAINBOW = 4};

class ColorProcessor {
 public:
  void setMainColor(Color color) {
    this->color = color;
  }

  Color getMainColor() {
    return this->color;
  }

  void setMode(ColorMode newMode) {
    mode = newMode;
  }

  ColorMode getMode() {
    return mode;
  }

  void loop(EffectProcessor *effectProcessor) {
    if (effectProcessor->getStep() <= 0) {
      if (this->mode == ColorMode::SOLID) {
        effectProcessor->setColor(this->color);
      }
      if (this->mode == ColorMode::RANDOM_LOOP) {
        randomColor(effectProcessor);
        effectProcessor->setColor(tmpColor);
      }
      if (this->mode == ColorMode::RAINBOW_LOOP) {
        rainbowColor();
        effectProcessor->setColor(tmpColor);
      }
    }
    if (this->mode == ColorMode::RANDOM) {
      randomColor(effectProcessor);
      effectProcessor->setColor(tmpColor);
    }
    if (this->mode == ColorMode::RAINBOW) {
      rainbowColor();
      effectProcessor->setColor(tmpColor);
    }
  }

 private:
  Color color = {.raw = 0x00FFFFFF};
  Color tmpColor = {.raw = 0x00FFFFFF};
  ColorMode mode = ColorMode::RAINBOW;
  byte rainbowStep = 0;

  void rainbowColor() {
    tmpColor.raw = Common::colorWheel(rainbowStep);
    rainbowStep++;
  }

  void randomColor(EffectProcessor *effectProcessor) {
    tmpColor.raw = Common::colorWheel(random(255));
  }
};

#endif