#ifndef StatusApi_H
#define StatusApi_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <AsyncMqttClient.h>
#include <GundamManager.h>
#include <consts.h>

void generateStatusJson(JsonVariant root, GundamManager *gundam) {
  auto chipInfo = root.createNestedObject("chipInfo");

  chipInfo["heap"] = ESP.getFreeHeap();
  chipInfo["uptime"] = millis();
  #ifdef USE_ESP8266
    chipInfo["chipId"] = ESP.getChipId();
  #endif
  #ifdef USE_ESP32
    chipInfo["chipId"] = ESP.getChipModel();
  #endif

  auto broker = root.createNestedObject("broker");
  auto wifi = root.createNestedObject("wifi");
  auto firmware = root.createNestedObject("firmware");
  auto gundamJson = root.createNestedObject("gundam");

  firmware["md5"] = ESP.getSketchMD5();
  firmware["release"] = Version::Release;
  firmware["name"] = Version::Name;

  wifi["connected"] = WiFi.isConnected();
  wifi["mac"] = WiFi.macAddress();
  wifi["ssid"] = WiFi.SSID();
  wifi["hostname"] = WiFi.getHostname();

  if (WiFi.isConnected()) {
    wifi["ip"] = WiFi.localIP();
    wifi["rssi"] = WiFi.RSSI();
  }

  broker["connected"] = gundam->relay->connected();

  if (gundam->relay->connected()) {
    broker["address"] = gundam->configuration->broker.address;
    broker["port"] = gundam->configuration->broker.port;
  }

  gundamJson["name"] = gundam->configuration->gundam.name;
  gundamJson["hostname"] = gundam->configuration->gundam.hostname;
  gundamJson["model"] = gundam->configuration->gundam.model;
}

void onStatusApiAction(AsyncWebServerRequest *request, GundamManager *gundam) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();
  generateStatusJson(root, gundam);

  response->setLength();
  request->send(response);
}

#endif
