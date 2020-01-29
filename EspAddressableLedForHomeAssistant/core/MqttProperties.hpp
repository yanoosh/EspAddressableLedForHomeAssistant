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

  void generateTopics(const char *deviceName);

  const char *getPowerOn();

  const char *getPowerOff();

  const char *getAvailableOnline();

  const char *getAvailableOffline();

  const char *getTopicStatus();

  const char *getTopicAvailable();

 private:
  bool statusExtended = false;
  uint32_t stateInterval = 3600000;
  const char *topicStatus = NULL;
  const char *topicAvailable = NULL;
  const char *topicPrefix = "led";
  const char *topicAvailableSuffix = "/availability";
  const char *topicStatusSuffix = "";

  const char *availableOnline = "online";
  const char *availableOffline = "offline";

  const char *powerOn = "ON";
  const char *powerOff = "OFF";
};

#endif