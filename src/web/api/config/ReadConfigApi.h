#ifndef OnReadConfigApi_H
#define OnReadConfigApi_H

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "helper.h"

void onReadConfigApi(AsyncWebServerRequest *request) {
  auto fileRequest = getFilePath(request);
  Serial.printf("Sending: %s to client\n", fileRequest.path.c_str());

  request->send(LittleFS, fileRequest.path.c_str(), fileRequest.mimeType, false);
}

#endif
