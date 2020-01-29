#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>

// class MemmorizedSetting {
//  public:
//   void init() {
//     EEPROM.begin(sizeof(Memorized));
//   }
//   void load(Setting *current) {
//     Memorized memorized = {};
//     EEPROM.get(0, memorized);
//     current->setTurnOn(memorized.turnOn);
//     current->setSourceColor(memorized.color);
//     current->setBrightness(memorized.brightness);
//     // todo set correct effect usig id
//     // current->setEffect(memorized.effect);
//     current->setSpeed(memorized.speed);
//   }

//   void saveChanges(Setting *current) {
//     Memorized memorized = {};
//     memorized.turnOn = current->getTurnOn();
//     memorized.color = current->getColor();
//     memorized.brightness = current->getBrightness();
//     // todo set correct id
//     memorized.effect = 0;
//     memorized.speed = current->getSpeed();

//     EEPROM.put(0, memorized);
//     EEPROM.commit();
//   }

//  private:
//   struct Memorized {
//     boolean turnOn;
//     RGBW color;
//     byte brightness;
//     byte effect;
//     byte speed;
//   };
// };
//core->filteredColor.
#endif
