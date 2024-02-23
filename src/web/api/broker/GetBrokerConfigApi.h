#ifndef GetBrokerConfigApi_H
#define GetBrokerConfigApi_H

#include <ESPAsyncWebServer.h>
#include <AsyncMqttClient.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <GundamManager.h>

void onGetBrokerConfigApi(AsyncWebServerRequest *request, GundamManager *gundam) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();

  auto broker = root.createNestedObject("broker");

  broker["connected"] = gundam->relay->connected();
  broker["username"] = gundam->configuration->broker.username;
  broker["password"] = gundam->configuration->broker.password;
  broker["address"] = gundam->configuration->broker.address;
  broker["port"] = gundam->configuration->broker.port;

  response->setLength();
  request->send(response);
}

#endif
