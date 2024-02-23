#ifndef UpdateConfigApi_H
#define UpdateConfigApi_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "helper.h"

void onUpdateConfigApiAction(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  response->addHeader("Connection", "close");
  auto root = response->getRoot();
  root["write"] = true;
  response->setLength();
  request->send(response);
}

void onHandleConfigUpdateBody(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  auto fileRequest = getFilePath(request);
  File tempFile = LittleFS.open(fileRequest.tempPath.c_str(), !index ? "w" : "a"
    #ifdef USE_ESP32
      , true
    #endif
  );

  if (!index) {
    Serial.printf("------- Writing config into: %s\n", fileRequest.tempPath.c_str());
  }

  Serial.printf("Writing: %i\n", len);
  tempFile.write(data, len);
  tempFile.flush();
  tempFile.close();

  if (final) {
    Serial.printf("Moving json from: %s to %s\n", fileRequest.tempPath.c_str(), fileRequest.path.c_str());
    LittleFS.remove(fileRequest.path.c_str());
    LittleFS.rename(fileRequest.tempPath.c_str(), fileRequest.path.c_str());
  }
}

#endif
