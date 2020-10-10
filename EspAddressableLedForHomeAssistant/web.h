#ifndef _WEB_INO
#define _WEB_INO

ESP8266WebServer server(80);

String row(String title, String value) {
  return String("<dl class='row'><dt class='col-sm-5'>" + title + "<dd class='col-sm-7'>" + value + "</dl>");
}
void ServeWebClients() {
  uint32_t heap = ESP.getFreeHeap();
  String response = F("<!doctype html><html class='h-100'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'><link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css'><title>");
  response += core->getDeviceName();
  response += F("</title><body class='h-100'><div class='row row-cols-1 row-cols-sm-2' style='max-width: 720px; margin:40px auto'><div class='col mb-4'><div class='card'><div class='card-header bg-primary text-white'>");
  response += core->getDeviceName();
  response += F("</div><div class='card-body'>");
  response += row("State", core->isTurnOn() ? "On" : "Off");
  response += row("Red", String(core->getEffect()->getColor().red));
  response += row("Green", String(core->getEffect()->getColor().green));
  response += row("Blue", String(core->getEffect()->getColor().blue));
  response += row("White", String(core->getEffect()->getColor().white));

  response += row("Brightness", String(core->getBrightness()));
  response += row("Speed", String(core->getSpeed()));
  response += row("Effect", core->getEffect()->getActiveName());
  response += "</div></div></div><div class='col mb-4'><div class='card'><div class='card-header'>MQTT</div><div class='card-body'>";
  response += row("server", MQTT_SERVER);
  response += row("topic", core->mqtt->getTopicStatus());
  response += row("available", core->mqtt->getTopicAvailable());
  response += row("command", core->mqtt->getTopicCommand());
  response += row("HA config", core->mqtt->getTopicHomeAssistantConfig());
  response += "</div></div></div><div class='col mb-4'><div class='card'><div class='card-body'>";
  response += row("Led count", String(core->getLength()));
  response += row("Wifi ssid", WIFI_SSID);
  response += row("Wifi signal (dbBm)", String(WiFi.RSSI()));
  response += row("Id", String(ESP.getChipId()));
  response += row("Mac", String(WiFi.macAddress()));
  response += row("Memory heap (kB)", String(heap));
  response += row("Work time (ms)", String(millis()));

  server.send(200, "text/html", response);
}

void webSetup() {
  server.on("/", ServeWebClients);
  server.begin();
}

void webLoop() {
  server.handleClient();
}

#endif