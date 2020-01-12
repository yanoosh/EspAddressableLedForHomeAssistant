#include "rgbw.h"

RGBW mapColor(RGBW color, uint8_t range) {
  color.red = map(color.red, 0, 255, 0, range);
  color.green = map(color.green, 0, 255, 0, range);
  color.blue = map(color.blue, 0, 255, 0, range);
  color.white = map(color.white, 0, 255, 0, range);

  return color;
}