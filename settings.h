struct RGBW {
  byte red;
  byte green;
  byte blue;
  byte white;
};

struct Setting {
  boolean turnOn;
  RGBW sourceColor;
  RGBW filteredColor;
  byte brightness;
  byte effect;
};

struct MemorizedSetting {
  boolean turnOn;
  RGBW color;
  byte brightness;
};

Setting setting = {
  false,
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  255
};

RGBW colorBlack = {0, 0, 0, 0};

void cpyColor(RGBW *destination, RGBW *source) {
  memcpy(destination, source, sizeof(RGBW));
}

void mapColor(RGBW *destination, RGBW *source, byte range) {
  destination->red = map(source->red, 0, 255, 0, range);
  destination->green = map(source->green, 0, 255, 0, range);
  destination->blue = map(source->blue, 0, 255, 0, range);
  destination->white = map(source->white, 0, 255, 0, range);
}
//setting.filteredColor.
