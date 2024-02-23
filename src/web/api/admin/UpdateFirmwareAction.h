#ifndef UpdateFirmwareAction_H
#define UpdateFirmwareAction_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <FS.h>

#ifdef USE_ESP32
  #include <Update.h>
#endif
#ifdef USE_ESP8266
  #include <Updater.h>
  #include <flash_hal.h>

  void getEsp8266UpdateErrors(JsonArray &errors) {
    switch (Update.getError()) {
      case UPDATE_ERROR_WRITE:
        errors.add(F("Flash Write Failed"));
      break;
      case UPDATE_ERROR_ERASE:
        errors.add(F("Flash Erase Failed"));
      break;
      case UPDATE_ERROR_READ:
        errors.add(F("Flash Read Failed"));
      break;
      case UPDATE_ERROR_SPACE:
        errors.add(F("Not Enough Space"));
      break;
      case UPDATE_ERROR_SIZE:
        errors.add(F("Bad Size Given"));
      break;
      case UPDATE_ERROR_STREAM:
        errors.add(F("Stream Read Timeout"));
      break;
      case UPDATE_ERROR_NO_DATA:
        errors.add(F("No data supplied"));
      break;
      case UPDATE_ERROR_MD5:
        errors.add("MD5 Failed");
      break;
      case UPDATE_ERROR_SIGN:
        errors.add("Signature verification failed");
      break;
      case UPDATE_ERROR_MAGIC_BYTE:
        errors.add(F("Magic byte is wrong, not 0xE9"));
      break;
      case UPDATE_ERROR_BOOTSTRAP:
        errors.add(F("Invalid bootstrapping state, reset ESP8266 before updating"));
      break;
    }
  }

#endif

//TODO: make this work on esp32
void onUpdateFirmwareAction(AsyncWebServerRequest *request) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  response->addHeader("Connection", "close");
  auto root = response->getRoot();
  root["success"] = !Update.hasError();
  auto errors = root.createNestedArray("errors");

  #ifdef USE_ESP8266
    getEsp8266UpdateErrors(errors);
  #endif

  //TODO: errors for esp32

  response->setLength();
  request->send(response);
}

void onHandleFileUpdateFirmware(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  if (!index) {
    if(!request->hasParam("checksum", true)) {
      Serial.println("Missing checksum");
      return;
    }
    auto checksum = request->getParam("checksum", true)->value().c_str();
    Serial.printf("Current md5: %s\n", checksum);
    if(!Update.setMD5(checksum)) {
      Serial.println("Invalid checksum");
      return;
    }

    #ifdef USE_ESP8266
      Update.runAsync(true);
    #endif

    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    Serial.printf("Initialize update with max size: %i\n", maxSketchSpace);
    if (Update.begin(maxSketchSpace, U_FLASH)) {
      Serial.print("Update started: ");
    } else {
      Serial.println("Failed to start update");
      Update.printError(Serial);
      return;
    }
  }

  if (!Update.hasError()) {
    Serial.print(".");
    if (Update.write(data, len) != len) {
      Update.printError(Serial);
    }
  }

  if (final) {
    if (!Update.end(true)) {
      Serial.println("Failed with update...");
      Update.printError(Serial);
    } else {
      Serial.println("OK. Update complete");
    }
  }
}

#endif
