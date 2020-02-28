#ifndef _DIODE_CPP
#define _DIODE_CPP

#include <Adafruit_NeoPixel.h>

class Diode {
 public:
  Diode(Adafruit_NeoPixel *strip) {
    this->strip = strip;
  }

  void progress(uint32_t color) {
    this->strip->setPixelColor(this->step++, color);
    this->strip->show();
  }

  void done() {
    this->step = 0;
  }

 private:
  Adafruit_NeoPixel *strip;
  uint8_t step = 0;
};

#endif