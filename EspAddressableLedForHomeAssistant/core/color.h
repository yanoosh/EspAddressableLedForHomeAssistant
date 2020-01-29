#ifndef _COLOR_H
#define _COLOR_H

#include <Arduino.h>

union Color {
  struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t white = 0;
  };
  uint32_t raw;
};

#endif