#ifndef ResetApiAction_H
#define ResetApiAction_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Configuration.h>
#include <GundamManager.h>

void onResetApiAction(AsyncWebServerRequest *request, GundamManager *gundam) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();
  root["reset"] = true;

  gundam->configuration->cleanup();
  gundam->auth->cleanup();

  response->setLength();
  request->send(response);

  gundam->restart();
}

#endif
