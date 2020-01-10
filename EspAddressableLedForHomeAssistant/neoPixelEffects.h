#ifndef _NEOPIXEL_EFFECTS
#define _NEOPIXEL_EFFECTS
#include "effect/RainbowCycle.cpp"

// Previous requested values
byte previousRed = 0;
byte previousGreen = 0;
byte previousBlue = 0;
byte previousWhite = 0;

///**************************** START EFFECTS *****************************************/
// Effects from: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/

bool shouldAbortEffect() {
  yield();
  ESP.wdtFeed();
  client.loop(); // Update from MQTT
  return transitionAbort;
}

void showStrip() {
  if (!setting.getTurnOn()) {
    return;
  }

  strip.show();
}

void setPixel(int pixel, byte r, byte g, byte b, byte w, bool applyBrightness) {
  if (!setting.getTurnOn()) {
    return;
  }

  if (applyBrightness) {
    r = map(r, 0, 255, 0, setting.getBrightness());
    g = map(g, 0, 255, 0, setting.getBrightness());
    b = map(b, 0, 255, 0, setting.getBrightness());
    w = map(w, 0, 255, 0, setting.getBrightness());
  }

  strip.setPixelColor(pixel, r, g, b, w);
}

void setPixel(int pixel, RGBW color, bool applyBrightness) {
  setPixel(pixel, color.red, color.green, color.blue, color.white, applyBrightness);
}

void setAll(byte r, byte g, byte b, byte w, bool refreshStrip = true) {
  if (!setting.getTurnOn()) {
    return;
  }

  for (int i = 0; i <= ledCount; i++ ) {
    setPixel(i, r, g, b, w, false);
  }

  if (refreshStrip) {
    showStrip();
  }
}

void setAll(RGBW rgbw, bool refreshStrip = true) {
  setAll(rgbw.red, rgbw.green, rgbw.blue, rgbw.white, refreshStrip);
}

// Twinkle(10, 100, false);
void Twinkle(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(BLACK, false);

  for (int i = 0; i < Count; i++) {
    if (shouldAbortEffect()) {
      return;
    }
    setPixel(random(ledCount), setting.getFilteredColor(), false);
    showStrip();
    delay(SpeedDelay);
    if (OnlyOne) {
      setAll(BLACK);
    }
  }

  delay(SpeedDelay);
}

// CylonBounce(4, 10, 50);
void CylonBounce(int EyeSize, int SpeedDelay, int ReturnDelay) {
  RGBW paleColor = {setting.getFilteredColor().red / 10, setting.getFilteredColor().green / 10, setting.getFilteredColor().blue / 10, setting.getFilteredColor().white / 10};

  for (int i = 0; i <= (ledCount - EyeSize - 2); i++) {
    if (shouldAbortEffect()) {
      return;
    }
    setAll(BLACK, false);
    setPixel(i, paleColor, false);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, setting.getFilteredColor(), false);
    }
    setPixel(i + EyeSize + 1, paleColor, false);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for (int i = (ledCount - EyeSize - 2); i > 0; i--) {
    if (shouldAbortEffect()) {
      return;
    }
    setAll(0, 0, 0, 0, false);
    setPixel(i, paleColor, false);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(i + j, setting.getFilteredColor(), false);
    }
    setPixel(i + EyeSize + 1, paleColor, false);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    setPixel(Pixel, 255, 255, heatramp, 0, true);
  } else if ( t192 > 0x40 ) {            // middle
    setPixel(Pixel, 255, heatramp, 0, 0, true);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0, 0, true);
  }
}
// Fire(55,120,15);
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[LED_COUNT_MAXIMUM];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i <= ledCount; i++) {
    cooldown = random(0, ((Cooling * 10) / ledCount) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = ledCount - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j <= ledCount; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

// FadeInOut();
void FadeInOut() {
  float r, g, b, w;

  for (int k = 0; k < 256; k = k + 1) {
    if (shouldAbortEffect()) {
      return;
    }
    r = (k / 256.0) * setting.getFilteredColor().red;
    g = (k / 256.0) * setting.getFilteredColor().green;
    b = (k / 256.0) * setting.getFilteredColor().blue;
    w = (k / 256.0) * setting.getFilteredColor().white;
    setAll(r, g, b, w);
    showStrip();
  }

  for (int k = 255; k >= 0; k = k - 2) {
    if (shouldAbortEffect()) {
      return;
    }
    r = (k / 256.0) * setting.getFilteredColor().red;
    g = (k / 256.0) * setting.getFilteredColor().green;
    b = (k / 256.0) * setting.getFilteredColor().blue;
    w = (k / 256.0) * setting.getFilteredColor().white;
    setAll(r, g, b, w);
    showStrip();
  }
}

// Slower:
// Strobe(10, 100);
// Fast:
// Strobe(10, 50);
void Strobe(int StrobeCount, int FlashDelay) {
  for (int j = 0; j < StrobeCount; j++) {
    if (shouldAbortEffect()) {
      return;
    }
    setAll(setting.getFilteredColor());
    showStrip();
    delay(FlashDelay);
    setAll(BLACK);
    delay(FlashDelay);
  }
}

// theaterChase(50);
void theaterChase(int SpeedDelay) {
  for (int q = 0; q < 3; q++) {
    if (shouldAbortEffect()) {
      return;
    }
    for (int i = 0; i <= ledCount; i = i + 3) {
      setPixel(i + q, setting.getFilteredColor(), false);  //turn every third pixel on
    }
    showStrip();

    delay(SpeedDelay);

    for (int i = 0; i <= ledCount; i = i + 3) {
      setPixel(i + q, 0, 0, 0, 0, false);  //turn every third pixel off
    }
  }
}

//  rainbowCycle(20);
RainbowCycle rainbowCycle = RainbowCycle(&strip);

void rainbowCycleRunner(int SpeedDelay) {
  rainbowCycle.loop();
  delay(SpeedDelay);
}

//  colorWipe(50);
void colorWipe(int SpeedDelay) {
  for (uint16_t i = 0; i <= ledCount; i++) {
    if (shouldAbortEffect()) {
      return;
    }
    setPixel(i, setting.getFilteredColor(), false);
    showStrip();
    delay(SpeedDelay);
  }
  transitionDone = true;
}

//  colorWipeOnce(50);
void colorWipeOnce(int SpeedDelay) {
  colorWipe(SpeedDelay);

  // Reset back to previous color
  RGBW color = {previousRed, previousGreen, previousBlue, previousWhite};
  setting.setFilteredColor(color);

  colorWipe(SpeedDelay);
}

//  RunningLights(50);
void RunningLights(int WaveDelay) {
  int Position = 0;

  for (int i = 0; i <= ledCount; i++)
  {
    if (shouldAbortEffect()) {
      return;
    }
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i <= ledCount; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0,false);
      //float level = sin(i+Position) * 127 + 128;
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*setting.getFilteredColor().red,
               ((sin(i + Position) * 127 + 128) / 255)*setting.getFilteredColor().green,
               ((sin(i + Position) * 127 + 128) / 255)*setting.getFilteredColor().blue,
               ((sin(i + Position) * 127 + 128) / 255)*setting.getFilteredColor().white,
               false);
    }

    showStrip();
    delay(WaveDelay);
  }
}

//  SnowSparkle(20, random(100,1000));
void SnowSparkle(int SparkleDelay, int SpeedDelay) {
  setAll(setting.getFilteredColor());

  int Pixel = random(ledCount);
  setPixel(Pixel, 0, 0, 0, 255, false); //@todo why change all to white?
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel, setting.getFilteredColor(), false);
  showStrip();
  delay(SpeedDelay);
}

//  Sparkle(0);
void Sparkle(int SpeedDelay) {
  setAll(BLACK, false);
  int Pixel = random(ledCount);
  setPixel(Pixel, setting.getFilteredColor(), false);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel, BLACK, false);
}

//  TwinkleRandom(20, 100, false);
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(BLACK, false);

  for (int i = 0; i < Count; i++) {
    if (shouldAbortEffect()) {
      return;
    }
    setPixel(random(ledCount), random(0, 255), random(0, 255), random(0, 255), 0, true);
    showStrip();
    delay(SpeedDelay);
    if (OnlyOne) {
      setAll(BLACK);
    }
  }

  delay(SpeedDelay);
}


// BouncingBalls(3);
void BouncingBalls(int BallCount) {
  float Gravity = -9.81;
  int StartHeight = 1;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (int i = 0 ; i < BallCount ; i++) {
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
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i] / 1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

      if ( Height[i] < 0 ) {
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();

        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (ledCount - 1) / StartHeight);
    }

    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i], setting.getFilteredColor(), false);
    }

    showStrip();
    setAll(BLACK, false);
  }
}


/**************************** START TRANSITION FADER *****************************************/
// From https://www.arduino.cc/en/Tutorial/ColorCrossfader
/* BELOW THIS LINE IS THE MATH -- YOU SHOULDN'T NEED TO CHANGE THIS FOR THE BASICS
  The program works like this:
  Imagine a crossfade that moves the red LED from 0-10,
    the green from 0-5, and the blue from 10 to 7, in
    ten steps.
    We'd want to count the 10 steps and increase or
    decrease color values in evenly stepped increments.
    Imagine a + indicates raising a value by 1, and a -
    equals lowering it. Our 10 step fade would look like:
    1 2 3 4 5 6 7 8 9 10
  R + + + + + + + + + +
  G   +   +   +   +   +
  B     -     -     -
  The red rises from 0 to 10 in ten steps, the green from
  0-5 in 5 steps, and the blue falls from 10 to 7 in three steps.
  In the real program, the color percentages are converted to
  0-255 values, and there are 1020 steps (255*4).
  To figure out how big a step there should be between one up- or
  down-tick of one of the LED values, we call calculateStep(),
  which calculates the absolute gap between the start and end values,
  and then divides that gap by 1020 to determine the size of the step
  between adjustments in the value.
*/
int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue;  // What's the overall gap?
  if (step) {                       // If its non-zero,
    step = 1020 / step;            //   divide by 1020
  }

  return step;
}
/* The next function is calculateVal. When the loop value, i,
   reaches the step size appropriate for one of the
   colors, it increases or decreases the value of that color by 1.
   (R, G, and B are each calculated separately.)
*/
int calculateVal(int step, int val, int i) {
  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;
    }
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    }
  }

  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  }
  else if (val < 0) {
    val = 0;
  }

  return val;
}
// Fade(50);
void Fade(int SpeedDelay) {
  int redVal = previousRed;
  int grnVal = previousGreen;
  int bluVal = previousBlue;
  int whiVal = previousWhite;
  int stepR = calculateStep(redVal, setting.getFilteredColor().red);
  int stepG = calculateStep(grnVal, setting.getFilteredColor().green);
  int stepB = calculateStep(bluVal, setting.getFilteredColor().blue);
  int stepW = calculateStep(whiVal, setting.getFilteredColor().white);

  // If no change then exit
  if (stepR == 0 && stepG == 0 && stepB == 0 && stepW == 0) {
    setAll(redVal, grnVal, bluVal, whiVal); // Write current values to LED pins
    transitionDone = true;
    return;
  }

  for (int i = 0; i < 1020; i++) {
    if (shouldAbortEffect()) {
      return;
    }

    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);
    whiVal = calculateVal(stepW, whiVal, i);

    if (i % 50 == 0) {
      setAll(redVal, grnVal, bluVal, whiVal); // Write current values to LED pins
      delay(SpeedDelay);
    }
  }

  setAll(redVal, grnVal, bluVal, whiVal); // Write current values to LED pins
  transitionDone = true;
}

void Lightning(int SpeedDelay) {
  setAll(BLACK, false);
  int ledstart = random(ledCount);           // Determine starting location of flash
  int ledlen = random(ledCount - ledstart);  // Determine length of flash (not to go beyond ledCount-1)
  for (int flashCounter = 0; flashCounter < random(1, 4); flashCounter++) {
    int dimmer = random(10, setting.getBrightness());          // return strokes are brighter than the leader
    RGBW lightningColor = {};
    lightningColor = mapColor(setting.getFilteredColor(), dimmer);
    //    int rr = map(red, 0, 255, 0, dimmer);
    //    int gg = map(green, 0, 255, 0, dimmer);
    //    int bb = map(blue, 0, 255, 0, dimmer);
    //    int ww = map(white, 0, 255, 0, dimmer);

    for (int i = ledstart ; i < (ledstart + ledlen) ; i++) {
      setPixel(i, lightningColor, false);
    }
    showStrip();    // Show a section of LED's
    delay(random(4, 15));                // each flash only lasts 4-10 milliseconds
    for (int i = ledstart ; i < (ledstart + ledlen) ; i++) {
      setPixel(i, BLACK, false);
    }
    showStrip();
    //if (flashCounter == 0) delay (130);   // longer delay until next flash after the leader
    delay(50 + random(100));             // shorter delay between strokes
  }
  delay(random(SpeedDelay) * 50);        // delay between strikes
}

void ShowPixels() {
  // If there are only 2 items in the array then we are setting from and to
  if (pixelLen == 2) {
    // Make sure smallest one first, less than max pixel
    //Serial.println(F("ShowPixels-Range"));

    int startL = pixelArray[0];
    int endL = pixelArray[1];
    if (startL > ledCount) {
      startL = ledCount;
    }
    if (endL > ledCount) {
      endL = ledCount;
    }
    if (startL > endL) {
      startL = 0;
    }

    for (int i = startL; i < endL; i++) {
      setPixel(i, setting.getFilteredColor(), true);
    }

  } else {
    // For each item in array
    //Serial.println(F("ShowPixels-Array"));

    for (int i = 0; i < pixelLen; i++) {
      int pixel = pixelArray[i];
      if (pixel > ledCount) {
        pixel = ledCount;
      }
      setPixel(pixel, setting.getFilteredColor(), true);
    }
  }

  showStrip();
  transitionDone = true;
}
// Meteor rain
// meteorRain(10, 64, true, 30);
void _meteorRainFadeToBlack(int ledNo, byte fadeValue) {
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, r, g, b);
}

void meteorRain(byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {

  for (int i = 0; i < ledCount + ledCount; i++) {
    if (shouldAbortEffect()) {
      return;
    }

    // fade brightness all LEDs one step
    for (int j = 0; j < ledCount; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        _meteorRainFadeToBlack(j, meteorTrailDecay );
      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < ledCount) && (i - j >= 0) ) {
        setPixel(i - j, setting.getFilteredColor().red, setting.getFilteredColor().green, setting.getFilteredColor().blue, 0, true);
      }
    }

    showStrip();
    delay(SpeedDelay);
  }
}

#endif
///**************************** END EFFECTS *****************************************/
