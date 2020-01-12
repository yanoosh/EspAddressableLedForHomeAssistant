#ifndef _RGBW_H
#define _RGBW_H

#include <Arduino.h>

struct RGBW {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t white;
};

RGBW mapColor(RGBW color, uint8_t range);
#endif