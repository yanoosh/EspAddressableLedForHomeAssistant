#ifndef _MQTT_PROPERTIES_HPP
#define _MQTT_PROPERTIES_HPP

#include <Arduino.h>

class MqttProperties {
 public:
  void enableStatusExtended();

  void disableStatusExtended();

  bool isStatusExtended();

  void setStateInterval(uint32_t stateInterval);

  uint32_t getStateInterval();

 private:
  bool statusExtended = false;
  uint32_t stateInterval = 3600000;
};

#endif