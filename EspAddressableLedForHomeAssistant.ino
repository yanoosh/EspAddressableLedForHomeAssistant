/*
  To use this code you will need the following dependancies: 
  
  - Support for the ESP8266 boards. 
        - You can add it to the board manager by going to File -> Preference and pasting http://arduino.esp8266.com/stable/package_esp8266com_index.json into the Additional Board Managers URL field.
        - Next, download the ESP8266 dependancies by going to Tools -> Board -> Board Manager and searching for ESP8266 and installing it.
  
  - You will also need to download the follow libraries by going to Sketch -> Include Libraries -> Manage Libraries
      - Adafruit NeoPixel 
      - PubSubClient
      - ArduinoJSON
*/
// ------------------------------
// ---- all config in auth.h ----
// ------------------------------
#define MQTT_MAX_PACKET_SIZE 512
#define VERSION F("v3.4 - LedController - https://github.com/DotNetDann - http://dotnetdan.info")

#include <ArduinoJson.h> //Not beta version. Tested with v5.3.14
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "settings.h"
#include "auth.h"

// The maximum mqtt message size, including header, is 128 bytes by default.
// You must update your PubSubClient.h file manually.......
#if MQTT_MAX_PACKET_SIZE < 512  // If the max message size is too small, throw an error at compile time. See PubSubClient.cpp line 359
  #error "MQTT_MAX_PACKET_SIZE is too small in libraries/PubSubClient/src/PubSubClient.h at const MQTT_MAX_PACKET_SIZE, increase it from 128 to 512"
#endif

/****************************************FOR JSON***************************************/
const int BUFFER_SIZE = JSON_OBJECT_SIZE(60);

char* birthMessage = "online";
const char* lwtMessage = "offline";

/*********************************** LED Defintions ********************************/
// Real values as requested from the MQTT server


// Values as set to strip
byte brightness = 255;


/******************************** OTHER GLOBALS *******************************/
const char* on_cmd = "ON";
const char* off_cmd = "OFF";
const char* effectString = "solid";
String previousEffect = "solid";
String effect = "solid";
bool stateOn = true;
bool newStateOn = true;
bool transitionDone = true;
bool transitionAbort = false;
int transitionTime = 50; // 1-150
int pixelLen = 1;
int pixelArray[50];

WiFiClient espClient;
PubSubClient client(espClient);
ESP8266WebServer server(80);

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT_MAXIMUM, DATA_PIN_LEDS, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT_MAXIMUM, DATA_PIN_LEDS, NEO_RGB + NEO_KHZ400);
  
#include "neoPixelEffects.h"
#include "web.h"

/********************************** START SETUP*****************************************/
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);       // Initialize the LED_BUILTIN pin as an output (So it doesnt float as a LED is on this pin)
  digitalWrite(LED_BUILTIN, LOW);     // Turn the status LED on
  pinMode(DATA_PIN_RELAY, OUTPUT);    // Initialize the P-Channel MOSFET for the LED strip
  digitalWrite(DATA_PIN_RELAY, LOW);  // Turn the LED strip on

  Serial.begin(115200);
  
  delay(1000); // Wait for Leds to init and Cap to charge
  setup_config();
  
  // End of trinket special code
  strip.setBrightness(maxBrightness);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  // Standalone startup sequence - Wipe White
  for(uint16_t i=0; i<=ledCount; i++) {
    setPixel(i, 0, 0, 0, 255, false);
    showStrip();
    delay(1); // Need delay to be like a yield so it will not restatrt
  }

  setup_wifi();

  // OK we are on Wifi so we are no standalone.
  setPixel(0, 255, 0, 0, 255, false); // Red tinge on first Pixel
  showStrip();
  
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  
  server.on("/", ServeWebClients);
  server.begin();

  //OTA SETUP
  ArduinoOTA.setPort(OTAport);
  ArduinoOTA.setHostname(deviceName); // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setPassword((const char *)OTApassword); // No authentication by default

  ArduinoOTA.onStart([]() {
    Serial.println("Starting");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  Serial.println(F("Ready"));
  
  // OK we are connected
  setPixel(0, 0, 255, 0, 255, false); // Green tinge on first Pixel
  showStrip();
  delay(500); // Wait so we can see the green before clearing
  digitalWrite(LED_BUILTIN, HIGH);     // Turn the status LED off
}


/********************************** START SETUP WIFI *****************************************/
void setup_wifi() {
  int retries = 0;
  delay(100);
  Serial.print(F("Connecting to SSID: "));
  Serial.println(WIFI_SSID);
  
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);  
  WiFi.hostname(deviceName);

  if (WiFi.status() != WL_CONNECTED) {  // FIX FOR USING 2.3.0 CORE (only .begin if not connected)
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
    if (retries++ > 20) {
      ESP.restart();
    }
  }

  Serial.println(F(""));
  Serial.println(F("WiFi connected"));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
}

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


/********************************** START LED POWER STATE *****************************************/
void setOff() {
  setAll(&colorBlack);
  stateOn = false;
  transitionDone = true; // Ensure we dont run the loop
  transitionAbort = true; // Ensure we about any current effect
  previousRed = 0;
  previousGreen = 0;
  previousBlue = 0;
  previousWhite = 0;

  if (!digitalRead(DATA_PIN_RELAY)) { 
    delay(200); // Wait for sequence to complete and stable
    digitalWrite(DATA_PIN_RELAY, HIGH); // Do NOT write to strip while it has no power. (https://forums.adafruit.com/viewtopic.php?f=47&t=100265)
    Serial.println("LED: OFF");
  }
  
  // NOTE: Should really set the xxx pin to be an input to ensure that data is not sent and to stop potential current flow.
  //Writing to pin in INPUT/High-impedance mode enables/disables the internal pullup resistors. But the high impedance ensures that any current flow through the pin will be negligible.
}

void setOn() {
  if (digitalRead(DATA_PIN_RELAY)) {
    digitalWrite(DATA_PIN_RELAY, LOW);
    delay(50);
    setAll(&colorBlack);
    delay(500); // Wait for Leds to init and capasitor to charge??
    Serial.println("LED: ON");
  }
  
  stateOn = true;  
}


/********************************** START CALLBACK*****************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(F(""));
  Serial.print(F("Message arrived ["));
  Serial.print(topic);
  Serial.print(F("] "));
  
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);

  previousEffect = effect;

  if (!processJson(message)) {
    return;
  }

  previousRed = setting.filteredColor.red;
  previousGreen = setting.filteredColor.green;
  previousBlue = setting.filteredColor.blue;
  previousWhite = setting.filteredColor.white;

  if (stateOn || newStateOn) {
    mapColor(&setting.filteredColor, &setting.sourceColor, brightness);
  } else {
    cpyColor(&setting.filteredColor, &colorBlack);
  }

  transitionAbort = true; // Kill the current effect
  transitionDone = false; // Start a new transition

  if (stateOn != newStateOn) {
    if (newStateOn) {
      setOn();
    } else {
      setOff(); // NOTE: Will change transitionDone
    }
  }

  sendState();
}


/********************************** START PROCESS JSON*****************************************/
bool processJson(char* message) {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(message);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return false;
  }

  if (root.containsKey("state")) {
    if (strcmp(root["state"], on_cmd) == 0) {
      newStateOn = true;
    }
    else if (strcmp(root["state"], off_cmd) == 0) {
      newStateOn = false;
    }
    else { 
      sendState();
      return false;
    }
  }

  if (root.containsKey("transition")) {
    transitionTime = root["transition"];
  }
  
  if (root.containsKey("color")) {
    setting.sourceColor.red = root["color"]["r"];
    setting.sourceColor.green = root["color"]["g"];
    setting.sourceColor.blue = root["color"]["b"];
    setting.sourceColor.white = 0;
  }

  // To prevent our power supply from having a cow. Only RGB OR White
  if (root.containsKey("white_value")) {
    setting.sourceColor.red = 0;
    setting.sourceColor.green = 0;
    setting.sourceColor.blue = 0;
    setting.sourceColor.white = root["white_value"];
  }

  if (root.containsKey("brightness")) {
    brightness = root["brightness"];
  }
  
  if (root.containsKey("pixel")) {
    pixelLen = root["pixel"].size();
    if (pixelLen > sizeof(pixelArray)) {
      pixelLen = sizeof(pixelArray);
    }
    for (int i = 0; i < pixelLen; i++) {
      pixelArray[i]=root["pixel"][i];
    }
  }
  
  if (root.containsKey("effect")) {
    effectString = root["effect"];
    effect = effectString;
  }

  return true;
}


/********************************** START SEND STATE*****************************************/
void sendState() {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();

  root["state"] = (stateOn) ? on_cmd : off_cmd;
  JsonObject& color = root.createNestedObject("color");
  color["r"] = setting.sourceColor.red;
  color["g"] = setting.sourceColor.green;
  color["b"] = setting.sourceColor.blue;

  root["white_value"] = setting.sourceColor.white;
  root["brightness"] = brightness;
  root["transition"] = transitionTime;
  root["effect"] = effect.c_str();
  
  #ifdef DEBUGE_MODE
    root["id"] = ESP.getChipId();
    root["memory_heap"] = ESP.getFreeHeap();
    root["work_time"] = millis();
  #endif
  
  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));
  
  char combinedArray[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(deviceName)];
  sprintf(combinedArray, "%s%s", MQTT_STATE_TOPIC_PREFIX, deviceName); // with word space
  if (!client.publish(combinedArray, buffer, true)) {
    Serial.println(F("Failed to publish to MQTT. Check you updated your MQTT_MAX_PACKET_SIZE"));
  }
}


/********************************** START RECONNECT *****************************************/
void reconnect(unsigned long now) {
  // Loop until we're reconnected
  static unsigned long lastTry = -6000;
  if (now - lastTry > 5000) {
    lastTry = now;
    Serial.print(F("Attempting MQTT connection..."));

    char mqttAvailTopic[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(deviceName) + sizeof(MQTT_AVAIL_TOPIC)];
    sprintf(mqttAvailTopic, "%s%s%s", MQTT_STATE_TOPIC_PREFIX, deviceName, MQTT_AVAIL_TOPIC); // with word space

    // Attempt to connect
    if (client.connect(deviceName, MQTT_USER, MQTT_PASSWORD, mqttAvailTopic, 0, true, lwtMessage)) {
      Serial.println(F("connected"));

      // Publish the birth message on connect/reconnect
      client.publish(mqttAvailTopic, birthMessage, true);      
      
      char combinedArray[sizeof(MQTT_STATE_TOPIC_PREFIX) + sizeof(deviceName) + 4];
      sprintf(combinedArray, "%s%s/set", MQTT_STATE_TOPIC_PREFIX, deviceName); // with word space    
      client.subscribe(combinedArray);
      
      setOff();
      sendState();
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
    }
  }
}


/********************************** START MAIN LOOP *****************************************/
void loop() {
  unsigned long now = millis();
  
  if (!client.connected()) {
    reconnect(now);
  }

  if (WiFi.status() != WL_CONNECTED) {
    delay(1);
    Serial.print(F("WIFI Disconnected. Attempting reconnection."));
    setup_wifi();
    return;
  }

  client.loop(); // Check MQTT

  ArduinoOTA.handle(); // Check OTA Firmware Updates
  
  server.handleClient(); // Check Web page requests

  transitionAbort = false; // Because we came from the loop and not 1/2 way though a transition
  
  if (!transitionDone) {  // Once we have completed the transition, No point to keep going though the process
    if (stateOn) {   // if the light is turned on

      //EFFECTS
      if (effect == "clear") {
        setAll(&colorBlack);
        transitionDone = true;
      }
      if (effect == "solid") {
        if (transitionTime <= 1) {
          setAll(&setting.filteredColor);
          transitionDone = true;
        } else {
          Fade(transitionTime);
        }
      }
      if (effect == "pixel") {
        ShowPixels();
      }
      if (effect == "twinkle") {
        Twinkle(10, (2*transitionTime), false);
      }
      if (effect == "cylon bounce") {
        CylonBounce(4, transitionTime/10, 50);
      }
      if (effect == "fire") {
        Fire(55,120,(2*transitionTime/2));
      }
      if (effect == "fade in out") {
        FadeInOut();
      }
      if (effect == "strobe") {
        Strobe(10, transitionTime);
      }
      if (effect == "theater chase") {
        theaterChase(transitionTime);
      }
      if (effect == "rainbow cycle") {
        rainbowCycle(transitionTime/5);
      }
      if (effect == "color wipe") {
        colorWipe(transitionTime/20);
      }
      if (effect == "running lights") {
        RunningLights(transitionTime);
      }
      if (effect == "snow sparkle") {
        SnowSparkle(20, random(transitionTime,(10*transitionTime)));
      }
      if (effect == "sparkle") {
        Sparkle(transitionTime);
      }
      if (effect == "twinkle random") {
        TwinkleRandom(20, (2*transitionTime), false);
      }
      if (effect == "bouncing balls") {
        BouncingBalls(3);
      }
      if (effect == "lightning") {
        Lightning(transitionTime);
      }
      if (effect == "meteor rain") {
        meteorRain(ledCount / 5, maxBrightness / 4, true , transitionTime);
      }
      
      // Run once notification effects
      // Reverts color and effect after run
      if (effect == "color wipe once") {
        colorWipeOnce(transitionTime);

        if (effect != "color wipe once") {
          effect = previousEffect;
        }
        
        if (setting.filteredColor.red == 0 && setting.filteredColor.green == 0 && setting.filteredColor.blue == 0 && setting.filteredColor.white == 0) {
          setOff();
        } else {
          transitionDone = false; // Run the old effect again
        }
        sendState();
      }
    
//      if (effect == "bpm") {
//      }
//      if (effect == "candy cane") {
//      }
//      if (effect == "confetti" ) {
//      }
//      if (effect == "dots") {
//      }
//      if (effect == "glitter") {
//      }
//      if (effect == "juggle" ) {                           // eight colored dots, weaving in and out of sync with each other
//      }
//      if (effect == "lightning") {
//      }
//      if (effect == "police all") {                 //POLICE LIGHTS (TWO COLOR SOLID)
//      }
//      if (effect == "police one") {
//      }
//      if (effect == "rainbow with glitter") {               // FastLED's built-in rainbow generator with Glitter
//      }
      
    } else {
      setAll(&colorBlack);
      transitionDone = true;
    }
  } else {
	  delay(600); // Save some power? (from 0.9w to 0.4w when off with ESP8266)
  }
  #ifdef DEBUGE_MODE
    static unsigned long lastSendState = 0;
    if (now - lastSendState > DEBUGE_MODE) {
      sendState();
      lastSendState = now;
    }
  #endif
}
