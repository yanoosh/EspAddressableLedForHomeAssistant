#ifndef _WIFI_INO
#define _WIFI_INO
WiFiClient espClient;

void wifiSetup() {
  int retries = 0;
  delay(100);
  _DP("Connecting to SSID: ");
  _DPLN(WIFI_SSID);
  
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);  
  WiFi.hostname(deviceName);

  if (WiFi.status() != WL_CONNECTED) {  // FIX FOR USING 2.3.0 CORE (only .begin if not connected)
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    _DP(".");
    if (retries++ > 20) {
      ESP.restart();
    }
  }

  _DPLN();
  _DPLN("WiFi connected");
  _DP("IP address: ");
  _DPLN(WiFi.localIP());
}

void wifiLoop() {
  if (WiFi.status() != WL_CONNECTED) {
    delay(1);
    _DP( "WIFI Disconnected. Attempting reconnection.");
    wifiSetup();
  }
}

#endif