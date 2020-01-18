#ifndef _MQTT_INO
#define _MQTT_INO

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
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();

  root["state"] = (core->isTurnOn()) ? on_cmd : off_cmd;
  JsonObject& color = root.createNestedObject("color");
  color["r"] = core->getColor().red;
  color["g"] = core->getColor().green;
  color["b"] = core->getColor().blue;

  root["white_value"] = core->getColor().white;
  root["brightness"] = core->getBrightness();
  root["transition"] = transitionTime;
  // todo change after move all effect to class
  root["effect"] = core->getEffectName();

#ifdef _DEBUG
  root["id"] = ESP.getChipId();
  root["memory_heap"] = ESP.getFreeHeap();
  root["work_time"] = millis();
#endif

  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));

  char combinedArray[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(core->getDeviceName())];
  sprintf(combinedArray, "%s%s", MQTT_STATE_TOPIC_PREFIX, core->getDeviceName());  // with word space
  if (!mqtt.publish(combinedArray, buffer, true)) {
    _DPLN("Failed to publish to MQTT. Check you updated your MQTT_MAX_PACKET_SIZE");
  }
}

/********************************** START PROCESS JSON*****************************************/
bool processJson(char* message) {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(message);

  if (!root.success()) {
    _DPLN("parseObject() failed");
    return false;
  }

  if (root.containsKey("state")) {
    if (strcmp(root["state"], on_cmd) == 0) {
      newStateOn = true;
    } else if (strcmp(root["state"], off_cmd) == 0) {
      newStateOn = false;
    } else {
      sendState();
      return false;
    }
  }

  if (root.containsKey("transition")) {
    transitionTime = root["transition"];
  }

  if (root.containsKey("color")) {
    core->setColor({root["color"]["r"], root["color"]["g"], root["color"]["b"], 0});
  }

  // To prevent our power supply from having a cow. Only RGB OR White
  if (root.containsKey("white_value")) {
    core->setColor({0, 0, 0, root["white_value"]});
  }

  if (root.containsKey("brightness")) {
    core->setBrightness(root["brightness"]);
  }

  if (root.containsKey("effect")) {
    updateEffectByName(root["effect"]);
  }

  return true;
}

void callback(char* topic, byte* payload, unsigned int length) {
  _DPLN();
  _DP("Message arrived [");
  _DP(topic);
  _DP("] ");

  char message[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  _DPLN(message);

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
    _DP("Attempting MQTT connection...");

    char mqttAvailTopic[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(core->getDeviceName()) + sizeof(MQTT_AVAIL_TOPIC)];
    sprintf(mqttAvailTopic, "%s%s%s", MQTT_STATE_TOPIC_PREFIX, core->getDeviceName(), MQTT_AVAIL_TOPIC);  // with word space

    // Attempt to connect
    core->getDiode()->progress(Core::BLUE);
    if (mqtt.connect(core->getDeviceName(), MQTT_USER, MQTT_PASSWORD, mqttAvailTopic, 0, true, lwtMessage)) {
      core->getDiode()->done();
      _DPLN("connected");

      // Publish the birth message on connect/reconnect
      mqtt.publish(mqttAvailTopic, birthMessage, true);

      char combinedArray[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(core->getDeviceName()) + 4];
      sprintf(combinedArray, "%s%s/set", MQTT_STATE_TOPIC_PREFIX, core->getDeviceName());  // with word space
      mqtt.subscribe(combinedArray);

      setOff();
      sendState();
    } else {
      _DP("failed, rc=");
      _DP(mqtt.state());
      _DPLN(" try again in 5 seconds");
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

#ifdef _DEBUG_STATE
  static unsigned long lastSendState = 0;
  if (now - lastSendState > _DEBUG_STATE) {
    sendState();
    lastSendState = now;
  }
#endif
}

#endif