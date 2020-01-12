#ifndef _EFFECT_PROCESSOR_CPP
#define _EFFECT_PROCESSOR_CPP

#include <Arduino.h>

class EffectProcessor {
 public:
  virtual const char *getName();
  virtual void begin();
  virtual void loop();
  virtual void end();
};

#endif