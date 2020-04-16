#ifndef _MEMORIZED_SETTING_CPP
#define _MEMORIZED_SETTING_CPP

#include <Arduino.h>
#include <EEPROM.h>
#include "./Core.cpp"

class MemmorizedSetting {
 public:
  void init() {
    EEPROM.begin(sizeof(Memorized));
  }
  void load(Core *core) {
    Memorized memorized = {};
    EEPROM.get(0, memorized);
    core->setTurnOn(memorized.turnOn);
    core->setBrightness(memorized.brightness);
    core->getEffect()->setColor(memorized.color);
    core->getEffect()->setActiveById(memorized.effect);
    core->setSpeed(memorized.speed);
  }

  void saveChanges(Core *core) {
    Memorized memorized = {};
    memorized.turnOn = core->isTurnOn();
    memorized.color = core->getEffect()->getColor();
    memorized.brightness = core->getBrightness();
    // todo set correct id
    memorized.effect = core->getEffect()->getActiveId();
    memorized.speed = core->getSpeed();

    EEPROM.put(0, memorized);
    EEPROM.commit();
  }

 private:
  struct Memorized {
    boolean turnOn;
    Color color = {.raw = 0};
    int8_t brightness;
    int8_t effect;
    int8_t speed;
  };
};
// core->filteredColor.
#endif
