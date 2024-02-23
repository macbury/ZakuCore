#ifndef ScanWifiApi_H
#define ScanWifiApi_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <Network.h>

void onScanWifiApiAction(AsyncWebServerRequest *request, Network *network) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();

  switch (network->scan()) {
    case NetworkScan::Running:
      root["status"] = "running";
    break;

    case NetworkScan::Failed:
      root["status"] = "failed";
    break;

    default:
      root["status"] = "done";
    break;
  }

  response->setLength();
  request->send(response);
}

#endif
