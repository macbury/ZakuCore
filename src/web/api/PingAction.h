#ifndef PingApi_H
#define PingApi_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

void onPingAction(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  response->addHeader("Access-Control-Allow-Origin", "*");
  auto root = response->getRoot();
  root["done"] = true;
  response->setLength();
  request->send(response);
}

#endif
