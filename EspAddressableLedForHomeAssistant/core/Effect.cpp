#ifndef _EFFECT_CPP
#define _EFFECT_CPP
#include <Adafruit_NeoPixel.h>

#include "../effect/Clear.cpp"
#include "../effect/ColorWipe.cpp"
#include "../effect/CylonBounce.cpp"
#include "../effect/EffectProcessor.cpp"
#include "../effect/Fade.cpp"
#include "../effect/Fire.cpp"
#include "../effect/Lightning.cpp"
#include "../effect/MeteorRain.cpp"
#include "../effect/RainbowCycle.cpp"
#include "../effect/Solid.cpp"
#include "../effect/Strobe.cpp"
#include "../effect/TheaterChase.cpp"
#include "../effect/Twinkle.cpp"
#include "../effect/Wave.cpp"
#include "ColorProcessor.cpp"

#define EFFECT_LENGTH 15

class Effect {
 public:
  Effect(Adafruit_NeoPixel *strip) {
    this->length = EFFECT_LENGTH;
    this->strip = strip;
    this->colorProcessor = new ColorProcessor();

    this->processors[0] = new Solid(strip);
    this->processors[1] = new MeteorRain(strip);
    this->processors[2] = new Twinkle(strip, strip->numPixels() / 5, Twinkle::COLOR_SINGLE);
    this->processors[3] = new CylonBounce(strip);
    this->processors[4] = new Fire(strip);
    this->processors[5] = new Fade(strip, 1, -1, 64, false);
    this->processors[6] = new Strobe(strip);
    this->processors[7] = new TheaterChase(strip);
    this->processors[8] = new RainbowCycle(strip);
    this->processors[9] = new ColorWipe(strip);
    this->processors[10] = new Wave(strip, 0.5, 0.1);
    this->processors[10]->setName("running lights");
    this->processors[11] = new Twinkle(strip, 1, Twinkle::COLOR_SINGLE);
    ((Twinkle *)this->processors[11])->setBackgroundColor(255, 255, 255, 255);
    this->processors[11]->setName("snow sparkle");
    this->processors[12] = new Twinkle(strip, 1, Twinkle::COLOR_SINGLE);
    this->processors[12]->setName("sparkle");
    this->processors[13] = new Twinkle(strip, strip->numPixels(), Twinkle::COLOR_RANDOM_DIOD);
    this->processors[13]->setName("twinkle random");
    this->processors[EFFECT_LENGTH - 1] = new Lightning(strip);
    setActiveById(0);
  }

  void setColor(Color color) {
    this->colorProcessor->setMainColor(color);
  }

  Color getColor() {
    return colorProcessor->getMainColor();
  }

  void setColorMode(int mode) {
    switch (mode) {
      case 1:
        colorProcessor->setMode(RANDOM_LOOP);
        break;
      case 2:
        colorProcessor->setMode(RANDOM);
        break;
      case 3:
        colorProcessor->setMode(RAINBOW_LOOP);
        break;
      case 4:
        colorProcessor->setMode(RAINBOW);
        break;
      default:
        colorProcessor->setMode(SOLID);
        break;
    }
  }

  byte getColorMode() {
    return colorProcessor->getMode();
  }

  uint8_t getLength() {
    return length;
  }

  EffectProcessor **getProcessors() {
    return this->processors;
  }

  EffectProcessor *getActive() {
    return active;
  }

  const char *getActiveName() {
    if (this->active != NULL) {
      return this->active->getName();
    } else {
      return "";
    }
  }

  const uint8_t getActiveId() {
    return activeId;
  }

  void setActiveByName(const char *effect) {
    setActiveById(getEffectId(effect));
  }

  void setActiveById(int8_t id) {
    if (id > -1 && id < length) {
      if (processors[id] != NULL) {
        activeId = id;
        active = processors[id];
      }
    }
  }

  void loop() {
    if (this->active != NULL) {
      this->colorProcessor->loop(this->active);
      this->active->loop();
    }
  }

 private:
  uint8_t length;
  uint8_t activeId;
  Adafruit_NeoPixel *strip;
  ColorProcessor *colorProcessor;
  EffectProcessor *processors[EFFECT_LENGTH];
  EffectProcessor *active = NULL;

  int8_t getEffectId(const char *name) {
    for (byte i = 0; i < length; i++) {
      if (processors[i] != NULL && 0 == strcmp(processors[i]->getName(), name)) {
        return i;
      }
    }
    return -1;
  }
};

#endif