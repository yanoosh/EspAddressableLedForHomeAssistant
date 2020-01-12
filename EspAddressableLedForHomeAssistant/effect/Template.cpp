#ifndef _TEMPLATE_CPP
#define _TEMPLATE_CPP

#include "EffectProcessor.cpp"

class Template : public EffectProcessor {
 public:
  const char *getName(){
    return "";
  }
  void loop(){}
  bool isFinished(){
    return true;
  }
};

#endif