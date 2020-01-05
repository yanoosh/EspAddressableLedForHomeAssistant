#ifndef _NEOPIXEL_EFFECTS
#define _NEOPIXEL_EFFECTS

///**************************** START EFFECTS *****************************************/
// Effects from: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/


bool shouldAbortEffect() {
  yield(); // Watchdog timer
  client.loop(); // Update from MQTT
  return transitionAbort;
}

void showStrip() {
  if (!stateOn) { return; }
  
  strip.show();
}

void setPixel(int pixel, byte r, byte g, byte b, byte w, bool applyBrightness) {
  if (!stateOn) { return; }

  if (applyBrightness) {
    r = map(r, 0, 255, 0, brightness);
    g = map(g, 0, 255, 0, brightness);
    b = map(b, 0, 255, 0, brightness);
    w = map(w, 0, 255, 0, brightness);
  }
  
  #ifndef SUPPORT_RGBW
    strip.setPixelColor(pixel, strip.Color(r, g, b, w));
  #else
    strip.setPixelColor(pixel, strip.Color(r, g, b));
  #endif
}

void setAll(byte r, byte g, byte b, byte w, bool refreshStrip = true) {
  if (!stateOn) { return; }

  for(int i = 0; i <= ledCount; i++ ) {
    setPixel(i, r, g, b, w, false);
  }

  if (refreshStrip) {
    showStrip();
    
    //Serial.print("Setting LEDs - ");
    //Serial.print("r: ");
    //Serial.print(r);
    //Serial.print(", g: ");
    //Serial.print(g);
    //Serial.print(", b: ");
    //Serial.print(b);
    //Serial.print(", w: ");
    //Serial.println(w);
  }
}




// Twinkle(10, 100, false);
void Twinkle(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0,0);
  
  for (int i=0; i<Count; i++) {
    if (shouldAbortEffect()) { return; }
    setPixel(random(ledCount), red, green, blue, white, false);
    showStrip();
    delay(SpeedDelay);
    if(OnlyOne) { 
      setAll(0,0,0,0); 
    }
  }
  
  delay(SpeedDelay);
}

// CylonBounce(4, 10, 50);
void CylonBounce(int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i <= (ledCount-EyeSize-2); i++) {
    if (shouldAbortEffect()) { return; } 
    setAll(0,0,0,0,false);
    setPixel(i, red/10, green/10, blue/10, white/10, false);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue, white, false); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10, white/10, false);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = (ledCount-EyeSize-2); i > 0; i--) {
    if (shouldAbortEffect()) { return; } 
    setAll(0,0,0,0,false);
    setPixel(i, red/10, green/10, blue/10, white/10, false);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue, white, false); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10, white/10, false);
    showStrip();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp, 0, true);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0, 0, true);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0, 0, true);
  }
}
// Fire(55,120,15);
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  byte heat[ledCount];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i <= ledCount; i++) {
    cooldown = random(0, ((Cooling * 10) / ledCount) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k = ledCount - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j <= ledCount; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

// FadeInOut();
void FadeInOut(){
  float r, g, b, w;
      
  for(int k = 0; k < 256; k=k+1) {
    if (shouldAbortEffect()) { return; } 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    w = (k/256.0)*white;
    setAll(r,g,b,w);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    if (shouldAbortEffect()) { return; } 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    w = (k/256.0)*white;
    setAll(r,g,b,w);
    showStrip();
  }
}

// Slower:
// Strobe(10, 100);
// Fast:
// Strobe(10, 50);
void Strobe(int StrobeCount, int FlashDelay){
  for(int j = 0; j < StrobeCount; j++) {
    if (shouldAbortEffect()) { return; } 
    setAll(red, green, blue, white);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0,0);
    showStrip();
    delay(FlashDelay);
  }
}

// theaterChase(50);
void theaterChase(int SpeedDelay) {
  for (int q=0; q < 3; q++) {
    if (shouldAbortEffect()) { return; } 
    for (int i=0; i <= ledCount; i=i+3) {
      setPixel(i+q, red, green, blue, white, false);    //turn every third pixel on
    }
    showStrip();
   
    delay(SpeedDelay);
   
    for (int i=0; i <= ledCount; i=i+3) {
      setPixel(i+q, 0,0,0,0,false);        //turn every third pixel off
    }
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}
//  rainbowCycle(20);
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*2; j++) { // 2 cycles of all colors on wheel
    if (shouldAbortEffect()) { return; } 
    for(i=0; i <= ledCount; i++) {
      c=Wheel(((i * 256 / ledCount) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2), 0, true);
    }
    showStrip();
    delay(SpeedDelay);
  }
}

//  colorWipe(50);
void colorWipe(int SpeedDelay) {
  for(uint16_t i=0; i<=ledCount; i++) {
    if (shouldAbortEffect()) { return; } 
    setPixel(i, red, green, blue, white, false);
    showStrip();
    delay(SpeedDelay);
  }
  transitionDone = true;
}

//  colorWipeOnce(50);
void colorWipeOnce(int SpeedDelay) {
  colorWipe(SpeedDelay);

  // Reset back to previous color
  red = previousRed;
  green = previousGreen;
  blue = previousBlue;
  white = previousWhite;

  colorWipe(SpeedDelay);
}

//  RunningLights(50);
void RunningLights(int WaveDelay) {
  int Position=0;
  
  for(int i=0; i<=ledCount; i++)
  {
    if (shouldAbortEffect()) { return; } 
    Position++; // = 0; //Position + Rate;
    for(int i=0; i<=ledCount; i++) {
      // sine wave, 3 offset waves make a rainbow!
      //float level = sin(i+Position) * 127 + 128;
      //setPixel(i,level,0,0,false);
      //float level = sin(i+Position) * 127 + 128;
      setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue,
                   ((sin(i+Position) * 127 + 128)/255)*white,
				   false);
    }
      
    showStrip();
    delay(WaveDelay);
  }
}

//  SnowSparkle(20, random(100,1000));
void SnowSparkle(int SparkleDelay, int SpeedDelay) {
  setAll(red, green, blue, white);
  
  int Pixel = random(ledCount);
  setPixel(Pixel,0,0,0,255,false);
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel, red, green, blue, white, false);
  showStrip();
  delay(SpeedDelay);
}

//  Sparkle(0);
void Sparkle(int SpeedDelay) {
  setAll(0,0,0,0);
  int Pixel = random(ledCount);
  setPixel(Pixel, red, green, blue, white, false);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0,0,false);
}

//  TwinkleRandom(20, 100, false);
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0,0);
  
  for (int i=0; i<Count; i++) {
    if (shouldAbortEffect()) { return; }     
    setPixel(random(ledCount), random(0,255), random(0,255), random(0,255), 0, true);
    showStrip();
    delay(SpeedDelay);
    if(OnlyOne) { 
      setAll(0,0,0,0); 
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
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2); 
  }

  while (true) {
    if (shouldAbortEffect()) { return; }
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
  
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
      setPixel(Position[i],red,green,blue,white,false);
    }
    
    showStrip();
    setAll(0,0,0,0);
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
    step = 1020/step;              //   divide by 1020
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
void Fade(int SpeedDelay){
  int redVal = previousRed;
  int grnVal = previousGreen;
  int bluVal = previousBlue;
  int whiVal = previousWhite;  
  int stepR = calculateStep(redVal, red);
  int stepG = calculateStep(grnVal, green);
  int stepB = calculateStep(bluVal, blue);
  int stepW = calculateStep(whiVal, white);

  // If no change then exit
  if (stepR == 0 && stepG == 0 && stepB == 0 && stepW == 0){ 
    setAll(redVal, grnVal, bluVal, whiVal); // Write current values to LED pins
    transitionDone = true;
    return;
  }

  for (int i=0; i<1020; i++) {  
    if (shouldAbortEffect()) { return; }     
    
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

void Lightning(int SpeedDelay){
  setAll(0,0,0,0);
  int ledstart = random(ledCount);           // Determine starting location of flash
  int ledlen = random(ledCount - ledstart);  // Determine length of flash (not to go beyond ledCount-1)
  for (int flashCounter = 0; flashCounter < random(1, 4); flashCounter++) {
    int dimmer = random(10, brightness);          // return strokes are brighter than the leader
    int rr = map(red, 0, 255, 0, dimmer);
    int gg = map(green, 0, 255, 0, dimmer);
    int bb = map(blue, 0, 255, 0, dimmer);
    int ww = map(white, 0, 255, 0, dimmer);
    
    for (int i = ledstart ; i < (ledstart + ledlen) ; i++) {
      setPixel(i, rr, gg, bb, ww, false);
    }
    showStrip();    // Show a section of LED's
    delay(random(4, 15));                // each flash only lasts 4-10 milliseconds
    for (int i = ledstart ; i < (ledstart + ledlen) ; i++) {
      setPixel(i, 0, 0, 0, 0, false);
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
    if (startL > ledCount) { startL = ledCount; }
    if (endL > ledCount) { endL = ledCount; }
    if (startL > endL) { startL = 0; }
    
    for (int i = startL; i < endL; i++) {
      setPixel(i, red, green, blue, white, true);
    }    
    
  } else {
    // For each item in array
    //Serial.println(F("ShowPixels-Array"));
    
    for (int i = 0; i < pixelLen; i++) {
      int pixel = pixelArray[i];
      if (pixel > ledCount) { pixel = ledCount; }
      setPixel(pixel, red, green, blue, white, true);
    }
  }
  
  showStrip();
  transitionDone = true;
}

#endif
///**************************** END EFFECTS *****************************************/
