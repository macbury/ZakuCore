#ifndef HomePage_H
#define HomePage_H

#include <ESPAsyncWebServer.h>
#include "index.html.gz.h"
#include "index.js.gz.h"
#include "zaku3.svg.gz.h"
#include "dots.svg.gz.h"

void onHomePageAction(AsyncWebServerRequest *request) {
  if (request->header("If-None-Match").equals(index_html_gz_hash)) {
    Serial.println(F("Client cached the html file"));
    request->send(304);
    return;
  }

  Serial.println(F("Sending html to client"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, index_html_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  response->addHeader("ETag", index_html_gz_hash);

  request->send(response);
}

void onJavascriptBundleAction(AsyncWebServerRequest *request) {
  if (request->header("If-None-Match").equals(index_js_gz_hash)) {
    Serial.println(F("Client cached the js file"));
    request->send(304);
    return;
  }

  Serial.println(F("Sending js to client"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", index_js_gz, index_js_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  response->addHeader("ETag", index_js_gz_hash);

  request->send(response);
}

void onSvgZaku3Action(AsyncWebServerRequest *request) {
  if (request->header("If-None-Match").equals(zaku3_svg_gz_hash)) {
    Serial.println(F("Cached zaku3 svg"));
    request->send(304);
    return;
  }

  Serial.println(F("Sending zaku3.svg"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "image/svg+xml", zaku3_svg_gz, zaku3_svg_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  response->addHeader("ETag", zaku3_svg_gz_hash);

  request->send(response);
}

void onSvgDotsAction(AsyncWebServerRequest *request) {
  if (request->header("If-None-Match").equals(dots_svg_gz_hash)) {
    Serial.println(F("Cached dots svg"));
    request->send(304);
    return;
  }

  Serial.println(F("Sending dots.svg"));
  AsyncWebServerResponse *response = request->beginResponse_P(200, "image/svg+xml", dots_svg_gz, dots_svg_gz_len);
  response->addHeader("Content-Encoding", "gzip");
  response->addHeader("ETag", dots_svg_gz_hash);

  request->send(response);
}


#endif
