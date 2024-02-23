#ifndef UpdatePasswordApi_H
#define UpdatePasswordApi_H

#include <functional> // for bind()
using namespace std;
using namespace std::placeholders;

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <GundamManager.h>
#include <ArduinoJson.h>
#include "web/api/auth/SignInApi.h"

void onSignOutApiAction(GundamManager *gundam, AsyncWebServerRequest *request, JsonVariant &json) {
  JsonObject jsonObj = json.as<JsonObject>();
  String password = jsonObj["password"] | "";
  String currentPassword = jsonObj["currentPassword"] | "";

  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();

  bool updatedPassword = gundam->auth->updatePassword(currentPassword, password);
  root["success"] = updatedPassword;

  if (updatedPassword) {
    putNewSidInCookie(response, gundam->auth);
    gundam->restart();
  } else {
    auto errors = root.createNestedArray("errors");
    errors.add("Invalid password");
  }

  response->setLength();
  request->send(response);
}

void configureUpdatePasswordApi(AsyncWebServer *server, GundamManager *gundam) {
  auto logicBind = std::bind(&onSignOutApiAction, gundam, _1, _2);
  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/api/password", logicBind);
  handler->setMethod(HTTP_POST);
  server->addHandler(handler);
}

#endif
