#ifndef _EFFECT_CPP
#define _EFFECT_CPP
#include <Adafruit_NeoPixel.h>

#include "../effect/Clear.cpp"
#include "../effect/ColorWipeRainbow.cpp"
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

#define EFFECT_LENGTH 16

class Effect {
 public:
  Effect(Adafruit_NeoPixel *strip) {
    this->length = EFFECT_LENGTH;
    this->strip = strip;
    this->processors[0] = new Clear(strip);
    this->processors[1] = new Solid(strip);
    this->processors[2] = new Twinkle(strip, strip->numPixels() / 5, Twinkle::COLOR_SINGLE);
    this->processors[3] = new CylonBounce(strip);
    this->processors[4] = new Fire(strip);
    this->processors[5] = new Fade(strip, 1, -1, 20, false);
    this->processors[6] = new Strobe(strip);
    this->processors[7] = new TheaterChase(strip);
    this->processors[8] = new RainbowCycle(strip);
    this->processors[9] = new ColorWipeRainbow(strip);
    this->processors[10] = new Wave(strip, 0.5, 0.1);
    this->processors[10]->setName("running lights");
    this->processors[11] = new Twinkle(strip, 1, Twinkle::COLOR_SINGLE);
    ((Twinkle *)this->processors[11])->setBackgroundColor(255, 255, 255, 255);
    this->processors[11]->setName("snow sparkle");
    this->processors[12] = new Twinkle(strip, 1, Twinkle::COLOR_SINGLE);
    this->processors[12]->setName("sparkle");
    this->processors[13] = new Twinkle(strip, strip->numPixels(), Twinkle::COLOR_RANDOM_DIOD);
    this->processors[13]->setName("twinkle random");
    this->processors[14] = new Lightning(strip);
    this->processors[15] = new MeteorRain(strip);
    setActiveById(0);
  }

  void setColor(Color color) {
    this->color = color;
    if (this->active != NULL) {
      this->active->setColor(color);
    }
  }

  Color getColor() {
    return color;
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
        processors[id]->setColor(color.red, color.green, color.blue, color.white);
        activeId = id;
        active = processors[id];
      }
    }
  }

 private:
  Color color = {.raw = 0x00FFFFFF};
  uint8_t length;
  uint8_t activeId;
  Adafruit_NeoPixel *strip;
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