# Light controller using esp

Main goal of project is to provide illumination for special occasions e.g christmas, halloween or valentine's day. Work with addresable led and controled by ESP82XX family devices

## Features

- effect 
- work with led strip
  - WS2811
  - WS2812B (should work, not tested)
  - SK6812 (should work, not tested)
- mqtt
  - protocol compatibile with [Home Assistanet mqtt light](https://home-assistant.io/components/light.mqtt_json/)
  - HomeAssitant discovery
- update over wifi (OTA)

## Devices

Tested and work all features:
- esp8266mod
- SP501e (esp8285)

## Inspirations

- [tasmota](https://github.com/arendst/Tasmota)
- source code for project:
  - [ESP-MQTT-JSON-Digital-LEDs](https://github.com/bruhautomation/ESP-MQTT-JSON-Digital-LEDs)
  - [ESP8266-MQTT-JSON-SK6812RGBW-HomeAssistant](https://github.com/DotNetDann/ESP8266-MQTT-JSON-SK6812RGBW-HomeAssistant)
- effects [adruino-led-strip-effects](https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects)


## Sample MQTT commands

Make the full string white

    mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'white_value': 0, 'effect': 'solid', 'transition': 0, 'brightness': 255}"

Make the full string blue

    mosquitto_pub -h 172.17.0.1 -t led/kitchen/set -m "{'state': 'ON', 'color': {'r':0, 'g':0, 'b':255}, 'effect': 'solid', 'transition': 0, 'brightness': 255}"

## troubleshoot
### no status led

During booting device set last brightness so when the last persisted value is very low led display status but very dark. Disable load setting from EEPROM or change brightness.
