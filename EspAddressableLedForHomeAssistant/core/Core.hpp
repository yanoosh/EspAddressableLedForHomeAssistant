#ifndef _CORE_HPP
#define _CORE_HPP

#include "../effect/EffectProcessor.cpp"
#include "../rgbw.h"
#include "Adafruit_NeoPixel.h"
#include "Diode.hpp"
#include "MqttProperties.hpp"

class Core {
 public:
  static const uint32_t RED = 0x00330000;
  static const uint32_t GREEN = 0x00003300;
  static const uint32_t BLUE = 0x00000033;
  static const uint32_t DARK_YELLOW = 0x00333300;
  MqttProperties *mqtt = new MqttProperties();

  void setup();

  Adafruit_NeoPixel *getStrip();

  Diode *getDiode();

  void setLength(uint16_t length);

  uint16_t getLength();

  void setStripPin(uint16_t stripPin);

  uint16_t getStripPin();

  void setDeviceName(const char *deviceName);

  const char *getDeviceName();

  void setTurnOn(bool turnOn);

  bool isTurnOn();

  void setColor(RGBW color);

  RGBW getColor();

  void setBrightness(uint8_t brightness);

  uint8_t getBrightness();

  void setMaxBrightness(uint8_t maxBrightness);

  uint8_t getMaxBrightness();

  void setSpeed(byte speed);

  byte getSpeed();

  void setEffect(EffectProcessor *effectProcessor);

  const char *getEffectName();

  EffectProcessor *getEffectProcessor();

  void disableLoop();

  bool isLoopEnabled();

  void setTransitionInterval(uint8_t transitionInterval);

  uint8_t getTransitionInterval();

 private:
  Adafruit_NeoPixel *strip;
  Diode *diode;
  uint16_t length = 50;
  uint8_t stripPin;
  const char *deviceName = "";
  bool turnOn;
  RGBW color = {255, 255, 255, 0};
  uint8_t brightness = 255;
  uint8_t maxBrightness = 255;
  EffectProcessor *effectProcessor;
  uint8_t speed;
  bool loopEnabled = true;
  uint8_t transitionInterval = 150;

  void syncLoopAndState();

  void generateNameWhenEmpty();
};

#endif