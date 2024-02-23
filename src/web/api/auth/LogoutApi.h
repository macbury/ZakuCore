#ifndef OnLogoutApi_H
#define OnLogoutApi_H

#include <GundamManager.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

void onLogoutApi(AsyncWebServerRequest *request, GundamManager *gundam) {
  AsyncJsonResponse * response = new AsyncJsonResponse();
  auto root = response->getRoot();

  String sid = "";
  if (gundam->auth->getSidFromRequest(request, sid)) {
    gundam->auth->signOut(sid.c_str());
  }

  root["success"] = true;
  response->addHeader("Cache-Control", "no-cache");
  response->addHeader("Set-Cookie", gundam->auth->buildCookie("guest"));

  response->setLength();
  request->send(response);
}

#endif
