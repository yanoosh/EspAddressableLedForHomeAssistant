#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "rgbw.h"
#include "effect/EffectProcessor.cpp"

class Setting {
  public:
    Adafruit_NeoPixel *strip;

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
      this->updateFilteredColor();
    }

    byte getBrightness() {
      return this->brightness;
    }

    void setEffect(const char* name, EffectProcessor *effectProcessor) {
      // todo remove name after refactor all effect to class
      this->effectName = name;
      this->effectProcessor = effectProcessor;
    }

    const char *getEffectName() {
      return this->effectName;
    }

    EffectProcessor *getEffectProcessor() {
      return this->effectProcessor;
    }

    void setSpeed(byte speed) {
      this->speed = speed;
    }

    byte getSpeed() {
      return this->speed;
    }

  private:
    boolean turnOn;
    RGBW sourceColor = {255, 255, 255, 0};
    RGBW filteredColor;
    byte brightness;
    const char *effectName;
    EffectProcessor *effectProcessor;
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
      // todo set correct effect usig id
      // current->setEffect(memorized.effect);
      current->setSpeed(memorized.speed);
    }

    void saveChanges(Setting *current) {
      Memorized memorized = {};
      memorized.turnOn = current->getTurnOn();
      memorized.color = current->getSourceColor();
      memorized.brightness = current->getBrightness();
      // todo set correct id
      memorized.effect = 0;
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
