#ifndef CaptivePortal_H
#define CaptivePortal_H
#include <ESPAsyncWebServer.h>

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    String redirectUrl = "http://" + WiFi.softAPIP().toString() + "/wifi/setup";
    Serial.print("Redirecting to: ");
    Serial.println(redirectUrl);
    request->redirect(redirectUrl);
  }
};

#endif
