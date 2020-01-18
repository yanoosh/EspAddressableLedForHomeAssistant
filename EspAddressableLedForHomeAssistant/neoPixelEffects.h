#ifndef _NEOPIXEL_EFFECTS
#define _NEOPIXEL_EFFECTS
#include "effect/RainbowCycle.cpp"

///**************************** START EFFECTS *****************************************/
// Effects from: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/

bool shouldAbortEffect() {
  yield();
  ESP.wdtFeed();
  // todo move to mqtt file
  mqtt.loop();  // Update from MQTT
  return transitionAbort;
}

void showStrip() {
  if (!core->isTurnOn()) {
    return;
  }

  core->getStrip()->show();
}

void setPixel(int pixel, byte r, byte g, byte b, byte w, bool applyBrightness) {
  if (!core->isTurnOn()) {
    return;
  }

  if (applyBrightness) {
    r = map(r, 0, 255, 0, core->getBrightness());
    g = map(g, 0, 255, 0, core->getBrightness());
    b = map(b, 0, 255, 0, core->getBrightness());
    w = map(w, 0, 255, 0, core->getBrightness());
  }

  core->getStrip()->setPixelColor(pixel, r, g, b, w);
}

void setPixel(int pixel, RGBW color, bool applyBrightness) {
  setPixel(pixel, color.red, color.green, color.blue, color.white, applyBrightness);
}

void setAll(byte r, byte g, byte b, byte w, bool refreshStrip = true) {
  if (!core->isTurnOn()) {
    return;
  }

  for (int i = 0; i <= ledCount; i++) {
    setPixel(i, r, g, b, w, false);
  }

  if (refreshStrip) {
    showStrip();
  }
}

void setAll(RGBW rgbw, bool refreshStrip = true) {
  setAll(rgbw.red, rgbw.green, rgbw.blue, rgbw.white, refreshStrip);
}

// BouncingBalls(3);
void BouncingBalls(int BallCount) {
  float Gravity = -9.81;
  int StartHeight = 1;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt(-2 * Gravity * StartHeight);
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int Position[BallCount];
  long ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (int i = 0; i < BallCount; i++) {
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i) / pow(BallCount, 2);
  }

  while (true) {
    if (shouldAbortEffect()) {
      return;
    }
    for (int i = 0; i < BallCount; i++) {
      TimeSinceLastBounce[i] = millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow(TimeSinceLastBounce[i] / 1000, 2.0) + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

      if (Height[i] < 0) {
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();

        if (ImpactVelocity[i] < 0.01) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round(Height[i] * (ledCount - 1) / StartHeight);
    }

    for (int i = 0; i < BallCount; i++) {
      setPixel(Position[i], core->getColor(), false);
    }

    showStrip();
    setAll(BLACK, false);
  }
}

void Lightning(int SpeedDelay) {
  setAll(BLACK, false);
  int ledstart = random(ledCount);           // Determine starting location of flash
  int ledlen = random(ledCount - ledstart);  // Determine length of flash (not to go beyond ledCount-1)
  for (int flashCounter = 0; flashCounter < random(1, 4); flashCounter++) {
    int dimmer = random(10, core->getBrightness());  // return strokes are brighter than the leader
    RGBW lightningColor = {};
    lightningColor = mapColor(core->getColor(), dimmer);
    //    int rr = map(red, 0, 255, 0, dimmer);
    //    int gg = map(green, 0, 255, 0, dimmer);
    //    int bb = map(blue, 0, 255, 0, dimmer);
    //    int ww = map(white, 0, 255, 0, dimmer);

    for (int i = ledstart; i < (ledstart + ledlen); i++) {
      setPixel(i, lightningColor, false);
    }
    showStrip();           // Show a section of LED's
    delay(random(4, 15));  // each flash only lasts 4-10 milliseconds
    for (int i = ledstart; i < (ledstart + ledlen); i++) {
      setPixel(i, BLACK, false);
    }
    showStrip();
    //if (flashCounter == 0) delay (130);   // longer delay until next flash after the leader
    delay(50 + random(100));  // shorter delay between strokes
  }
  delay(random(SpeedDelay) * 50);  // delay between strikes
}

#endif
///**************************** END EFFECTS *****************************************/
