#ifndef Web_H
#define Web_H

#include <pins_arduino.h>
#include "web/pages/HomePage.h"
#include "web/api/StatusApi.h"
#include "web/api/wifi/ScanWifiApi.h"
#include "web/api/PingAction.h"
#include "web/api/ResetApiAction.h"
#include "web/api/broker/GetBrokerConfigApi.h"
#include "web/AuthenticationRequiredHandler.h"
#include "web/api/auth/SignInApi.h"
#include "web/api/auth/LogoutApi.h"
#include "web/api/RestartApiAction.h"
#include "web/api/auth/UpdatePasswordApi.h"
#include "web/api/config/ReadConfigApi.h"
#include "web/api/config/configureUpdateConfigApi.h"
#include "web/api/admin/UpdateFirmwareAction.h"
#include "web/api/entities/ReloadEntitiesApiAction.h"

class Web {
  private:
    GundamManager *gundam;
  public:
    Web();
    void setup(GundamManager *gundam);
};

Web::Web() {}

void Web::setup(GundamManager *gundam) {
  this->gundam = gundam;
  auto server = this->gundam->server;
  auto relay = this->gundam->relay;
  auto auth = this->gundam->auth;

  server->onNotFound(onHomePageAction);
  server->on("/", HTTP_GET, onHomePageAction);
  server->on("/index.js", HTTP_GET, onJavascriptBundleAction);
  server->on("/zaku3.svg", HTTP_GET, onSvgZaku3Action);
  server->on("/dots.svg", HTTP_GET, onSvgDotsAction);
  server->on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(404);
  });
  server->on("/api/ping", HTTP_GET, onPingAction);
  server->on("/api/logout", HTTP_GET,  [this](AsyncWebServerRequest *request){
    onLogoutApi(request, this->gundam);
  });
  configureSignInAction(server, auth);

  server->addHandler(new AuthenticationRequiredHandler(auth));
  relay->registerHandler(server);

  // Everything below requires token
  server->on("/api/pins", HTTP_GET, onListPinsApiAction);
  server->on("/api/config/write", HTTP_PUT, onUpdateConfigApiAction, onHandleConfigUpdateBody);
  server->on("/api/config", HTTP_GET, onReadConfigApi);
  server->on("/api/firmware/update", HTTP_POST, onUpdateFirmwareAction, onHandleFileUpdateFirmware);

  server->on("/api/entities/reload", HTTP_GET, [this](AsyncWebServerRequest *request){
    onReloadEntitiesApiAction(request, this->gundam);
  });

  server->on("/api/restart", HTTP_GET, [this](AsyncWebServerRequest *request){
    onRestartApiAction(request, this->gundam);
  });
  server->on("/api/reset", HTTP_GET, [this](AsyncWebServerRequest *request){
    onResetApiAction(request, this->gundam);
  });
  server->on("/api/status", HTTP_GET, [this](AsyncWebServerRequest *request){
    onStatusApiAction(request, this->gundam);
  });
  server->on("/api/broker", HTTP_GET, [this](AsyncWebServerRequest *request){
    onGetBrokerConfigApi(request, this->gundam);
  });
  server->on("/api/wifi/scan", HTTP_GET, [this](AsyncWebServerRequest *request){
    onScanWifiApiAction(request, this->gundam->network);
  });
  configureUpdatePasswordApi(server, this->gundam);
  server->on("/api", HTTP_GET, onPingAction);
}

void GundamManager::pushStatus() {
  DynamicJsonDocument message(PAYLOAD_BUFFER_SIZE);
  auto status = message.createNestedObject("status");
  generateStatusJson(status, this);
  this->relay->publish(message);
}

#endif
