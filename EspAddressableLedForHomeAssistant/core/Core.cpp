
#include "Core.hpp"

void Core::setup() {
  this->generateNameWhenEmpty();
  this->strip = new Adafruit_NeoPixel(this->length, this->stripPin, NEO_RGB + NEO_KHZ400);
  this->diode = new Diode(this->strip);
  this->strip->begin();
  this->strip->show();

  this->strip->fill(DARK_YELLOW, 0);
  this->strip->setPixelColor(this->strip->numPixels() - 1, GREEN);
  this->strip->show();
  this->mqtt->generateTopics(this->deviceName);
}

Adafruit_NeoPixel *Core::getStrip() {
  return this->strip;
}

Diode *Core::getDiode() {
  return this->diode;
}

void Core::setLength(uint16_t length) {
  this->length = length;
}

uint16_t Core::getLength() {
  return this->length;
}

void Core::setStripPin(uint16_t stripPin) {
  this->stripPin = stripPin;
}

uint16_t Core::getStripPin() {
  return this->stripPin;
}

void Core::setDeviceName(const char *deviceName) {
  this->deviceName = deviceName;
}

const char *Core::getDeviceName() {
  return this->deviceName;
}

void Core::setTurnOn(bool turnOn) {
  this->turnOn = turnOn;
  this->syncLoopAndState();
}

bool Core::isTurnOn() {
  return this->turnOn;
}

void Core::setColor(Color color) {
  this->color = color;
  this->syncLoopAndState();
  if (this->effectProcessor != NULL) {
    this->effectProcessor->setColor(color);
  }
}

Color Core::getColor() {
  return color;
}

void Core::setBrightness(uint8_t brightness) {
  this->brightness = max((uint8_t)0, min(brightness, this->maxBrightness));
  this->strip->setBrightness(this->brightness);
  this->syncLoopAndState();
}

uint8_t Core::getBrightness() {
  return this->brightness;
}

void Core::setMaxBrightness(uint8_t maxBrightness) {
  this->maxBrightness = maxBrightness;
  this->setBrightness(this->brightness);
}

uint8_t Core::getMaxBrightness() {
  return this->brightness;
}

void Core::setSpeed(byte speed) {
  this->speed = speed;
}

byte Core::getSpeed() {
  return this->speed;
}

void Core::setEffect(EffectProcessor *effectProcessor) {
  this->effectProcessor = effectProcessor;
  this->syncLoopAndState();
}

const char *Core::getEffectName() {
  if (this->effectProcessor != NULL) {
    return this->effectProcessor->getName();
  } else {
    return "";
  }
}

EffectProcessor *Core::getEffectProcessor() {
  return this->effectProcessor;
}

void Core::disableLoop() {
  this->loopEnabled = false;
}

bool Core::isLoopEnabled() {
  return this->loopEnabled;
}

void Core::setTransitionInterval(uint8_t transitionInterval) {
  this->transitionInterval = transitionInterval;
}

uint8_t Core::getTransitionInterval() {
  return this->transitionInterval;
}

void Core::syncLoopAndState() {
  this->loopEnabled = this->turnOn;
}

void Core::generateNameWhenEmpty() {
  if (strlen(this->deviceName) == 0) {
    this->deviceName = new char[12 + 1];
    sprintf((char *)this->deviceName, "led-%d", ESP.getChipId());
  }
}