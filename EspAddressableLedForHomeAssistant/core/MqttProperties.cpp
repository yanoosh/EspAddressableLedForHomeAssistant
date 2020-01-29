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