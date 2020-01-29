#ifndef _MQTT_INO
#define _MQTT_INO

#include <ArduinoJson.h>

PubSubClient mqtt(espClient);

/********************************** START SEND STATE*****************************************/
/*
  SAMPLE PAYLOAD:
  {
    "brightness": 120,
    "color": {
      "r": 255,
      "g": 100,
      "b": 100
    },
    "flash": 2,
    "transition": 5,
    "state": "ON"
  }
*/
void sendState() {
  StaticJsonDocument<JSON_BUFFER_SIZE> document;

  document["state"] = (core->isTurnOn()) ? on_cmd : off_cmd;
  JsonObject color = document.createNestedObject("color");
  color["r"] = core->getColor().red;
  color["g"] = core->getColor().green;
  color["b"] = core->getColor().blue;

  document["white_value"] = core->getColor().white;
  document["brightness"] = core->getBrightness();
  document["transition"] = core->getTransitionInterval();
  // todo change after move all effect to class
  document["effect"] = core->getEffectName();

  document["ip"] = WiFi.localIP().toString();
  document["rssi"] = WiFi.RSSI();
  
  if (core->mqtt->isStatusExtended()) {
    document["id"] = ESP.getChipId();
    document["mac"] = WiFi.macAddress();
    document["memory_heap"] = ESP.getFreeHeap();
    document["work_time"] = millis();
  }

  char buffer[measureJson(document) + 1];
  // document.printTo(buffer, sizeof(buffer));
  serializeJson(document, buffer, sizeof(buffer));

  char combinedArray[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(core->getDeviceName())];
  sprintf(combinedArray, "%s%s", MQTT_STATE_TOPIC_PREFIX, core->getDeviceName());  // with word space
  if (!mqtt.publish(combinedArray, buffer, true)) {
    _DPLN("Failed to publish to MQTT. Check you updated your MQTT_MAX_PACKET_SIZE")
  }
}

/********************************** START PROCESS JSON*****************************************/
bool processJson(char* message) {
  StaticJsonDocument<JSON_BUFFER_SIZE> document;

  DeserializationError err = deserializeJson(document, message);

  if (err != DeserializationError::Ok) {
    _DPLN("parseObject() failed")
    return false;
  }

  if (document.containsKey("state")) {
    if (strcmp(document["state"], on_cmd) == 0) {
      newStateOn = true;
    } else if (strcmp(document["state"], off_cmd) == 0) {
      newStateOn = false;
    } else {
      sendState();
      return false;
    }
  }

  if (document.containsKey("transition")) {
    core->setTransitionInterval(document["transition"]);
  }

  if (document.containsKey("color")) {
    Color tmp = core->getColor();
    tmp.red = document["color"]["r"];
    tmp.green = document["color"]["g"];
    tmp.blue = document["color"]["b"];
    core->setColor(tmp);
  }

  // To prevent our power supply from having a cow. Only RGB OR White
  if (document.containsKey("white_value")) {
    Color tmp = core->getColor();
    tmp.white = document["color"]["w"];
    core->setColor(tmp);
  }

  if (document.containsKey("brightness")) {
    core->setBrightness(document["brightness"]);
  }

  if (document.containsKey("effect")) {
    updateEffectByName(document["effect"]);
  }

  return true;
}

void callback(char* topic, byte* payload, unsigned int length) {
  _DPLN()
  _DP("Message arrived [")
  _DP(topic)
  _DP("] ")

  char message[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  _DPLN(message)

  if (!processJson(message)) {
    return;
  }

  if (core->isTurnOn() != newStateOn) {
    if (newStateOn) {
      setOn();
    } else {
      setOff();  // NOTE: Will change transitionDone
    }
  }

  sendState();
}

/********************************** START RECONNECT *****************************************/
void reconnect(unsigned long now) {
  // Loop until we're reconnected
  static unsigned long lastTry = -6000;
  if (now - lastTry > 5000) {
    lastTry = now;
    _DP("Attempting MQTT connection...")

    char mqttAvailTopic[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(core->getDeviceName()) + sizeof(MQTT_AVAIL_TOPIC)];
    sprintf(mqttAvailTopic, "%s%s%s", MQTT_STATE_TOPIC_PREFIX, core->getDeviceName(), MQTT_AVAIL_TOPIC);  // with word space

    // Attempt to connect
    core->getDiode()->progress(Core::BLUE);
    if (mqtt.connect(core->getDeviceName(), MQTT_USER, MQTT_PASSWORD, mqttAvailTopic, 0, true, lwtMessage)) {
      core->getDiode()->done();
      _DPLN("connected")

      // Publish the birth message on connect/reconnect
      mqtt.publish(mqttAvailTopic, birthMessage, true);

      char combinedArray[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(core->getDeviceName()) + 4];
      sprintf(combinedArray, "%s%s/set", MQTT_STATE_TOPIC_PREFIX, core->getDeviceName());  // with word space
      mqtt.subscribe(combinedArray);

      setOff();
      sendState();
    } else {
      _DP("failed, rc=")
      _DP(mqtt.state())
      _DPLN(" try again in 5 seconds")
    }
  }
}
void mqttSetup() {
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
}

void mqttLoop(unsigned long now) {
  if (!mqtt.connected()) {
    reconnect(now);
  }
  mqtt.loop();  // Check MQTT

  static unsigned long lastSendState = 0;
  if (now - lastSendState > core->mqtt->getStateInterval()) {
    sendState();
    lastSendState = now;
  }
}

#endif