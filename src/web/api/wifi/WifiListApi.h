#ifndef WifiList_H
#define WifiList_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

void onWifiListAction(AsyncWebServerRequest *request, GundamManager *gundam) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();

  auto networks = root.createNestedArray("networks");
  auto scanResult = WiFi.scanComplete();

  root["scan"] = scanResult == WIFI_SCAN_RUNNING;

  if (scanResult > 0) {
    for (int8_t i = 0; i < scanResult; i++) {
      auto ssid = WiFi.SSID(i);

      auto network = networks.createNestedObject();
      network["ssid"] = ssid;
      network["rssi"] = WiFi.RSSI(i);
      network["encryption"] = WiFi.encryptionType(i);
    }
  } else {
    Serial.println("No wifi networks detected...");
  }

  response->setLength();
  request->send(response);
}

#endif
