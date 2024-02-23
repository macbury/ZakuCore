#ifndef RestartApiAction_H
#define RestartApiAction_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

void onRestartApiAction(AsyncWebServerRequest *request, GundamManager *gundam) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();
  root["restart"] = true;

  response->setLength();
  request->send(response);

  gundam->restart();
}

#endif
