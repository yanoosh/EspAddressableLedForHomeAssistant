#ifndef SETTINGS_H
#define SETTINGS_H
#include "rgbw.h"
#include <EEPROM.h>

class Setting {
  public:
    void setTurnOn(boolean turnOn) {
      this->turnOn = turnOn;
    }
    
    boolean getTurnOn() {
      return this->turnOn;
    }
    
    void setSourceColor(RGBW color) {
      this->sourceColor = color;
      this->updateFilteredColor();
    }
    
    RGBW getSourceColor() {
      return sourceColor;
    }

    void setFilteredColor(RGBW color) {
      this->filteredColor = color;
    }
    
    RGBW getFilteredColor() {
      return filteredColor;
    }

    void setBrightness(byte brightness) {
      this->brightness = brightness;
    }

    byte getBrightness() {
      return this->brightness;
    }

    void setEffect(byte effect) {
      this->effect = effect;
    }

    byte getEffect() {
      return this->effect;
    }

    void setSpeed(byte speed) {
      this->speed = speed;
    }

    byte getSpeed() {
      return this->speed;
    }

  private:
    boolean turnOn;
    RGBW sourceColor;
    RGBW filteredColor;
    byte brightness;
    byte effect;
    byte speed;

    void updateFilteredColor() {
      this->filteredColor = mapColor(this->sourceColor, this->brightness);
    }
};

class MemmorizedSetting {
  public:
    void init() {
      EEPROM.begin(sizeof(Memorized));
    }
    void load(Setting *current) {
      Memorized memorized = {};
      EEPROM.get(0, memorized);
      current->setTurnOn(memorized.turnOn);
      current->setSourceColor(memorized.color);
      current->setBrightness(memorized.brightness);
      current->setEffect(memorized.effect);
      current->setSpeed(memorized.speed);
    }

    void saveChanges(Setting *current) {
      Memorized memorized = {};
      memorized.turnOn = current->getTurnOn();
      memorized.color = current->getSourceColor();
      memorized.brightness = current->getBrightness();
      memorized.effect = current->getEffect();
      memorized.speed = current->getSpeed();
      
      EEPROM.put(0, memorized);
      EEPROM.commit();
    }

  private:
    struct Memorized {
      boolean turnOn;
      RGBW color;
      byte brightness;
      byte effect;
      byte speed;
    };
};
//setting.filteredColor.
#endif
