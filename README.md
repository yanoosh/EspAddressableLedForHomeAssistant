# ESP8266 MQTT JSON SK6812RGBW HomeAssistant

This project shows a super easy way to get started using Digital LED strips with [Home Assistant](https://home-assistant.io/), a sick, open-source Home Automation platform that can do just about anything. 

The code covered in this repository utilizes [Home Assistant's MQTT JSON Light Component](https://home-assistant.io/components/light.mqtt_json/) and an ESP8266 microcontroller. 

This project can run standalone. When turned on the white LEDs will light.  This means you can use the project as a normal light source. If the device can connect to WiFi and MQTT, then you are able to control remotely.


#### Supported Features Include
- RGB Color Selection
- White Selection
- Brightness 
- Effects with Animation Speed
- Over-the-Air (OTA) Upload from the ArduinoIDE!

Most of the effects incorporate the currently selected color while other effects use pre-defined colors. The input_number and automation in the HA configuration example allow you to easily set a transition speed from HA's user interface without needing to use the Services tool. 

The default speed for the effects is hard coded and is set when the light is first turned on. When changing between effects, the previously used transition speed will take over. If the effects don't look great, play around with the slider to adjust the transition speed (AKA the effect's animation speed). 

#### Optional MOSFET
Addressable LED are different to a normal LED string. Every pixel ALWAYS has power as it is awaiting commands. So even while the string is off. This quiescent current can add up. On a 5-meter strip (60 LEDs per meter) a full watt is used from the strip while the strip is not illuminated. (That’s nearly enough to send a person to the moon)
The sketch and related wiring diagram includes a P-Channel MOSFET which will turn off power to the strip when the strip is off. This means the ESP8266 can always energised and the LED strip will turn on only when it is meant to light up.


#### OTA Uploading
This code also supports remote uploading to the ESP8266 using Arduino's OTA library. To utilize this, you'll need to first upload the sketch using the traditional USB method. However, if you need to update your code after that, your WIFI-connected ESP chip should show up as an option under Tools -> Port -> Porch at your.ip.address.xxx. More information on OTA uploading can be found [here](http://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html). Note: You cannot access the serial monitor over WIFI at this point.  


#### Demo Video
[![Demo Video](http://i.imgur.com/cpW2JAX.png)](https://www.youtube.com/watch?v=DQZ4x6Z3678 "Demo - RGB Digital LED Strip controlled using ESP, MQTT, and Home Assistant")
NOTE: This sketch is using a different set of effects.


#### Tutorial Video
[![Tutorial Video](http://i.imgur.com/9UMl8Xo.jpg)](https://www.youtube.com/watch?v=9KI36GTgwuQ "The BEST Digital LED Strip Light Tutorial - DIY, WIFI-Controllable via ESP, MQTT, and Home Assistant")

#### Sample MQTT commands
Listen to MQTT commands
> mosquitto_sub -h 172.17.0.1 -t '#'

Make the full string white
> mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'white_value': 0, 'effect': 'solid', 'transition': 0, 'brightness': 255}"

Make the full string blue
> mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'color': {'r':0, 'g':0, 'b':255}, 'effect': 'solid', 'transition': 0, 'brightness': 255}"

Wipe the current effect with a color and return
> mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'color': {'r':255, 'g':0, 'b':0}, 'effect': 'color wipe once', 'transition': 10}"

Turn a specific pixel green
> mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'color': {'r':0, 'g':255, 'b':0}, 'effect': 'pixel', 'pixel': [10]}"
> mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'color': {'r':0, 'g':255, 'b':0}, 'effect': 'pixel', 'pixel': [1, 10, 12]}"

Turn on a section of the strip
> mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'color': {'r':0, 'g':255, 'b':0}, 'effect': 'pixel', 'pixel': [0, 50]}"

## Home Assistant config

````yaml
light:
  - platform: mqtt
    schema: json
    name: "Led 6733067"
    state_topic: "led/led-6733067"
    command_topic: "led/led-6733067/set"
    effect: true
    effect_list:
        - solid
        - clear
        - twinkle
        - cylon bounce
        - fire
        - fade in out
        - strobe
        - theater chase
        - rainbow cycle
        - color wipe
        - running lights
        - snow sparkle
        - sparkle
        - twinkle random
        - lightning
    brightness: true
    rgb: true
    optimistic: false
    qos: 0
input_number:
  led_6733067_animation_speed:
    name: Led 6733067 Speed
    initial: 150
    min: 1
    max: 150
    step: 10
automation:
  - alias: "Led 6733067 animation speed"
    trigger:
      - platform: state
        entity_id: input_number.led_6733067_animation_speed
    action:
      - service: mqtt.publish
        data_template:
          topic: "led/led-6733067/set"
          payload: '{"transition":{{ trigger.to_state.state | int }}}'
````