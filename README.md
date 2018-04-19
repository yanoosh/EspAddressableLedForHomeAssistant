# ESP8266 MQTT JSON SK6812RGBW HomeAssistant
=====================

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


#### Parts List
- [Digital RGBW Leds (SK6812RGBW)](https://www.adafruit.com/product/2842)
- [NodeMCU](https://www.amazon.com/HiLetgo-Version-NodeMCU-Internet-Development/dp/B010O1G1ES/)
- [P-Channel MOSFET](http://au.element14.com/vishay/sup53p06-20-e3/mosfet-p-to-220/dp/1684102)
- [30Ohm Resisters](http://au.element14.com/multicomp/mccfr0w4j0331a50/carbon-film-resistor-330-ohm-250mw/dp/1128021)
- [Aluminum Mounting Channel/Diffuser](https://www.amazon.com/gp/product/B00PJSUZSK)
- [5V 15amp Power Supply](https://www.amazon.com/gp/product/B01LATMSGS)
- [Strip Connector](https://www.amazon.com/gp/product/B01E902DY2)
- [Logic Level Shifter](http://au.element14.com/texas-instruments/sn74ahct125n/logic-bus-buffer-tri-st-qd-14dip/dp/1749628)
- [20 Gauge Wire](https://www.amazon.com/gp/product/B009VCZ4V8)
- [Project Box](https://www.amazon.com/BUD-Industries-NBF-32016-Plastic-Economy/dp/B005UPANU2)
- [Power Jacks](https://www.amazon.com/E-outstanding-Power-Female-5-5mm-Adapter/dp/B011YKCK5M)


#### Wiring Diagram
![alt text](https://github.com/DotNetDann/ESP8266-MQTT-JSON-SK6812RGBW-HomeAssistant/blob/master/Wiring%20Diagram.png?raw=true "Wiring Diagram")

Here is an example of a completed perfboard [Image 1](https://github.com/DotNetDann/ESP8266-MQTT-JSON-SK6812RGBW-HomeAssistant/blob/master/PerfBoard1.jpg?raw=true)  [Image 2](https://github.com/DotNetDann/ESP8266-MQTT-JSON-SK6812RGBW-HomeAssistant/blob/master/PerfBoard2.jpg?raw=true)


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
> mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'color': {'r':0, 'g':255, 'b':0}, 'effect': 'pixel', 'pixel': 10}"
