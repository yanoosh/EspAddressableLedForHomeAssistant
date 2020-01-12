#ifndef _EFFECT_INO
#define _EFFECT_INO

byte effectLength = 1;
String effects[] = {
  "rainbow cycle"
};

byte getEffectId(char* name) {
  for (byte i = 0; i < effectLength; i++) {
    if (effects[i] == name)  {
      return i;
    }
  }
  return NULL;
}

String getEffectName(byte id)  {
  if (id < effectLength) {
    return effects[id];
  }
  return "";
}

#endif