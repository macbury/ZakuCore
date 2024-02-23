#ifndef ConnectBrokerApi_H
#define ConnectBrokerApi_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "Configuration.h"

void configureConnectBrokerApiAction(AsyncWebServer *server, ConfigurationManager *configuration) {
  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/api/broker/connect", [&](AsyncWebServerRequest *request, JsonVariant &json) {
    JsonObject jsonObj = json.as<JsonObject>();
    String address = jsonObj["address"] | "";
    String password = jsonObj["password"] | "";
    String username = jsonObj["username"] | "";
    int port = jsonObj["port"] | 1883;

    AsyncJsonResponse * response = new AsyncJsonResponse();
    auto root = response->getRoot();

    strlcpy(
      configuration->broker.username,
      username.c_str(),
      sizeof(configuration->broker.username)
    );

    strlcpy(
      configuration->broker.password,
      password.c_str(),
      sizeof(configuration->broker.password)
    );

    strlcpy(
      configuration->broker.address,
      address.c_str(),
      sizeof(configuration->broker.address)
    );

    configuration->broker.port = port;

    configuration->save();

    root["success"] = true;
    Serial.println(F("Restarting in 5 seconds"));

    response->setLength();
    request->send(response);

    restartEsp();
  });

  handler->setMethod(HTTP_POST);
  server->addHandler(handler);
}

#endif
