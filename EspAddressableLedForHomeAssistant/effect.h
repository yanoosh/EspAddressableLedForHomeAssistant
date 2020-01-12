#ifndef _EFFECT_INO
#define _EFFECT_INO

#include "effect/Clear.cpp"
#include "effect/EffectProcessor.cpp"
#include "effect/RainbowCycle.cpp"
#include "effect/Solid.cpp"

int8_t effectLength = 19;
const char *effects[] = {
    "clear",  // 0
    "solid",
    "pixel",
    "twinkle",
    "cylon bounce",  // 4
    "fire",
    "fade in out",
    "strobe",
    "theater chase",
    "rainbow cycle",  // 9
    "color wipe",
    "running lights",
    "snow sparkle",
    "sparkle",
    "twinkle random",  // 14
    "bouncing balls",
    "lightning",
    "meteor rain",
    "color wipe once"};

int8_t getEffectId(const char *name) {
  for (byte i = 0; i < effectLength; i++) {
    if (0 == strcmp(effects[i], name)) {
      return i;
    }
  }
  return -1;
}

void updateEffect(int8_t id, const char *name) {
  EffectProcessor *effectProcessor;
  delete core->getEffectProcessor();

  if (id > -1) {
    switch (id) {
      case 0:
        effectProcessor = new Clear(core->getStrip());
        break;
      case 1:
        effectProcessor = new Solid(core->getStrip(), core->getColor());
        break;
      case 9:
        effectProcessor = new RainbowCycle(core->getStrip());
        break;
      default:
        effectProcessor = NULL;
    }
    core->setEffect(name, effectProcessor);
    return;
  }

  core->setEffect(name, NULL);
}

void updateEffectByName(const char *effect) {
  int8_t id = getEffectId(effect);

  if (id > -1) {
    updateEffect(id, effect);
  }
}

void updateEffectById(int8_t id) {
  if (id < effectLength) {
    updateEffect(id, effects[id]);
  }
  updateEffect(1, effects[1]);
}

#endif
