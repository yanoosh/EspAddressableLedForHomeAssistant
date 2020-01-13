#include "diode.h"
#include <Adafruit_NeoPixel.h>

Diode::Diode(Adafruit_NeoPixel *strip) {
  this->strip = strip;
}

void Diode::progress(uint32_t color) {
  this->strip->setPixelColor(this->step++, color);
  this->strip->show();
}

void Diode::done() {
  this->step = 0;
}