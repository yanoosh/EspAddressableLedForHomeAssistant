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
#ifdef _DEBUG
#define _DP(message) Serial.print(message);
#define _DPLN(message) Serial.println(message);
#else
#define _DP(message)
#define _DPLN(message)
#endif

#define VERSION F("v3.4 - LedController - https://github.com/DotNetDann - http://dotnetdan.info")

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>  //Not beta version. Tested with v5.3.14
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include "auth.h"
#include "settings.h"

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

/******************************** OTHER GLOBALS *******************************/
Setting setting;
const char* on_cmd = "ON";
const char* off_cmd = "OFF";
const char* effectString = "solid";
String previousEffect = "solid";
String effect = "solid";
bool newStateOn = true;
bool transitionDone = true;
bool transitionAbort = false;
int transitionTime = 150;  // 1-150
int pixelLen = 1;
int pixelArray[50];

// Previous requested values
byte previousRed = 0;
byte previousGreen = 0;
byte previousBlue = 0;
byte previousWhite = 0;

void setOn();
void setOff();

#include "effect.h"
#include "wifi.h"
#include "mqtt.h"
#include "neoPixelEffects.h"
#include "ota.h"
#include "settings.h"
#include "web.h"

/********************************** START SETUP*****************************************/
void setup() {
  //Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT_MAXIMUM, DATA_PIN_LEDS, NEO_GRBW + NEO_KHZ800);
  setting.strip = new Adafruit_NeoPixel(LED_COUNT_MAXIMUM, DATA_PIN_LEDS, NEO_RGB + NEO_KHZ400);
  pinMode(LED_BUILTIN, OUTPUT);       // Initialize the LED_BUILTIN pin as an output (So it doesnt float as a LED is on this pin)
  digitalWrite(LED_BUILTIN, LOW);     // Turn the status LED on
  pinMode(DATA_PIN_RELAY, OUTPUT);    // Initialize the P-Channel MOSFET for the LED strip
  digitalWrite(DATA_PIN_RELAY, LOW);  // Turn the LED strip on

#ifdef _DEBUG
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
#endif

  delay(1000);  // Wait for Leds to init and Cap to charge
  setup_config();

  // End of trinket special code
  //TODO why it isnt use as brightness settings?
  setting.strip->setBrightness(maxBrightness);
  setting.strip->begin();
  setting.strip->show();  // Initialize all pixels to 'off'

  // Standalone startup sequence - Wipe White
  for (uint16_t i = 0; i <= ledCount; i++) {
    setPixel(i, 0, 0, 0, 255, false);
    showStrip();
    delay(1);  // Need delay to be like a yield so it will not restatrt
  }

  wifiSetup();

  // OK we are on Wifi so we are no standalone.
  setPixel(0, 255, 0, 0, 255, false);  // Red tinge on first Pixel
  showStrip();

  mqttSetup();
  webSetup();
  otaSetup();

  _DPLN("Ready");

  // OK we are connected
  setPixel(0, 0, 255, 0, 255, false);  // Green tinge on first Pixel
  showStrip();
  delay(500);                       // Wait so we can see the green before clearing
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the status LED off
}

/********************************** START LED POWER STATE *****************************************/
void setOff() {
  setAll(BLACK);
  setting.setTurnOn(false);
  transitionDone = true;   // Ensure we dont run the loop
  transitionAbort = true;  // Ensure we about any current effect
  previousRed = 0;
  previousGreen = 0;
  previousBlue = 0;
  previousWhite = 0;

  if (!digitalRead(DATA_PIN_RELAY)) {
    delay(200);                          // Wait for sequence to complete and stable
    digitalWrite(DATA_PIN_RELAY, HIGH);  // Do NOT write to strip while it has no power. (https://forums.adafruit.com/viewtopic.php?f=47&t=100265)
    _DPLN("LED: OFF");
  }

  // NOTE: Should really set the xxx pin to be an input to ensure that data is not sent and to stop potential current flow.
  //Writing to pin in INPUT/High-impedance mode enables/disables the internal pullup resistors. But the high impedance ensures that any current flow through the pin will be negligible.
}

void setOn() {
  if (digitalRead(DATA_PIN_RELAY)) {
    digitalWrite(DATA_PIN_RELAY, LOW);
    delay(50);
    setAll(BLACK);
    delay(500);  // Wait for Leds to init and capasitor to charge??
    _DPLN("LED: ON");
  }

  setting.setTurnOn(true);
}

void loop() {
  unsigned long now = millis();

  wifiLoop();
  mqttLoop(now);
  otaLoop();
  webLoop();

  transitionAbort = false;      // Because we came from the loop and not 1/2 way though a transition
  if (!transitionDone) {        // Once we have completed the transition, No point to keep going though the process
    if (setting.getTurnOn()) {  // if the light is turned on

      //EFFECTS
      if (setting.getEffectProcessor() != NULL) {
        setting.getEffectProcessor()->loop();
        delay(transitionTime);
      } else {
        if (effect == "clear") {
          setAll(BLACK);
          transitionDone = true;
        }
        if (effect == "solid") {
          if (transitionTime <= 1) {
            setAll(setting.getFilteredColor());
            transitionDone = true;
          } else {
            Fade(transitionTime);
          }
        }
        if (effect == "pixel") {
          ShowPixels();
        }
        if (effect == "twinkle") {
          Twinkle(10, (2 * transitionTime), false);
        }
        if (effect == "cylon bounce") {
          CylonBounce(4, transitionTime / 10, 50);
        }
        if (effect == "fire") {
          Fire(55, 120, (2 * transitionTime / 2));
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
        if (effect == "color wipe") {
          colorWipe(transitionTime / 20);
        }
        if (effect == "running lights") {
          RunningLights(transitionTime);
        }
        if (effect == "snow sparkle") {
          SnowSparkle(20, random(transitionTime, (10 * transitionTime)));
        }
        if (effect == "sparkle") {
          Sparkle(transitionTime);
        }
        if (effect == "twinkle random") {
          TwinkleRandom(20, (2 * transitionTime), false);
        }
        if (effect == "bouncing balls") {
          BouncingBalls(3);
        }
        if (effect == "lightning") {
          Lightning(transitionTime);
        }
        if (effect == "meteor rain") {
          meteorRain(ledCount / 5, maxBrightness / 4, true, transitionTime);
        }

        // Run once notification effects
        // Reverts color and effect after run
        if (effect == "color wipe once") {
          colorWipeOnce(transitionTime);

          if (effect != "color wipe once") {
            effect = previousEffect;
          }

          if (setting.getFilteredColor().red == 0 && setting.getFilteredColor().green == 0 && setting.getFilteredColor().blue == 0 && setting.getFilteredColor().white == 0) {
            setOff();
          } else {
            transitionDone = false;  // Run the old effect again
          }
          sendState();
        }
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
      setAll(BLACK);
      transitionDone = true;
    }
  } else {
    delay(600);  // Save some power? (from 0.9w to 0.4w when off with ESP8266)
  }
}
