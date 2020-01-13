#ifndef _DIODE_H
#define _DIODE_H

#include <Adafruit_NeoPixel.h>

class Diode {
 public:
  Diode(Adafruit_NeoPixel *strip);

  void progress(uint32 color);

  void done();

 private:
  Adafruit_NeoPixel *strip;
  uint8_t step = 0;
};

#endif