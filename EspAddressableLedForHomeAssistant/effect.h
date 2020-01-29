#ifndef _EFFECT_INO
#define _EFFECT_INO

#include "effect/Clear.cpp"
#include "effect/ColorWipeRainbow.cpp"
#include "effect/CylonBounce.cpp"
#include "effect/EffectProcessor.cpp"
#include "effect/Fade.cpp"
#include "effect/Fire.cpp"
#include "effect/Lightning.cpp"
#include "effect/MeteorRain.cpp"
#include "effect/RainbowCycle.cpp"
#include "effect/Wave.cpp"
#include "effect/Solid.cpp"
#include "effect/Strobe.cpp"
#include "effect/TheaterChase.cpp"
#include "effect/Twinkle.cpp"
#include "rgbw.h"

#define EFFECT_LENGTH 16
  
EffectProcessor *effectProcessors[EFFECT_LENGTH];

int8_t getEffectId(const char *name) {
  for (byte i = 0; i < EFFECT_LENGTH; i++) {
    if (effectProcessors[i] != NULL && 0 == strcmp(effectProcessors[i]->getName(), name)) {
      return i;
    }
  }
  return -1;
}

void setupEffects() {
  memset(effectProcessors, EFFECT_LENGTH, sizeof(EffectProcessor *));
  effectProcessors[0] = new Clear(core->getStrip());
  effectProcessors[1] = new Solid(core->getStrip());
  effectProcessors[2] = new Twinkle(core->getStrip(), core->getStrip()->numPixels() / 5, Twinkle::COLOR_SINGLE);
  effectProcessors[3] = new CylonBounce(core->getStrip());
  effectProcessors[4] = new Fire(core->getStrip());
  effectProcessors[5] = new Fade(core->getStrip(), 1, -1, 20, false);
  effectProcessors[6] = new Strobe(core->getStrip());
  effectProcessors[7] = new TheaterChase(core->getStrip());
  effectProcessors[8] = new RainbowCycle(core->getStrip());
  effectProcessors[9] = new ColorWipeRainbow(core->getStrip());
  effectProcessors[10] = new Wave(core->getStrip(), 0.5, 0.1);
  effectProcessors[10]->setName("running lights");
  effectProcessors[11] = new Twinkle(core->getStrip(), 1, Twinkle::COLOR_SINGLE);
  ((Twinkle *)effectProcessors[11])->setBackgroundColor(255, 255, 255, 255);
  effectProcessors[11]->setName("snow sparkle");
  effectProcessors[12] = new Twinkle(core->getStrip(), 1, Twinkle::COLOR_SINGLE);
  effectProcessors[12]->setName("sparkle");
  effectProcessors[13] = new Twinkle(core->getStrip(), core->getStrip()->numPixels(), Twinkle::COLOR_RANDOM_DIOD);
  effectProcessors[13]->setName("twinkle random");
  effectProcessors[14] = new Lightning(core->getStrip());
  effectProcessors[15] = new MeteorRain(core->getStrip());
}

void updateEffect(int8_t id) {
  if (id > -1 && id < EFFECT_LENGTH) {
    if (effectProcessors[id] != NULL) {
      RGBW color = core->getColor();
      effectProcessors[id]->setColor(color.red, color.green, color.blue, color.white);
      core->setEffect(effectProcessors[id]);
    }
  }
}

void updateEffectByName(const char *effect) {
  updateEffect(getEffectId(effect));
}

void updateEffectById(int8_t id) {
  updateEffect(id); 
}

#endif
