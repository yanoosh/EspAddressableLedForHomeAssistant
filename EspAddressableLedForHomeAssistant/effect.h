#ifndef _EFFECT_INO
#define _EFFECT_INO

#include "effect/Clear.cpp"
#include "effect/ColorWipeRainbow.cpp"
#include "effect/CylonBounce.cpp"
#include "effect/EffectProcessor.cpp"
#include "effect/Fade.cpp"
#include "effect/Fire.cpp"
#include "effect/MeteorRain.cpp"
#include "effect/RainbowCycle.cpp"
#include "effect/Solid.cpp"
#include "effect/Strobe.cpp"
#include "effect/TheaterChase.cpp"
#include "effect/Twinkle.cpp"

int8_t effectLength = 19;
const char *effects[] = {
    "clear",  // 0
    "solid",
    "twinkle",
    "cylon bounce",
    "fire",  // 4
    "fade in out",
    "strobe",
    "theater chase",
    "rainbow cycle",
    "color wipe",  // 9
    "running lights",
    "snow sparkle",
    "sparkle",
    "twinkle random",
    "bouncing balls",  // 14
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
      case 2:
        effectProcessor = new Twinkle(core->getStrip(), core->getColor(), BLACK, core->getStrip()->numPixels() / 5, Twinkle::COLOR_SINGLE);
        break;
      case 3:
        effectProcessor = new CylonBounce(core->getStrip(), core->getColor());
        break;
      case 4:
        effectProcessor = new Fire(core->getStrip());
        break;
      case 5:
        effectProcessor = new Fade(core->getStrip(), core->getColor(), 1, -1, 20, false);
        break;
      case 6:
        effectProcessor = new Strobe(core->getStrip(), core->getColor());
        break;
      case 7:
        effectProcessor = new TheaterChase(core->getStrip(), core->getColor());
        break;
      case 8:
        effectProcessor = new RainbowCycle(core->getStrip());
        break;
      case 9:
        effectProcessor = new ColorWipeRainbow(core->getStrip());
        break;
      case 11:
        effectProcessor = new Twinkle(core->getStrip(), {0, 0, 0, 1}, core->getColor(), 1, Twinkle::COLOR_SINGLE);
        effectProcessor->setName(effects[11]);
        break;
      case 12:
        effectProcessor = new Twinkle(core->getStrip(), core->getColor(), {0, 0, 0, 0}, 1, Twinkle::COLOR_SINGLE);
        effectProcessor->setName(effects[12]);
        break;
      case 13:
        effectProcessor = new Twinkle(core->getStrip(), {0, 0, 0, 0}, {0, 0, 0, 0}, core->getStrip()->numPixels(), Twinkle::COLOR_RANDOM_DIOD);
        effectProcessor->setName(effects[13]);
        break;
      case 16:
        effectProcessor = new MeteorRain(core->getStrip(), core->getColor());
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
