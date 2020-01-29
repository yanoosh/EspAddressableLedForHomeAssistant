#include "MqttProperties.hpp"

void MqttProperties::enableStatusExtended() {
  this->statusExtended = true;
}

void MqttProperties::disableStatusExtended() {
  this->statusExtended = false;
}

bool MqttProperties::isStatusExtended() {
  return this->statusExtended;
}

void MqttProperties::setStateInterval(uint32_t stateInterval) {
  this->stateInterval = stateInterval;
}

uint32_t MqttProperties::getStateInterval() {
  return this->stateInterval;
}

void MqttProperties::generateTopics(const char *deviceName) {
  delete[] this->topicAvailable;
  uint8_t size = strlen(this->topicPrefix) + 1 + strlen(deviceName) + strlen(this->topicAvailableSuffix) + 1;
  this->topicAvailable = new char[size];
  sprintf((char *)this->topicAvailable, "%s/%s%s", this->topicPrefix, deviceName, this->topicAvailableSuffix);

  delete[] this->topicStatus;
  size = strlen(this->topicPrefix) + 1 + strlen(deviceName) + strlen(this->topicStatusSuffix) + 1;
  this->topicStatus = new char[size];
  sprintf((char *)this->topicStatus, "%s/%s%s", this->topicPrefix, deviceName, this->topicStatusSuffix);
}

const char *MqttProperties::getPowerOn() {
  return this->powerOn;
}

const char *MqttProperties::getPowerOff() {
  return this->powerOff;
}

const char *MqttProperties::getAvailableOnline() {
  return this->availableOnline;
}

const char *MqttProperties::getAvailableOffline() {
  return this->availableOffline;
}

const char *MqttProperties::getTopicStatus() {
  return this->topicStatus;
}

const char *MqttProperties::getTopicAvailable() {
  return this->topicAvailable;
}