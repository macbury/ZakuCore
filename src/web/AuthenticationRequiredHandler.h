#ifndef AuthenticationRequiredHandler_H
#define AuthenticationRequiredHandler_H

#include <Arduino.h>
#include <AuthenticationManager.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

const bool AUTHORIZE_REQUEST = false;
const bool DENY_REQUEST = true;
class AuthenticationRequiredHandler : public AsyncWebHandler {
private:
  AuthenticationManager *auth;
public:
  AuthenticationRequiredHandler(AuthenticationManager *auth) {
    this->auth = auth;
  }

  virtual ~AuthenticationRequiredHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    if (!request->url().startsWith("/api")) {
      return AUTHORIZE_REQUEST;
    }

    if (!WiFi.isConnected()) {
      return AUTHORIZE_REQUEST;
    }

    String sid = "";
    if (this->auth->getSidFromRequest(request, sid) && this->auth->authenticate(sid.c_str())) {
      return AUTHORIZE_REQUEST;
    } else {
      return DENY_REQUEST;
    }
  }

  void handleRequest(AsyncWebServerRequest *request) {
    AsyncJsonResponse * response = new AsyncJsonResponse();
    auto root = response->getRoot();
    auto errors = root.createNestedArray("errors");
    errors.add(F("You need to sign in or sign up before continuing."));

    response->setCode(403);
    response->setLength();
    request->send(response);
  }
};

#endif
