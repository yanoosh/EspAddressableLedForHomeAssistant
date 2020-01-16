#ifndef _COMMON_CPP
#define _COMMON_CPP


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Common {
 public:
  static uint32_t colorWheel(byte wheelPos) {
    if (wheelPos < 85) {
      return Adafruit_NeoPixel::Color(wheelPos * 3, 255 - wheelPos * 3, 0);
    } else if (wheelPos < 170) {
      wheelPos -= 85;
      return Adafruit_NeoPixel::Color(255 - wheelPos * 3, 0, wheelPos * 3);
    } else {
      wheelPos -= 170;
      return Adafruit_NeoPixel::Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
  }

  static void shuffleArray(uint16_t arr[], uint16_t n) {
    srand(time(NULL));
    for (uint16_t i = n - 1; i > 0; i--) {
      uint16_t j = rand() % (i + 1);
      uint16_t temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    }
  }
};

#endif