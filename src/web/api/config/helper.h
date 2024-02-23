#ifndef ConfigHelper_H
#define ConfigHelper_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

struct FileRequest {
  String key;
  String path;
  String tempPath;
  String mimeType;
  bool json;
  bool exists;
};

FileRequest getFilePath(AsyncWebServerRequest *request) {
  if (!request->hasParam("key")) {
    return FileRequest{};
  }

  String key = request->getParam("key")->value();

  String filePath = String("/config/") + key;
  String tempPath = filePath + String(".tmp");
  bool exists = LittleFS.exists(filePath.c_str());
  bool json = key.endsWith(".json");
  auto mimeType = String(json ? "application/json" : "application/octet-stream");

  return FileRequest {
    key, filePath, tempPath, mimeType, exists
  };
}


#endif
