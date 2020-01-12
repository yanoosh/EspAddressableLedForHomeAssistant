#ifndef _OTA_INO
#define _OTA_INO

void otaSetup() {
  ArduinoOTA.setPort(OTAport);
  ArduinoOTA.setHostname(deviceName);                 // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setPassword((const char *)OTApassword);  // No authentication by default

#ifdef _DEBUG
  ArduinoOTA.onStart([]() {
    Serial.println("\nStarting");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.println();
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
#endif

  ArduinoOTA.begin();
}

void otaLoop() {
  ArduinoOTA.handle();  // Check OTA Firmware Updates
}

#endif