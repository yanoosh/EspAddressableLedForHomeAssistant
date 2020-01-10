#ifndef _RGBW_H
#define _RGBW_H
struct RGBW {
  byte red;
  byte green;
  byte blue;
  byte white;
};

RGBW mapColor(RGBW color, byte range) {
  color.red = map(color.red, 0, 255, 0, range);
  color.green = map(color.green, 0, 255, 0, range);
  color.blue = map(color.blue, 0, 255, 0, range);
  color.white = map(color.white, 0, 255, 0, range);

  return color;
}

RGBW BLACK = {0, 0, 0, 0};
#endif