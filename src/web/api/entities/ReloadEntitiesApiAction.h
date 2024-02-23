#ifndef ReloadEntitiesApiAction_H
#define ReloadEntitiesApiAction_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <GundamManager.h>

void onReloadEntitiesApiAction(AsyncWebServerRequest *request, GundamManager *gundam) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();
  gundam->requestEntityReload();

  root["reload"] = true;
  response->setLength();
  request->send(response);
}

#endif
