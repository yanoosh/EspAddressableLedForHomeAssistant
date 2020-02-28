#ifndef _MQTT_PROPERTIES_CPP
#define _MQTT_PROPERTIES_CPP

#include <Arduino.h>

class MqttProperties {
 public:
  void enableStatusExtended() {
    this->statusExtended = true;
  }

  void disableStatusExtended() {
    this->statusExtended = false;
  }

  bool isStatusExtended() {
    return this->statusExtended;
  }

  void enableHomeAssitantDiscovery() {
    this->statusHomeAssitantDiscovery = true;
  }

  void disableHomeAssitantDiscovery() {
    this->statusHomeAssitantDiscovery = false;
  }

  bool isHomeAssitantDiscovery() {
    return this->statusHomeAssitantDiscovery;
  }

  void setStateInterval(uint32_t stateInterval) {
    this->stateInterval = stateInterval;
  }

  uint32_t getStateInterval() {
    return this->stateInterval;
  }

  void generateTopics(const char *deviceName) {
    delete[] this->topicAvailable;
    uint8_t size = strlen(this->topicPrefix) + 1 + strlen(deviceName) + strlen(this->topicAvailableSuffix) + 1;
    this->topicAvailable = new char[size];
    sprintf((char *)this->topicAvailable, "%s/%s%s", this->topicPrefix, deviceName, this->topicAvailableSuffix);

    delete[] this->topicStatus;
    size = strlen(this->topicPrefix) + 1 + strlen(deviceName) + strlen(this->topicStatusSuffix) + 1;
    this->topicStatus = new char[size];
    sprintf((char *)this->topicStatus, "%s/%s%s", this->topicPrefix, deviceName, this->topicStatusSuffix);

    delete[] this->topicCommand;
    size = strlen(this->topicPrefix) + 1 + strlen(deviceName) + strlen(this->topicCommandSuffix) + 1;
    this->topicCommand = new char[size];
    sprintf((char *)this->topicCommand, "%s/%s%s", this->topicPrefix, deviceName, this->topicCommandSuffix);

    if (this->statusHomeAssitantDiscovery) {
      delete[] this->topicHomeAssistantConfig;
      size = strlen(this->topicHomeAssitant) + 7 + strlen(deviceName) + 7 + 1;
      this->topicHomeAssistantConfig = new char[size];
      sprintf((char *)this->topicHomeAssistantConfig, "%s/light/%s/config", this->topicHomeAssitant, deviceName);
    }
  }

  const char *getPowerOn() {
    return this->powerOn;
  }

  const char *getPowerOff() {
    return this->powerOff;
  }

  const char *getAvailableOnline() {
    return this->availableOnline;
  }

  const char *getAvailableOffline() {
    return this->availableOffline;
  }

  const char *getTopicPrefix() {
    return this->topicPrefix;
  }

  const char *getTopicStatus() {
    return this->topicStatus;
  }

  const char *getTopicAvailable() {
    return this->topicAvailable;
  }

  const char *getTopicCommand() {
    return this->topicCommand;
  }

  const char *getTopicHomeAssistantConfig() {
    return this->topicHomeAssistantConfig;
  }

 private:
  bool statusExtended = false;
  uint32_t stateInterval = 3600000;
  const char *topicStatus = NULL;
  const char *topicAvailable = NULL;
  const char *topicCommand = NULL;
  const char *topicPrefix = "led";
  const char *topicAvailableSuffix = "/availability";
  const char *topicStatusSuffix = "";
  const char *topicCommandSuffix = "/set";

  const char *availableOnline = "online";
  const char *availableOffline = "offline";

  const char *powerOn = "ON";
  const char *powerOff = "OFF";

  const char *topicHomeAssistantConfig = NULL;
  bool statusHomeAssitantDiscovery = false;
  const char *topicHomeAssitant = "homeassistant";
};

#endif