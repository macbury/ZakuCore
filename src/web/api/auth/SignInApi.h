#ifndef SignIn_H
#define SignIn_H

#include <functional> // for bind()
using namespace std;
using namespace std::placeholders;

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <GundamManager.h>
#include <ArduinoJson.h>

void putNewSidInCookie(AsyncJsonResponse * response, AuthenticationManager *auth) {
  String sid = auth->create();
  Serial.printf("New session generated: %s\n", sid.c_str());
  response->addHeader("Cache-Control", "no-cache");
  response->addHeader("Set-Cookie", auth->buildCookie(sid));
}

void onSignInApiAction(AuthenticationManager *auth, AsyncWebServerRequest *request, JsonVariant &json) {
  JsonObject jsonObj = json.as<JsonObject>();
  String password = jsonObj["password"] | "";

  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();
  bool signedIn = auth->signIn(password.c_str());

  if (!signedIn) {
    delay(5000);
  }

  root["success"] = signedIn;

  if (signedIn) {
    putNewSidInCookie(response, auth);
  } else {
    auto errors = root.createNestedArray("errors");
    errors.add("Invalid password");
  }

  response->setLength();
  request->send(response);
}

void configureSignInAction(AsyncWebServer *server, AuthenticationManager *auth) {
  auto logicBind = std::bind(&onSignInApiAction, auth, _1, _2);

  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/api/sign_in", logicBind);
  handler->setMethod(HTTP_POST);
  server->addHandler(handler);
}

#endif
