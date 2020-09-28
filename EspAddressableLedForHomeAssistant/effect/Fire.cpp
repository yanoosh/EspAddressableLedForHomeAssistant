#ifndef _FIRE_CPP
#define _FIRE_CPP

#include "EffectProcessor.cpp"

class Fire : public EffectProcessor {
 public:
  Fire(Adafruit_NeoPixel *strip) : EffectProcessor(strip) {
    this->setName("fire");
    this->heat = new uint8_t[strip->numPixels()];
    this->cooling = 55;
    this->sparking = 120;
  }

  ~Fire() {
    delete[] this->heat;
  }

  void loop() override {
    uint8_t cooldown, i, count;
    count = strip->numPixels();

    // Step 1.  Cool down every cell a little
    for (i = 0; i <= count; i++) {
      cooldown = random(0, ((this->cooling * 10) / count) + 2);

      if (cooldown > heat[i]) {
        heat[i] = 0;
      } else {
        heat[i] = heat[i] - cooldown;
      }
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int i = count - 1; i >= 2; i--) {
      heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' near the bottom
    if (random(255) < this->sparking) {
      int i = random(7);
      heat[i] = heat[i] + random(160, 255);
      //heat[i] = random(160,255);
    }

    // Step 4.  Convert heat to LED colors
    for (int i = 0; i <= count; i++) {
      setPixelHeatColor(i, heat[i]);
    }

    this->strip->show();
  }

 private:
  uint8_t *heat;
  uint8_t cooling;
  uint8_t sparking;

  void setPixelHeatColor(int pixel, byte temperature) {
    // Scale 'heat' down from 0-255 to 0-191
    uint8_t t192 = round((temperature / 255.0) * 191);

    // calculate ramp up from
    uint8_t heatramp = t192 & 0x3F;  // 0..63
    heatramp <<= 2;                  // scale up to 0..252

    // figure out which third of the spectrum we're in:
    if (t192 > 0x80) {  // hottest
      this->strip->setPixelColor(pixel, 255, 255, heatramp, 0);
    } else if (t192 > 0x40) {  // middle
      this->strip->setPixelColor(pixel, 255, heatramp, 0, 0);
    } else {  // coolest
      this->strip->setPixelColor(pixel, heatramp, 0, 0, 0);
    }
  }
};

#endif